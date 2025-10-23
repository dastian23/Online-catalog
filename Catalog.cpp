#include "Catalog.h"
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <set>
#include <algorithm>
#include <iostream>
#include <stdexcept>

// Constructor
Catalog::Catalog(const std::string &fileElevi,
                 const std::string &fileNote,
                 const std::string &fileAbsente)
    : fisierElevi(fileElevi),
      fisierNote(fileNote),
      fisierAbsente(fileAbsente)
{
    incarcaElevi();
    incarcaNote();
    incarcaAbsente();
}

// --- incarcaElevi: format "<id>;<Prenume> <Nume>"
void Catalog::incarcaElevi() {
    std::ifstream in(fisierElevi);
    if (!in) throw std::runtime_error("Nu pot deschide fisierul elevi: " + fisierElevi);

    std::string linie;
    while (std::getline(in, linie)) {
        if (linie.empty()) continue;
        std::stringstream ss(linie);

        std::string token;
        std::getline(ss, token, ';');
        int id = std::stoi(token);

        std::string numeComplet;
        std::getline(ss, numeComplet);
        if (!numeComplet.empty() && numeComplet[0] == ' ')
            numeComplet.erase(0, 1);

        elevi.emplace_back(id, numeComplet);
    }
    in.close();
}

// --- incarcaNote: format "<id>;<materie>;<valoare>"
void Catalog::incarcaNote() {
    std::ifstream in(fisierNote);
    if (!in) throw std::runtime_error("Nu pot deschide fisierul note: " + fisierNote);

    std::string linie;
    while (std::getline(in, linie)) {
        if (linie.empty()) continue;
        std::stringstream ss(linie);

        std::string token;
        std::getline(ss, token, ';');
        int id = std::stoi(token);

        std::string materie;
        std::getline(ss, materie, ';');

        std::getline(ss, token);
        int valoare = std::stoi(token);

        note.emplace_back(id, materie, valoare);
    }
    in.close();
}

// --- incarcaAbsente: format "<id>;<materie>;<data>;<0 sau 1>"
void Catalog::incarcaAbsente() {
    std::ifstream in(fisierAbsente);
    if (!in) throw std::runtime_error("Nu pot deschide " + fisierAbsente);
    std::string linie;
    while (std::getline(in, linie)) {
        if (linie.empty()) continue;
        std::stringstream ss(linie);

        std::string token;
        std::getline(ss, token, ';');
        int id = std::stoi(token);

        std::string materie;
        std::getline(ss, materie, ';');

        std::string data;
        std::getline(ss, data, ';');

        // ultimul câmp = 0 sau 1
        std::getline(ss, token);
        bool mov = (token == "1");

        absente.emplace_back(id, materie, data, mov);
    }
}


// --- afișare subcatalog pentru un elev
void Catalog::afisareCatalogElev(int id) {
    // 1) Verificăm existența elevului și reținem numele
    std::string numeElev;
    bool gasitElev = false;
    Elev *e = findIf(elevi, [&](const Elev &x){
    return x.getId() == id;
    });

    if (e) {
        numeElev = e->getNume();
        gasitElev = true;
    } else {
        gasitElev = false;
    }
    if (!gasitElev) {
        std::cout << "Nu exista elev cu ID = " << id << ".\n";
        return;
    }

    // 2) Afișăm numele elevului pe primul rând
    std::cout << "Elev: " << numeElev << "\n\n";

    // 3) Construim mapările: materia -> vector de note și materia -> vector de (dată, motivată)
    std::map<std::string, std::vector<int>> mapNote;
    std::map<std::string, std::vector<std::pair<std::string,bool>>> mapAbs;

    for (auto &n : note) {
        if (n.getIdElev() == id) {
            mapNote[n.getMaterie()].push_back(n.getValoare());
        }
    }
    for (auto &a : absente) {
        if (a.getIdElev() == id) {
            mapAbs[a.getMaterie()].emplace_back(a.getData(), a.esteMotivata());
        }
    }

    // 4) Obținem lista unică de materii (sortată alfabetic)
    std::set<std::string> setMaterii;
    for (auto &p : mapNote) setMaterii.insert(p.first);
    for (auto &p : mapAbs)  setMaterii.insert(p.first);

    if (setMaterii.empty()) {
        std::cout << "Elevul cu ID = " << id << " nu are note sau absente.\n";
        return;
    }

    // 5) Transferăm materiile într-un vector pentru indexare
    std::vector<std::string> materii(setMaterii.begin(), setMaterii.end());
    constexpr size_t MAX_COL = 5;
    size_t total = materii.size();

    // 6) Pentru fiecare bloc de câte MAX_COL materii:
    for (size_t start = 0; start < total; start += MAX_COL) {
        size_t end = std::min(start + MAX_COL, total);
        std::vector<std::string> blocMaterii;
        for (size_t i = start; i < end; ++i) {
            blocMaterii.push_back(materii[i]);
        }
        size_t coloane = blocMaterii.size();

        // 7) Pregătim șirurile de note / absențe și calculăm lățimile subcoloanelor
        std::vector<std::string> siruriNote(coloane), siruriAbs(coloane);
        std::vector<size_t> latNOTE(coloane), latABS(coloane), latCOL(coloane);

        for (size_t i = 0; i < coloane; ++i) {
            const auto &mat = blocMaterii[i];

            // (a) Șir note
            if (mapNote.count(mat)) {
                std::ostringstream oss;
                for (size_t j = 0; j < mapNote[mat].size(); ++j) {
                    oss << mapNote[mat][j];
                    if (j + 1 < mapNote[mat].size()) oss << ", ";
                }
                siruriNote[i] = oss.str();
            } else {
                siruriNote[i] = "-";
            }

            // (b) Șir absențe cu indicarea motivării: "data(m)" dacă e motivată
            if (mapAbs.count(mat)) {
                std::ostringstream oss;
                for (size_t j = 0; j < mapAbs[mat].size(); ++j) {
                    oss << mapAbs[mat][j].first;
                    if (mapAbs[mat][j].second) oss << "(m)";
                    if (j + 1 < mapAbs[mat].size()) oss << ", ";
                }
                siruriAbs[i] = oss.str();
            } else {
                siruriAbs[i] = "-";
            }

            // (c) Calculează lățimea fiecărei subcolane
            latNOTE[i] = std::max(siruriNote[i].size(), std::string("Note").size());
            latABS[i]  = std::max(siruriAbs[i].size(),  std::string("Absente").size());

            // (d) Lățimea totală a coloanei = lățimea materiei vs. subcolane + padding
            size_t wSub = latNOTE[i] + 1 + latABS[i]; // 1 spațiu între Note și Absente
            latCOL[i] = std::max(blocMaterii[i].size(), wSub) + 2;
            // +2 padding exterior (spații de-o parte și de alta)
        }

        // 8) Afișăm rândul cu numele materiilor, încadrat de '|'
        for (size_t i = 0; i < coloane; ++i) {
            std::cout << "| "
                      << std::left << std::setw(latCOL[i] - 2) << blocMaterii[i]
                      << " ";
        }
        std::cout << "|\n";

        // 9) Afișăm rândul cu subheaderele "Note" și "Absente"
        for (size_t i = 0; i < coloane; ++i) {
            size_t wNote = latNOTE[i];
            size_t wAbs  = latABS[i];
            std::cout << "| "
                      << std::left << std::setw(wNote) << "Note"
                      << " "
                      << std::left << std::setw(wAbs) << "Absente"
                      << std::string(latCOL[i] - (wNote + 1 + wAbs) - 2, ' ')
                      << " ";
        }
        std::cout << "|\n";

        // 10) Afișăm rândul cu valorile efective (note și absențe)
        for (size_t i = 0; i < coloane; ++i) {
            size_t wNote = latNOTE[i];
            size_t wAbs  = latABS[i];
            std::cout << "| "
                      << std::left << std::setw(wNote) << siruriNote[i]
                      << " "
                      << std::left << std::setw(wAbs) << siruriAbs[i]
                      << std::string(latCOL[i] - (wNote + 1 + wAbs) - 2, ' ')
                      << " ";
        }
        std::cout << "|\n\n";  // un rând gol înainte de următorul bloc
    }
}
// --- adaugare nota
void Catalog::adaugareNota(int id, const std::string &materie, int valoare) {
    // 1) Verificam daca exista elevul cu ID-ul dat
    bool gasitElev = false;
    for (auto &e : elevi) {
        if (e.getId() == id) {
            gasitElev = true;
            break;
        }
    }
    if (!gasitElev) {
        std::cout << "Nu exista elev cu ID = " << id << ".\n";
        return;
    }

    // 2) Verificam daca valoarea notei este intre 1 si 10
    if (valoare < 1 || valoare > 10) {
        std::cout << "Nota invalida. Trebuie sa fie intre 1 si 10.\n";
        return;
    }

    // 3) Adaugam nota si afisam mesaj de succes
    note.emplace_back(id, materie, valoare);
    std::cout << "Nota " << valoare
              << " la materia " << materie
              << " a fost adaugata cu succes.\n";

    // 4) Dupa adaugare, afisam toate notele curente ale elevului cu ID-ul dat,
    //    folosind operator<< pentru obiectul Nota
    std::cout << "Notele elevului cu ID = " << id << " sunt acum:\n";
    for (const auto &n : note) {
        if (n.getIdElev() == id) {
            // folosește operator<<(ostream&, const Nota&)
            std::cout << "  " << n << "\n";
        }
    }
    std::cout << "\n"; // linie goala de separare
}

// --- stergere nota (duplicat cu aceleași atribute)
void Catalog::stergereNota(int id, const std::string &materie, int valoare) {
    // 1) Verificăm dacă există elevul cu ID‐ul dat
    bool gasitElev = false;
    for (auto &e : elevi) {
        if (e.getId() == id) {
            gasitElev = true;
            break;
        }
    }
    if (!gasitElev) {
        std::cout << "Nu exista elev cu ID = " << id << ".\n";
        return;
    }

    // 2) Căutăm nota exactă (id, materie, valoare)
    auto it = std::find_if(
        note.begin(), note.end(),
        [&](const Nota &n) {
            return n.getIdElev() == id &&
                   n.getMaterie() == materie &&
                   n.getValoare() == valoare;
        }
    );

    // 3) Dacă nu am găsit nota, anunțăm și ieșim
    if (it == note.end()) {
        std::cout << "Nu exista nota " << valoare
                  << " la materia " << materie
                  << " pentru elevul cu ID = " << id << ".\n";
        return;
    }

    // 4) Ștergem nota și afișăm mesaj de confirmare
    note.erase(it);
    std::cout << "Nota " << valoare
              << " la materia " << materie
              << " pentru elevul cu ID = " << id
              << " a fost stearsa cu succes.\n";
}
bool Catalog::esteDataValida(const std::string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    int an, luna, zi;
    try {
        an   = std::stoi(date.substr(0, 4));
        luna = std::stoi(date.substr(5, 2));
        zi   = std::stoi(date.substr(8, 2));
    } catch (...) {
        return false;
    }
    if (luna < 1 || luna > 12) return false;
    if (zi < 1 || zi > 31)   return false;
    static const int zileLuna[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxZi = zileLuna[luna];
    if (luna == 2) {
        bool bisect = ((an % 4 == 0 && an % 100 != 0) || (an % 400 == 0));
        if (bisect) maxZi = 29;
    }
    if (zi > maxZi) return false;
    return true;
}

// --- adaugare absenta (implicit nemotivat)
void Catalog::adaugareAbsenta(int id, const std::string &materie, const std::string &data) {
    // 1) Verificam daca exista elevul cu ID-ul dat
    bool gasitElev = false;
    for (auto &e : elevi) {
        if (e.getId() == id) {
            gasitElev = true;
            break;
        }
    }
    if (!gasitElev) {
        std::cout << "Nu exista elev cu ID = " << id << ".\n";
        return;
    }

    // 2) Verificam daca data respecta formatul YYYY-MM-DD
    if (!esteDataValida(data)) {
        std::cout << "Data invalida: " << data
                  << ". Trebuie sa fie in formatul YYYY-MM-DD.\n";
        return;
    }

    // 3) Verificam daca absenta exista deja (id, materie, data)
    for (auto &a : absente) {
        if (a.getIdElev() == id && a.getMaterie() == materie && a.getData() == data) {
            std::cout << "Absenta exista deja: ID=" << id
                      << ", Materie=" << materie
                      << ", Data=" << data << ".\n";
            return;
        }
    }

    // 4) Adaugam absența (initial nemotivata) și afișăm mesaj de succes
    absente.emplace_back(id, materie, data, false);
    std::cout << "Absenta pentru elevul cu ID = " << id
              << " la materia " << materie
              << " in data " << data
              << " a fost adaugata cu succes.\n";

    // 5) După adăugare, afișăm toate absențele curente ale elevului cu ID-ul dat,
    //    folosind operator<< pentru obiectul Absenta
    std::cout << "Absentele elevului cu ID = " << id << " sunt acum:\n";
    for (const auto &a : absente) {
        if (a.getIdElev() == id) {
            // folosește operator<<(ostream&, const Absenta&)
            std::cout << "  " << a << "\n";
        }
    }
    std::cout << "\n"; // linie goala de separare
}

// --- stergere absenta cu exact aceleași atribute
void Catalog::stergereAbsenta(int id, const std::string &materie, const std::string &data) {
    // 1) Verificăm dacă există elevul cu ID‐ul dat
    bool gasitElev = false;
    for (auto &e : elevi) {
        if (e.getId() == id) {
            gasitElev = true;
            break;
        }
    }
    if (!gasitElev) {
        std::cout << "Nu exista elev cu ID = " << id << ".\n";
        return;
    }

    // 2) Căutăm absența exactă (id, materie, data)
    auto it = std::find_if(
        absente.begin(), absente.end(),
        [&](const Absenta &a) {
            return a.getIdElev() == id &&
                   a.getMaterie() == materie &&
                   a.getData() == data;
        }
    );

    // 3) Dacă nu am găsit absența, anunțăm și ieșim
    if (it == absente.end()) {
        std::cout << "Nu exista absenta pentru elevul cu ID = " << id
                  << " la materia " << materie
                  << " in data " << data << ".\n";
        return;
    }

    // 4) Ștergem absența și afișăm mesaj de confirmare
    absente.erase(it);
    std::cout << "Absenta pentru elevul cu ID = " << id
              << " la materia " << materie
              << " in data " << data
              << " a fost stearsa cu succes.\n";
}

// --- NU modifici adaugareAbsenta sau adaugareElev aici; vine mai jos motivareAbsenta

// --- afișează întregul catalog (toți elevii cu note+absențe)
void Catalog::afisareCatalog() {
    for (auto &e : elevi) {
        afisareCatalogElev(e.getId());
        std::cout << "-----------------------\n";
    }
}

// --- adăugare elev (numeComplet = "Prenume Nume")
void Catalog::adaugareElev(int id, const std::string &numeComplet) {
    // 1) Verifică dacă există deja un Elev cu acest ID
    Elev *existent = findIf(elevi, [&](const Elev &x){ return x.getId() == id; });
    if (existent) {
        std::cout << "Exista deja un elev cu ID = " << id << ".\n";
        return;
    }

    // 2) Dacă nu există, adaugă-l și anunță succesul
    elevi.emplace_back(id, numeComplet);
    std::cout << "Elevul \"" << numeComplet << "\" cu ID = " << id
              << " a fost adaugat cu succes.\n";
}

// --- stergere elev (după ID) + șterge și toate notele/absenele asociate
void Catalog::stergereElev(int id) {
    // Verificam daca exista un elev cu acest ID
    const Elev *e = findIf(elevi, [&](const Elev &x) {
    return x.getId() == id;
    });

    bool found = (e != nullptr);

    if (!found) {
        std::cout << "Nu exista elev cu ID = " << id << ".\n";
        return;
    }

    // Daca am ajuns aici, inseamna ca exista; stergem elevul si asociatele
    elevi.erase(
        std::remove_if(elevi.begin(), elevi.end(),
            [&](const Elev &e) {
                return e.getId() == id;
            }),
        elevi.end()
    );

    // Stergem si notele asociate, daca exista
    note.erase(
        std::remove_if(note.begin(), note.end(),
            [&](const Nota &n) {
                return n.getIdElev() == id;
            }),
        note.end()
    );

    // Stergem si absentele asociate
    absente.erase(
        std::remove_if(absente.begin(), absente.end(),
            [&](const Absenta &a) {
                return a.getIdElev() == id;
            }),
        absente.end()
    );

    std::cout << "Elevul cu ID = " << id << " a fost sters cu succes.\n";
}

// --- motivareAbsenta modifica flag-ul (nu mai adauga o absenta noua):
void Catalog::motivareAbsenta(int id, const std::string &materie, const std::string &data) {
    bool found = false;
    for (auto &a : absente) {
        if (a.getIdElev() == id && a.getMaterie() == materie && a.getData() == data) {
            found = true;
            if (a.esteMotivata()) {
                std::cout << "Absenta " << id << ";" << materie << ";" << data
                          << " este deja motivata.\n";
            } else {
                a.setMotivata(true);
                std::cout << "Absenta " << id << ";" << materie << ";" << data
                          << " a fost motivata cu succes.\n";
            }
            break;
        }
    }
    if (!found) {
        std::cout << "Nu exista absenta " << id << ";" << materie << ";" << data
                  << " de motivat.\n";
    }
}

// --- salvare: scrie elevi, note și absențe (include flag-ul):
void Catalog::salvare() {
    // 1) Scrie elevii
    std::ofstream outElevi(fisierElevi);
    if (!outElevi) throw std::runtime_error("Eroare la deschidere " + fisierElevi);
    for (auto &e : elevi) {
        outElevi << e.getId() << ";" << e.getNume() << "\n";
    }
    outElevi.close();

    // 2) Scrie notele
    std::ofstream outNote(fisierNote);
    if (!outNote) throw std::runtime_error("Eroare la deschidere " + fisierNote);
    for (auto &n : note) {
        outNote << n.getIdElev() << ";" << n.getMaterie() << ";" << n.getValoare() << "\n";
    }
    outNote.close();

    // 3) Scrie absențele cu flag (0 sau 1)
    std::ofstream outAbs(fisierAbsente);
    if (!outAbs) throw std::runtime_error("Eroare la deschidere " + fisierAbsente);
    for (auto &a : absente) {
        outAbs << a.getIdElev() << ";"
               << a.getMaterie() << ";"
               << a.getData() << ";"
               << (a.esteMotivata() ? "1" : "0")
               << "\n";
    }
    outAbs.close();
}

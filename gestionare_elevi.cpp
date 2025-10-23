#include "Elev.h"
#include "Nota.h"
#include "Absenta.h"
#include "Catalog.h"
#include <iostream>
#include <string>
#include <exception>

void printUsage() {
    std::cerr << "Utilizare:\n"
              << "  gestionare_elevi afisare_catalog\n"
              << "  gestionare_elevi adaugare_elev <id> <Prenume> <Nume>\n"
              << "  gestionare_elevi stergere_elev <id>\n"
              << "  gestionare_elevi motivare_absenta <id_elev> <materie> <data>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    Catalog catalog("data/elevi.txt",
                    "data/note.txt",
                    "data/absente.txt");

    std::string cmd = argv[1];

    try {
        if (cmd == "afisare_catalog") {
            // Ne asteptam la 2 argumente: [0]=program, [1]=comanda
            if (argc != 2) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_elevi afisare_catalog\n";
                return 1;
            }
            catalog.afisareCatalog();
        }
        else if (cmd == "adaugare_elev") {
            // Ne asteptam la 5 argumente: [0], [1], <id>, <Prenume>, <Nume>
            if (argc != 5) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_elevi adaugare_elev <id> <Prenume> <Nume>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            std::string prenume = argv[3];
            std::string nume    = argv[4];
            std::string numeComplet = prenume + " " + nume;
            catalog.adaugareElev(id, numeComplet);
        }
        else if (cmd == "stergere_elev") {
            // Ne asteptam la 3 argumente: [0], [1], <id>
            if (argc != 3) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_elevi stergere_elev <id>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            catalog.stergereElev(id);
        }
        else if (cmd == "motivare_absenta") {
            // Ne asteptam la 5 argumente: [0], [1], <id_elev>, <materie>, <data>
            if (argc != 5) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_elevi motivare_absenta <id_elev> <materie> <data>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            std::string materie = argv[3];
            std::string data    = argv[4];
            catalog.motivareAbsenta(id, materie, data);
        }
        else {
            std::cerr << "Comanda necunoscuta: " << cmd << "\n";
            printUsage();
            return 1;
        }

        // Salvam modificarile in fisiere
        catalog.salvare();
    }
    catch (const std::exception &ex) {
        std::cerr << "Eroare runtime: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

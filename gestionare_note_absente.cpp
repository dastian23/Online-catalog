#include "Elev.h"
#include "Nota.h"
#include "Absenta.h"
#include "Catalog.h"
#include <iostream>
#include <string>
#include <exception>

void printUsage() {
    std::cerr << "Utilizare:\n"
              << "  gestionare_note_absente afisare_catalog_elev <id_elev>\n"
              << "  gestionare_note_absente adaugare_nota <id_elev> <materie> <valoare>\n"
              << "  gestionare_note_absente stergere_nota <id_elev> <materie> <valoare>\n"
              << "  gestionare_note_absente adaugare_absenta <id_elev> <materie> <data>\n"
              << "  gestionare_note_absente stergere_absenta <id_elev> <materie> <data>\n";
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
        if (cmd == "afisare_catalog_elev") {
            // Ne asteptam la exact 3 argumente: [0]=program, [1]=comanda, [2]=id_elev
            if (argc != 3) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_note_absente afisare_catalog_elev <id_elev>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            catalog.afisareCatalogElev(id);
        }
        else if (cmd == "adaugare_nota") {
            // Ne asteptam la exact 5 argumente: [0], [1], <id_elev>, <materie>, <valoare>
            if (argc != 5) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_note_absente adaugare_nota <id_elev> <materie> <valoare>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            std::string materie = argv[3];
            int valoare = std::stoi(argv[4]);
            catalog.adaugareNota(id, materie, valoare);
        }
        else if (cmd == "stergere_nota") {
            // Ne asteptam la exact 5 argumente: [0], [1], <id_elev>, <materie>, <valoare>
            if (argc != 5) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_note_absente stergere_nota <id_elev> <materie> <valoare>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            std::string materie = argv[3];
            int valoare = std::stoi(argv[4]);
            catalog.stergereNota(id, materie, valoare);
        }
        else if (cmd == "adaugare_absenta") {
            // Ne asteptam la exact 5 argumente: [0], [1], <id_elev>, <materie>, <data>
            if (argc != 5) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_note_absente adaugare_absenta <id_elev> <materie> <data>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            std::string materie = argv[3];
            std::string dataAbs = argv[4];
            catalog.adaugareAbsenta(id, materie, dataAbs);
        }
        else if (cmd == "stergere_absenta") {
            // Ne asteptam la exact 5 argumente: [0], [1], <id_elev>, <materie>, <data>
            if (argc != 5) {
                std::cerr << "Sintaxa invalida! Sintaxa corecta: "
                          << "./gestionare_note_absente stergere_absenta <id_elev> <materie> <data>\n";
                return 1;
            }
            int id = std::stoi(argv[2]);
            std::string materie = argv[3];
            std::string dataAbs = argv[4];
            catalog.stergereAbsenta(id, materie, dataAbs);
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

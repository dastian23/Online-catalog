#include "Elev.h"
#include <iostream>

Elev::Elev(int id_, const std::string &numeComplect)
    : id(id_), nume(numeComplect)
{}

int Elev::getId() const {
    return id;
}

const std::string& Elev::getNume() const {
    return nume;
}


std::ostream& operator<<(std::ostream &out, const Elev &e) {
    out << "[ID: " << e.getId() << ", Nume: " << e.getNume() << "]";
    return out;
}

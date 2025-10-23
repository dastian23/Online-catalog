// src/Nota.cpp
#include "Nota.h"
#include <iostream>

Nota::Nota(int id_, const std::string &mat, int val)
    : idElev(id_), materie(mat), valoare(val)
{}

int Nota::getIdElev() const {
    return idElev;
}

const std::string& Nota::getMaterie() const {
    return materie;
}

int Nota::getValoare() const {
    return valoare;
}

std::ostream& operator<<(std::ostream &out, const Nota &n) {
    out <<"Materie: " << n.getMaterie()
        << ", Nota: " << n.getValoare();
    return out;
}

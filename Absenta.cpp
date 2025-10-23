
#include "Absenta.h"
#include <iostream>

Absenta::Absenta(int id, const std::string &mat, const std::string &d, bool mov)
    : idElev(id), materie(mat), data(d), motivata(mov)
{}

int Absenta::getIdElev() const {
    return idElev;
}

const std::string& Absenta::getMaterie() const {
    return materie;
}

const std::string& Absenta::getData() const {
    return data;
}

bool Absenta::esteMotivata() const {
    return motivata;
}

void Absenta::setMotivata(bool m) {
    motivata = m;
}

std::ostream& operator<<(std::ostream &out, const Absenta &a) {
    out <<"Materie: " << a.getMaterie()
        << ", Data: " << a.getData()
        << ", Motivata: " << (a.esteMotivata() ? "DA" : "NU");
    return out;
}

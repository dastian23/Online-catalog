// include/Nota.h
#ifndef NOTA_H
#define NOTA_H

#include <string>
#include <ostream>

class Nota {
private:
    int idElev;
    std::string materie;
    int valoare;
public:
    Nota(int id_, const std::string &mat, int val);
    int getIdElev() const;
    const std::string& getMaterie() const;
    int getValoare() const;
};

std::ostream& operator<<(std::ostream &out, const Nota &n);

#endif

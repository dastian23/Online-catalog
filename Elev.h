#ifndef ELEV_H
#define ELEV_H

#include <string>
#include <ostream>

class Elev {
private:
    int id;
    std::string nume;  // ex: "Prenume Nume"
public:
    Elev(int id_, const std::string &numeComplect);
    int getId() const;
    const std::string& getNume() const;
};

std::ostream& operator<<(std::ostream &out, const Elev &e);

#endif

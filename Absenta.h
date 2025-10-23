// include/Absenta.h
#ifndef ABSENTA_H
#define ABSENTA_H

#include <string>
#include <ostream>

class Absenta {
private:
    int idElev;
    std::string materie;
    std::string data;
    bool motivata;
public:
    Absenta(int id, const std::string &mat, const std::string &d, bool mov = false);
    int getIdElev() const;
    const std::string& getMaterie() const;
    const std::string& getData() const;
    bool esteMotivata() const;
    void setMotivata(bool m);
};

// Declarație operator<< extern
std::ostream& operator<<(std::ostream &out, const Absenta &a);

#endif // ABSENTA_H

// include/Catalog.h
#ifndef CATALOG_H
#define CATALOG_H

#include "Elev.h"
#include "Nota.h"
#include "Absenta.h"

#include <vector>
#include <algorithm>
#include <string>
#include <ostream>

// ==== FUNCȚIE TEMPLATE PENTRU FIND_IF ÎN VECTOR ====
// Caută în vector și returnează pointer la elementul care satisface predicatul,
// sau nullptr dacă nu există.
template<typename T, typename Predicate>
T* findIf(std::vector<T> &vec, Predicate pred) {
    auto it = std::find_if(vec.begin(), vec.end(), pred);
    return (it != vec.end()) ? &(*it) : nullptr;
}

template<typename T, typename Predicate>
const T* findIf(const std::vector<T> &vec, Predicate pred) {
    auto it = std::find_if(vec.begin(), vec.end(), pred);
    return (it != vec.end()) ? &(*it) : nullptr;
}
// ====================================================

class Catalog {
private:
    std::vector<Elev> elevi;
    std::vector<Nota> note;
    std::vector<Absenta> absente;

    std::string fisierElevi;
    std::string fisierNote;
    std::string fisierAbsente;

    void incarcaElevi();
    void incarcaNote();
    void incarcaAbsente();

    static bool esteDataValida(const std::string &date);

public:
    Catalog(const std::string &fileElevi,
            const std::string &fileNote,
            const std::string &fileAbsente);

    void afisareCatalog() ;
    void afisareCatalogElev(int id) ;

    void adaugareElev(int id, const std::string &numeComplet);
    void stergereElev(int id);

    void adaugareNota(int id, const std::string &materie, int valoare);
    void stergereNota(int id, const std::string &materie, int valoare);

    void adaugareAbsenta(int id, const std::string &materie, const std::string &data);
    void stergereAbsenta(int id, const std::string &materie, const std::string &data);
    void motivareAbsenta(int id, const std::string &materie, const std::string &data);

    void salvare() ;
};

#endif // CATALOG_H

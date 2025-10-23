PROIECT: SISTEM CATALOG ONLINE (C++)

DESCRIERE:
Sistemul Catalog Online este o aplicație de tip CLI (Command Line Interface) scrisă în C++ pentru gestionarea datelor școlare (elevi, note și absențe). Datele sunt stocate și persistate în fișiere text separate.

I. STRUCTURA DATELOR (CLASE)

Aplicația folosește următoarele clase principale pentru modelarea datelor:

* Elev

  * string nume
  * string prenume
  * int id
* Notă

  * int valoare
  * string materie
  * int id_elev
* Absență

  * string materie
  * string dată
  * bool motivată
  * int id_elev
* Catalog

  * vector<Elev> elevi
  * vector<Nota> note
  * vector<Absenta> absente

II. PERSISTENȚA DATELOR (FIȘIERE)

Datele sunt stocate în următoarele fișiere text:

1. elevi.txt

   * Stochează detalii despre elevi.
   * Formatul unei înregistrări: <id> <nume> <prenume>

2. note.txt

   * Stochează detaliile notelor elevilor.
   * Formatul unei înregistrări: <id_elev> <materie> <valoare>

3. absente.txt

   * Reține absențele elevilor.
   * Formatul unei înregistrări: <id_elev> <materie> <data> <motivata>

III. INTERFAȚA LINIE DE COMANDĂ (CLI)

Interacționarea cu aplicația se face prin intermediul a două executabile:

Executabil: gestionare_note_absente.exe

* Afișare catalog elev:
  ./gestionare_note_absente.exe afisare_catalog_elev
* Adăugare notă:
  ./gestionare_note_absente.exe adaugare_nota <id_elev> <materie> <valoare>
* Ștergere notă:
  ./gestionare_note_absente.exe stergere_nota <id_elev> <materie> <valoare>
* Adăugare absență:
  ./gestionare_note_absente.exe adaugare_absenta <id_elev> <materie> <data>
* Ștergere absență:
  ./gestionare_note_absente.exe stergere_absenta <id_elev> <materie> <data>

Executabil: gestionare_elevi.exe

* Afișare catalog complet:
  ./gestionare_elevi.exe afisare_catalog
* Adăugare elev:
  ./gestionare_elevi.exe adaugare_elev <id> <nume> <prenume>
* Ștergere elev (folosind ID-ul):
  ./gestionare_elevi.exe stergere_elev <id>
* Motivare absență:
  ./gestionare_elevi.exe motivare_absenta <id_elev> <materie> <data>

#ifndef PERSONA_H
#define PERSONA_H
#include <string>
#include <iostream>

/** class Persona
    @brief Classe custom

    Classe custom che rappresenta una persona

*/

class Persona{
public:
  Persona() : _codiceFiscale(""), _nome(""), _cognome("") { }

  Persona(std::string codiceFiscale, std::string nome, std::string cognome) :
   _codiceFiscale(codiceFiscale), _nome(nome), _cognome(cognome){ }

  std::string codiceFiscale() const {
    return _codiceFiscale;
  }

  std::string nome() const {
    return _nome;
  }

  std::string cognome() const {
    return _cognome;
  }

  friend std::ostream &operator<<(std::ostream& os, const Persona &p) {

    os << p._codiceFiscale;

    return os;
  }

protected:
  std::string _codiceFiscale;
  std::string _nome;
  std::string _cognome;
};




#endif

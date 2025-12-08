#include "StableCoin.hpp"
#include <cstdlib>

StableCoin::StableCoin(const std::string& nume,
                       const std::string& simbol,
                       double pret,
                       double volatilitate,
                       const std::string& emitent,
                       const std::string& moneda_legatura,
                       double raport_colateralizare)
    : Moneda(nume, simbol, pret, volatilitate),
      emitent(emitent),
      moneda_legatura(moneda_legatura),
      raport_colateralizare(raport_colateralizare) {}

void StableCoin::actualizarePret() {
    double zgomot_mic = ((rand() % 201) - 100) / 10000.0;
    pret = 1.0 + zgomot_mic * (volatilitate / 10.0);
}

void StableCoin::afisare(std::ostream& os) const {
    os << "[StableCoin] ";
    Moneda::afisare(os);
    os << " | Legat: " << moneda_legatura
       << " | Colateral: " << raport_colateralizare << "%"
       << " | Emitent: " << emitent;
}
#include "Monede/MonedaStabila.hpp"
#include <cstdlib>
#include <memory>

MonedaStabila::MonedaStabila(const std::string& nume,
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

void MonedaStabila::actualizare_pret() {
    double zgomot = ((rand() % 201) - 100) / 10000.0;
    pret = 1.0 + zgomot * (volatilitate / 10.0);
    
    // Menține stabilitate
    if (pret < 0.98) pret = 0.98;
    if (pret > 1.02) pret = 1.02;
}

void MonedaStabila::afisare(std::ostream& os) const {
    os << "[STABILĂ] ";
    Moneda::afisare(os);
    os << " | Legată la: " << moneda_legatura
       << " | Colateral: " << raport_colateralizare << "%"
       << " | Emitent: " << emitent;
}

std::unique_ptr<Moneda> MonedaStabila::cloneaza() const {
    return std::make_unique<MonedaStabila>(*this);
}
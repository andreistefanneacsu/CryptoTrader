#include "Monede/MonedaDeBaza.hpp"
#include <cstdlib>
#include <memory>

MonedaDeBaza::MonedaDeBaza(const std::string& nume,
                           const std::string& simbol,
                           double pret,
                           double volatilitate,
                           double capital_piata,
                           double factor_incredere)
    : Moneda(nume, simbol, pret, volatilitate),
      capital_piata(capital_piata),
      factor_incredere(factor_incredere) {}

void MonedaDeBaza::actualizare_pret() {
    double aleatoriu = ((rand() % 2001) - 1000) / 1000.0;
    pret *= 1 + aleatoriu * (volatilitate / 100.0) * (1 - 0.5 * factor_incredere);

    if (pret < 0.01) pret = 0.01;
}

void MonedaDeBaza::afisare(std::ostream& os) const {
    os << "[BAZĂ] ";
    Moneda::afisare(os);
    os << " | Capital Piață: " << capital_piata << "B | Încredere: " 
       << factor_incredere;
}

std::unique_ptr<Moneda> MonedaDeBaza::cloneaza() const {
    return std::make_unique<MonedaDeBaza>(*this);
}
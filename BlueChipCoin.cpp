#include "BlueChipCoin.hpp"
#include <cstdlib>

BlueChipCoin::BlueChipCoin(const std::string& nume,
                           const std::string& simbol,
                           double pret,
                           double volatilitate,
                           double capital_piata,
                           double factor_incredere)
    : Moneda(nume, simbol, pret, volatilitate),
      capital_piata(capital_piata),
      factor_incredere(factor_incredere) {}

void BlueChipCoin::actualizarePret() {
    double aleatoriu = ((rand() % 2001) - 1000) / 1000.0;
    pret *= 1 + aleatoriu * (volatilitate / 100.0) * (1 - 0.5 * factor_incredere);
}

void BlueChipCoin::afisare(std::ostream& os) const {
    os << "[BlueChip] ";
    Moneda::afisare(os);
    os << " | CapitalPiata: " << capital_piata << "B | Incredere: " << factor_incredere;
}
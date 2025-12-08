#include "MemeCoin.hpp"
#include <cstdlib>

MemeCoin::MemeCoin(const std::string& nume,
                   const std::string& simbol,
                   double pret,
                   double volatilitate,
                   double factor_meme,
                   double nivel_hype,
                   double scor_comunitate)
    : Moneda(nume, simbol, pret, volatilitate),
      factor_meme(factor_meme),
      nivel_hype(nivel_hype),
      scor_comunitate(scor_comunitate) {}

void MemeCoin::actualizarePret() {
    double aleatoriu = ((rand() % 2001) - 1000) / 1000.0;
    double impuls_hype = 1 + nivel_hype * 0.5;
    double impuls_comunitate = 1 + scor_comunitate * 0.3;
    pret *= 1 + aleatoriu * (volatilitate / 100.0) * (factor_meme / 10.0) * impuls_hype * impuls_comunitate;
}

void MemeCoin::afisare(std::ostream& os) const {
    os << "[MemeCoin] ";
    Moneda::afisare(os);
    os << " | FactorMeme: " << factor_meme
       << " | Hype: " << nivel_hype
       << " | Comunitate: " << scor_comunitate;
}
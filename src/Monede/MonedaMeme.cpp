#include "Monede/MonedaMeme.hpp"
#include <cstdlib>
#include <memory>

MonedaMeme::MonedaMeme(const std::string& nume,
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

void MonedaMeme::actualizare_pret() {
    double aleatoriu = ((rand() % 2001) - 1000) / 1000.0;
    double impuls_hype = 1 + nivel_hype * 0.5;
    double impuls_comunitate = 1 + scor_comunitate * 0.3;
    pret *= 1 + aleatoriu * (volatilitate / 100.0) * 
           (factor_meme / 10.0) * impuls_hype * impuls_comunitate;
    
    if (pret < 0.0001) pret = 0.0001;
}

void MonedaMeme::afisare(std::ostream& os) const {
    os << "[MEME] ";
    Moneda::afisare(os);
    os << " | Factor Meme: " << factor_meme
       << " | Hype: " << nivel_hype
       << " | Comunitate: " << scor_comunitate;
}

std::unique_ptr<Moneda> MonedaMeme::cloneaza() const {
    return std::make_unique<MonedaMeme>(*this);
}
#include "Monede/MonedaFiat.hpp"
#include <cstdlib>
#include <cmath>
#include <memory>

MonedaFiat::MonedaFiat(const std::string& nume,
                       const std::string& simbol,
                       double pret,
                       double volatilitate,
                       const std::string& tara_emitenta,
                       const std::string& cod_iso,
                       double rata_dobanda,
                       double rata_inflatie)
    : Moneda(nume, simbol, pret, volatilitate),
      tara_emitenta(tara_emitenta),
      cod_iso(cod_iso),
      rata_dobanda(rata_dobanda),
      rata_inflatie(rata_inflatie) {}

double MonedaFiat::calculeaza_putere_cumparare() const {
    return 1.0 / (1.0 + rata_inflatie / 100.0);
}

void MonedaFiat::actualizare_pret() {
    double variatie_inflatie = (rand() % 201 - 100) / 10000.0;
    double variatie_dobanda = (rand() % 101 - 50) / 10000.0;
    
    double modificare = (rata_dobanda - rata_inflatie) / 100.0 + 
                       variatie_inflatie + variatie_dobanda;
    
    double zgomot = (rand() % 41 - 20) / 10000.0;
    
    pret *= (1 + modificare + zgomot);
    
    if (std::abs(pret - 1.0) > 0.1) {
        pret = 1.0 + (pret - 1.0) * 0.5;
    }
}

void MonedaFiat::afisare(std::ostream& os) const {
    os << "[FIAT] ";
    Moneda::afisare(os);
    os << " | Țară: " << tara_emitenta
       << " | ISO: " << cod_iso
       << " | Dobândă: " << rata_dobanda << "%"
       << " | Inflație: " << rata_inflatie << "%"
       << " | Putere cumpărare: " << calculeaza_putere_cumparare();
}

std::unique_ptr<Moneda> MonedaFiat::cloneaza() const {
    return std::make_unique<MonedaFiat>(*this);
}
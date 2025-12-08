#include "FiatCoin.hpp"
#include <cstdlib>
#include <cmath>

FiatCoin::FiatCoin(const std::string& nume,
                   const std::string& simbol,
                   double pret,
                   double volatilitate,
                   const std::string& tara_emitenta,
                   const std::string& cod_iso,
                   double rata_dobanda,
                   double inflatie)
    : Moneda(nume, simbol, pret, volatilitate),
      tara_emitenta(tara_emitenta),
      cod_iso(cod_iso),
      rata_dobanda(rata_dobanda),
      inflatie(inflatie) {}

double FiatCoin::calculeazaPutereCumparare() const {
    return 1.0 / (1.0 + inflatie / 100.0);
}

void FiatCoin::actualizarePret() {
    double variatie_inflatie = (rand() % 201 - 100) / 10000.0;
    double variatie_dobanda = (rand() % 101 - 50) / 10000.0;
    
    double modificare = (rata_dobanda - inflatie) / 100.0 + variatie_inflatie + variatie_dobanda;
    
    double zgomot = (rand() % 41 - 20) / 10000.0;
    
    pret *= (1 + modificare + zgomot);
    
    if (std::abs(pret - 1.0) > 0.1) {
        pret = 1.0 + (pret - 1.0) * 0.5;
    }
}

void FiatCoin::afisare(std::ostream& os) const {
    os << "[Fiat] ";
    Moneda::afisare(os);
    os << " | Tara: " << tara_emitenta
       << " | ISO: " << cod_iso
       << " | Dobanda: " << rata_dobanda << "%"
       << " | Inflatie: " << inflatie << "%"
       << " | Putere cumparare: " << calculeazaPutereCumparare();
}
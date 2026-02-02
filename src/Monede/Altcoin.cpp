#include "Monede/Altcoin.hpp"
#include <cstdlib>
#include <cmath>
#include <memory>

Altcoin::Altcoin(const std::string& nume,
                 const std::string& simbol,
                 double pret,
                 double volatilitate,
                 double scalabilitate,
                 double interoperabilitate,
                 double scor_tehnologie)
    : Moneda(nume, simbol, pret, volatilitate),
      scalabilitate(scalabilitate),
      interoperabilitate(interoperabilitate),
      scor_tehnologie(scor_tehnologie) {}

double Altcoin::calculeaza_risc() const {
    return volatilitate * (1.0 - scor_tehnologie / 100.0);
}

double Altcoin::calculeaza_potential() const {
    return (scalabilitate + interoperabilitate + scor_tehnologie / 100.0) / 3.0;
}

void Altcoin::actualizare_pret() {
    double factor_scalare = 1.0 + (scalabilitate - 0.5) * 0.1;
    double factor_interop = 1.0 + (interoperabilitate - 0.5) * 0.05;
    double factor_tehnologie = 1.0 + (scor_tehnologie - 50) / 1000.0;
    
    double aleatoriu = ((rand() % 2001) - 1000) / 1000.0;
    double modificare = aleatoriu * (volatilitate / 100.0) * 
                       factor_scalare * factor_interop * factor_tehnologie;
    
    pret *= (1 + modificare);
    
    if (pret < 0.01) pret = 0.01;
}

void Altcoin::afisare(std::ostream& os) const {
    os << "[ALTCOIN] ";
    Moneda::afisare(os);
    os << " | Scalabilitate: " << scalabilitate
       << " | Interoperabilitate: " << interoperabilitate
       << " | Scor tehnologie: " << scor_tehnologie
       << "% | Risc: " << calculeaza_risc()
       << " | Potențial: " << calculeaza_potential();
}

std::unique_ptr<Moneda> Altcoin::cloneaza() const {
    return std::make_unique<Altcoin>(*this);
}
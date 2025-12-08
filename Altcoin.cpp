#include "Altcoin.hpp"
#include <cstdlib>
#include <cmath>
#include <utility>

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

Altcoin::Altcoin(const Altcoin& other)
    : Moneda(other),
      scalabilitate(other.scalabilitate),
      interoperabilitate(other.interoperabilitate),
      scor_tehnologie(other.scor_tehnologie) {}

Altcoin& Altcoin::operator=(Altcoin other) {
    swap(other);
    return *this;
}

void Altcoin::swap(Altcoin& other) noexcept {
    Moneda::swap(other);
    using std::swap;
    swap(scalabilitate, other.scalabilitate);
    swap(interoperabilitate, other.interoperabilitate);
    swap(scor_tehnologie, other.scor_tehnologie);
}

void Altcoin::actualizarePret() {
    double factor_scalare = 1.0 + (scalabilitate - 0.5) * 0.1;
    double factor_interop = 1.0 + (interoperabilitate - 0.5) * 0.05;
    double factor_tehnologie = 1.0 + (scor_tehnologie - 50) / 1000.0;
    
    double aleatoriu = ((rand() % 2001) - 1000) / 1000.0;
    double modificare = aleatoriu * (volatilitate / 100.0) * factor_scalare * factor_interop * factor_tehnologie;
    
    pret *= (1 + modificare);
    
    if (pret < 0.01) pret = 0.01;
}

void Altcoin::afisare(std::ostream& os) const {
    os << "[Altcoin] ";
    Moneda::afisare(os);
    os << " | Scalabilitate: " << scalabilitate
       << " | Interoperabilitate: " << interoperabilitate
       << " | Scor tehnologie: " << scor_tehnologie
       << "% | Risc: " << calculeazaRisc()
       << " | Potential: " << calculeazaPotential();
}

double Altcoin::calculeazaRisc() const {
    return volatilitate * (1.0 - scor_tehnologie / 100.0);
}

double Altcoin::calculeazaPotential() const {
    return (scalabilitate + interoperabilitate + scor_tehnologie / 100.0) / 3.0;
}
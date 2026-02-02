#include "Monede/Moneda.hpp"
#include <utility>
#include <algorithm>

Moneda::Moneda()
    : nume(""), simbol(""), pret(0.0), volatilitate(0.0) {}

Moneda::Moneda(const std::string& nume, const std::string& simbol, 
               double pret, double volatilitate)
    : nume(nume), simbol(simbol), pret(pret), volatilitate(volatilitate) {}

Moneda::Moneda(const Moneda& alta)
    : nume(alta.nume), simbol(alta.simbol), pret(alta.pret), 
      volatilitate(alta.volatilitate) {}

void Moneda::afisare(std::ostream& os) const {
    os << simbol << " (" << nume << ") - " << pret << " RON - Vol: " 
       << volatilitate << "%";
}

void interschimba(Moneda& x, Moneda& y) noexcept {
    using std::swap;
    swap(x.nume, y.nume);
    swap(x.simbol, y.simbol);
    swap(x.pret, y.pret);
    swap(x.volatilitate, y.volatilitate);
}

Moneda& Moneda::operator=(const Moneda& alta) {
    if (&alta!=this) {
        nume = alta.nume;
        simbol = alta.simbol;
        pret = alta.pret;
        volatilitate = alta.volatilitate;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Moneda& moneda) {
    moneda.afisare(os);
    return os;
}
#include "Moneda.hpp"
#include <utility>

Moneda::Moneda()
    : nume(""), simbol(""), pret(0), volatilitate(0) {}

Moneda::Moneda(const std::string &nume, const std::string &simbol, double pret, double volatilitate)
    : nume(nume), simbol(simbol), pret(pret), volatilitate(volatilitate) {}

Moneda::Moneda(const Moneda &m)
    : nume(m.nume), simbol(m.simbol), pret(m.pret), volatilitate(m.volatilitate) {}

void Moneda::afisare(std::ostream& os) const {
    os << simbol << " (" << nume << ") - " << pret << " RON";
}

void Moneda::swap(Moneda& other) noexcept {
    using std::swap;
    swap(nume, other.nume);
    swap(simbol, other.simbol);
    swap(pret, other.pret);
    swap(volatilitate, other.volatilitate);
}

std::ostream& operator<<(std::ostream& os, const Moneda& moneda) {
    moneda.afisare(os);
    return os;
}
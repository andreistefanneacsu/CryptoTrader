#include "Tranzactie.hpp"

Tranzactie::Tranzactie()
    : moneda(nullptr), tip(""), pret(0), cantitate(0), status(SUCCES) {}

Tranzactie::Tranzactie(Moneda* moneda, const std::string &tip,
                       double pret, double cantitate, Status status)
    : moneda(moneda), tip(tip), pret(pret), cantitate(cantitate), status(status) {}

Tranzactie::Tranzactie(const Tranzactie &t)
    : moneda(t.moneda), tip(t.tip), pret(t.pret), cantitate(t.cantitate), status(t.status) {}

std::ostream& operator<<(std::ostream& os, const Tranzactie& t) {
    os << "[" << t.get_tip() << "] " << t.moneda->get_simbol()
       << " " << t.cantitate << " status=" << t.status;
    return os;
}
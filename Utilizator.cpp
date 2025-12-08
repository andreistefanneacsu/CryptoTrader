#include "Utilizator.hpp"

Utilizator::Utilizator() {
    balanteFiat = 0.0;
}

Utilizator::Utilizator(const std::string& nume)
    : nume(nume) {
    balanteFiat = 0.0;
}

Utilizator::Utilizator(const std::string& nume, const Portofel& p)
    : nume(nume), portofel(p) {
    balanteFiat = 0.0;
}

bool Utilizator::cumpara(Moneda* moneda, const double suma) {
    if (balanteFiat < suma) {
        return false;
    }

    double cantitate = suma / moneda->get_pret();
    bool succes = portofel.cumparaMoneda(moneda, cantitate);
    if (succes) {
        balanteFiat -= suma;
    }

    return succes;
}

/*
bool Utilizator::vinde(Moneda* moneda, double cantitate) {
    const auto& monede = portofel.get_monede();
    bool areSuficient = false;
    for (const auto& pereche : monede) {
        if (pereche.first->get_simbol() == moneda->get_simbol() && pereche.second.first >= cantitate) {
            areSuficient = true;
            break;
        }
    }

    if (!areSuficient) {
        return false;
    }

    bool succes = portofel.vindeMoneda(moneda, cantitate);
    if (succes) {
        double sumaPrimita = cantitate * moneda->get_pret();
        balanteFiat += sumaPrimita;
    }

    return succes;
}
*/

void Utilizator::topUpFiat(double suma) {
        balanteFiat += suma;
}

void Utilizator::determinareValoarePortofel() {
    std::cout << portofel.get_ValoarePortofel() << "\n";
}

void Utilizator::determinareProfitPosibil() {
    auto p = portofel.estimareProfitTotal();
    std::cout << p.first << " " << p.second << "\n";
}

void Utilizator::determinareProfitTotal() {
    std::cout << portofel.get_ProfitTotal() << "\n";
}

std::ostream& operator<<(std::ostream& os, const Utilizator& u) {
    os << u.nume << ":\n" << u.portofel << u.balanteFiat << "\n";
    return os;
}

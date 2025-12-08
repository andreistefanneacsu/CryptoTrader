#include "Piata.hpp"

Piata::Piata() = default;

Piata::Piata(const std::vector<Moneda*>& monede_init)
        : monede(monede_init) {}

Piata::~Piata() {
    for (auto* moneda : monede)
        delete moneda;
    monede.clear();
}

const Moneda* Piata::find_moneda(const std::string& simbol) const {
    for (auto* moneda : monede) {
        if (moneda->get_simbol() == simbol) {
            return moneda;
        }
    }
    return nullptr;
}

Moneda* Piata::find_moneda(const std::string& simbol) {
    for (auto* moneda : monede) {
        if (moneda->get_simbol() == simbol) {
            return moneda;
        }
    }
    return nullptr;
}

void Piata::actualizarePiata() const {
    for (auto* moneda : monede) {
        moneda->actualizarePret();
    }
}

std::ostream& operator<<(std::ostream& os, const Piata& piata) {
    os << "Piata curenta:\n";
    for (const auto* moneda : piata.monede) {
        os << *moneda << "\n";
    }
    return os;
}
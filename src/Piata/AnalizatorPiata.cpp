#include "Piata/AnalizatorPiata.hpp"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

int AnalizatorPiata::numar_analizoare_active = 0;

AnalizatorPiata::AnalizatorPiata()
    : monede_sortate_volatilitate(
        [](const std::shared_ptr<Moneda>& a,
           const std::shared_ptr<Moneda>& b) {
            return a->obtine_volatilitate() < b->obtine_volatilitate();
        }) {
    ++numar_analizoare_active;
}

AnalizatorPiata::AnalizatorPiata(
    const std::vector<std::shared_ptr<Moneda>>& monede)
    : monede(monede),
      monede_sortate_volatilitate(
        [](const std::shared_ptr<Moneda>& a,
           const std::shared_ptr<Moneda>& b) {
            return a->obtine_volatilitate() < b->obtine_volatilitate();
        }) {
    ++numar_analizoare_active;

    for (const auto& moneda : monede) {
        monede_sortate_volatilitate.adauga(moneda);
    }
}

AnalizatorPiata::AnalizatorPiata(const AnalizatorPiata& alta)
    : monede(alta.monede),
      monede_sortate_volatilitate(alta.monede_sortate_volatilitate) {
    ++numar_analizoare_active;
}

AnalizatorPiata::~AnalizatorPiata() {
    --numar_analizoare_active;
}

void AnalizatorPiata::interschimba(AnalizatorPiata& alta) noexcept {
    using std::swap;
    swap(monede, alta.monede);
    swap(monede_sortate_volatilitate, alta.monede_sortate_volatilitate);
}

AnalizatorPiata& AnalizatorPiata::operator=(AnalizatorPiata alta) {
    interschimba(alta);
    return *this;
}

double AnalizatorPiata::calculeaza_volatilitate_medie() const {
    if (monede.empty()) {
        return 0.0;
    }

    double suma = std::accumulate(
        monede.begin(), monede.end(), 0.0,
        [](double acc, const std::shared_ptr<Moneda>& m) {
            return acc + m->obtine_volatilitate();
        });

    return suma / static_cast<double>(monede.size());
}

double AnalizatorPiata::calculeaza_volatilitate_maxima() const {
    if (monede.empty()) {
        return 0.0;
    }

    auto it = std::max_element(
        monede.begin(), monede.end(),
        [](const auto& a, const auto& b) {
            return a->obtine_volatilitate() < b->obtine_volatilitate();
        });

    return (*it)->obtine_volatilitate();
}

double AnalizatorPiata::calculeaza_volatilitate_minima() const {
    if (monede.empty()) {
        return 0.0;
    }

    auto it = std::min_element(
        monede.begin(), monede.end(),
        [](const auto& a, const auto& b) {
            return a->obtine_volatilitate() < b->obtine_volatilitate();
        });

    return (*it)->obtine_volatilitate();
}

std::vector<std::shared_ptr<Moneda>>
AnalizatorPiata::filtreaza_dupa_volatilitate(double min, double max) const {
    std::vector<std::shared_ptr<Moneda>> rezultat;

    std::copy_if(
        monede.begin(), monede.end(),
        std::back_inserter(rezultat),
        [min, max](const std::shared_ptr<Moneda>& m) {
            double v = m->obtine_volatilitate();
            return v >= min && v <= max;
        });

    return rezultat;
}

void AnalizatorPiata::afiseaza_statistici() const {
    std::cout << "\n=== STATISTICI PIATA ===\n";
    std::cout << "Număr monede analizate: " << monede.size() << "\n";
    std::cout << "Volatilitate medie: "
              << calculeaza_volatilitate_medie() << "%\n";
    std::cout << "Volatilitate maximă: "
              << calculeaza_volatilitate_maxima() << "%\n";
    std::cout << "Volatilitate minimă: "
              << calculeaza_volatilitate_minima() << "%\n";

    auto monede_volatile = filtreaza_dupa_volatilitate(10.0, 100.0);
    std::cout << "Monede volatile (>10%): "
              << monede_volatile.size() << "\n";

    auto stabile = filtreaza_dupa_volatilitate(0.0, 2.0);
    std::cout << "Monede stabile (<2%): "
              << stabile.size() << "\n";
}

void AnalizatorPiata::afiseaza_monede_volatile(double prag) const {
    auto volatile_list = filtreaza_dupa_volatilitate(prag, 100.0);

    std::cout << "\n=== MONEDE VOLATILE (> "
              << prag << "%) ===\n";

    std::cout << std::left
              << std::setw(10) << "Simbol"
              << std::setw(20) << "Nume"
              << std::setw(15) << "Volatilitate%"
              << std::setw(15) << "Preț (RON)\n";

    std::cout << std::string(60, '-') << "\n";

    for (const auto& moneda : volatile_list) {
        std::cout << std::left
                  << std::setw(10) << moneda->obtine_simbol()
                  << std::setw(20) << moneda->obtine_nume()
                  << std::setw(15) << std::fixed << std::setprecision(1)
                  << moneda->obtine_volatilitate()
                  << std::setw(15) << std::setprecision(2)
                  << moneda->obtine_pret()
                  << "\n";
    }
}

int AnalizatorPiata::obtine_numar_analizoare_active() {
    return numar_analizoare_active;
}

#include "AnalizatorPiata.hpp"
#include <algorithm>
#include <numeric>
#include <utility>

int AnalizatorPiata::numar_analizoare_active = 0;

AnalizatorPiata::AnalizatorPiata(const std::vector<std::shared_ptr<Moneda>>& monede)
    : monede(monede) {
    numar_analizoare_active++;
}

AnalizatorPiata::AnalizatorPiata(const AnalizatorPiata& other)
    : monede(other.monede) {
    numar_analizoare_active++;
}

AnalizatorPiata& AnalizatorPiata::operator=(AnalizatorPiata other) {
    swap(other);
    return *this;
}

AnalizatorPiata::~AnalizatorPiata() {
    numar_analizoare_active--;
}

void AnalizatorPiata::swap(AnalizatorPiata& other) noexcept {
    using std::swap;
    swap(monede, other.monede);
}

double AnalizatorPiata::calculeazaVolatilitateMedie() const {
    if (monede.empty()) return 0.0;

    double suma = std::accumulate(monede.begin(), monede.end(), 0.0,
        [](double acc, const std::shared_ptr<Moneda>& moneda) {
            return acc + moneda->get_volatilitate();
        });

    return suma / monede.size();
}

std::vector<std::shared_ptr<Moneda>> AnalizatorPiata::filtreazaMonedeDupaVolatilitate(double min, double max) const {
    std::vector<std::shared_ptr<Moneda>> rezultat;
    std::copy_if(monede.begin(), monede.end(), std::back_inserter(rezultat),
        [min, max](const std::shared_ptr<Moneda>& moneda) {
            double vol = moneda->get_volatilitate();
            return vol >= min && vol <= max;
        });
    return rezultat;
}

void AnalizatorPiata::actualizeazaToateMonedele() {
    for (auto& moneda : monede) {
        moneda->actualizarePret();
    }
}

void AnalizatorPiata::adaugaMoneda(std::shared_ptr<Moneda> moneda) {
    monede.push_back(moneda);
}
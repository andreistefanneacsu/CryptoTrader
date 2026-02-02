#include "Utilizator/Portofel.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

double Portofel::calculeaza_pret_mediu(double val_totala, double cant_totala) const {
    return (cant_totala == 0) ? 0 : val_totala / cant_totala;
}

Portofel::PozitieMoneda* Portofel::gaseste_pozitie(const std::string& simbol) {
    for (auto& pozitie : pozitii) {
        if (pozitie.simbol_cache == simbol) {
            return &pozitie;
        }
    }
    return nullptr;
}

Portofel::Portofel() : profit_total_realizat(0.0) {}

Portofel::Portofel(const Portofel& alta)
    : profit_total_realizat(alta.profit_total_realizat) {
    for (const auto& pozitie : alta.pozitii) {
        pozitii.push_back(pozitie);
    }
}

void Portofel::interschimba(Portofel& alta) noexcept {
    using std::swap;
    swap(pozitii, alta.pozitii);
    swap(tranzactii, alta.tranzactii);
    swap(profit_total_realizat, alta.profit_total_realizat);
}

Portofel& Portofel::operator=(Portofel alta) {
    interschimba(alta);
    return *this;
}

bool Portofel::adauga_moneda(const Moneda* moneda, double cantitate, double pret_achizitie) {
    if (!moneda || cantitate <= 0) return false;

    std::string simbol = moneda->obtine_simbol();
    PozitieMoneda* pozitie = gaseste_pozitie(simbol);

    if (pozitie) {
        double valoare_veche = pozitie->cantitate * pozitie->pret_mediu_achizitie;
        double valoare_noua = cantitate * pret_achizitie;
        pozitie->pret_mediu_achizitie = calculeaza_pret_mediu(valoare_veche + valoare_noua,
                                                             pozitie->cantitate + cantitate);
        pozitie->cantitate += cantitate;
    } else {
        pozitii.emplace_back(moneda, cantitate, pret_achizitie);
    }

    /*
    tranzactii.push_back(std::make_unique<Tranzactie>(
        moneda, "CUMPARARE", pret_achizitie, cantitate, StatusTranzactie::SUCCES
    ));
    */

    return true;
}

bool Portofel::vinde_moneda(const std::string& simbol, double cantitate, double pret_vanzare) {
    PozitieMoneda* pozitie = gaseste_pozitie(simbol);

    if (!pozitie || pozitie->cantitate < cantitate) return false;

    double profit = (pret_vanzare - pozitie->pret_mediu_achizitie) * cantitate;
    profit_total_realizat += profit;

    pozitie->cantitate -= cantitate;

    if (pozitie->cantitate < 0.000001) {
        auto it = std::remove_if(pozitii.begin(), pozitii.end(),
            [&](const PozitieMoneda& p) { return &p == pozitie; });
        pozitii.erase(it, pozitii.end());
    }

    /*
    const Moneda* moneda_ptr = pozitie ? pozitie->moneda : nullptr;
    tranzactii.push_back(std::make_unique<Tranzactie>(
        moneda_ptr, "VANZARE", pret_vanzare, cantitate, StatusTranzactie::SUCCES
    ));
    */

    return true;
}

double Portofel::obtine_valoare_portofel() const {
    double total = 0.0;
    for (const auto& pozitie : pozitii) {
        if (pozitie.moneda) {
            total += pozitie.cantitate * pozitie.moneda->obtine_pret();
        }
    }
    return total;
}

double Portofel::obtine_profit_total() const {
    double profit_nerealizat = 0.0;
    for (const auto& pozitie : pozitii) {
        if (pozitie.moneda) {
            profit_nerealizat += (pozitie.moneda->obtine_pret() - pozitie.pret_mediu_achizitie) * pozitie.cantitate;
        }
    }
    return profit_total_realizat + profit_nerealizat;
}

void Portofel::afiseaza_portofel(std::ostream& os) const {
    os << "\n=== PORTOFEL ===\n";
    os << std::left << std::setw(10) << "Simbol"
              << std::setw(15) << "Cantitate"
              << std::setw(20) << "Pret Mediu"
              << std::setw(20) << "Pret Curent"
              << std::setw(15) << "Profit" << "\n";
    os << std::string(80, '-') << "\n";

    for (const auto& pozitie : pozitii) {
        double pret_curent = pozitie.moneda ? pozitie.moneda->obtine_pret() : 0.0;
        double profit = (pret_curent - pozitie.pret_mediu_achizitie) * pozitie.cantitate;

        os << std::left << std::setw(10) << pozitie.simbol_cache
                  << std::setw(15) << std::fixed << std::setprecision(4) << pozitie.cantitate
                  << std::setw(20) << std::setprecision(2) << pozitie.pret_mediu_achizitie
                  << std::setw(20) << pret_curent
                  << std::setw(15) << (profit >= 0 ? "+" : "") << profit << "\n";
    }
    os << "\nValoare totala: " << obtine_valoare_portofel() << " RON\n";
}

void Portofel::afiseaza_tranzactii(std::ostream& os) const {
    os << "\n=== ISTORIC TRANZACTII ===\n";
    if (tranzactii.empty()) {
        os << "Nu exista tranzactii (istoric dezactivat).\n";
        return;
    }
    for (auto it = tranzactii.rbegin(); it != tranzactii.rend(); ++it) {
        os << **it << "\n";
    }
}

void Portofel::afiseaza_statistici(std::ostream& os) const {
    os << "\n=== STATISTICI PORTOFEL ===\n";
    os << "Valoare curenta: " << std::fixed << std::setprecision(2)
              << obtine_valoare_portofel() << " RON\n";
    os << "Profit realizat (vanzari): " << profit_total_realizat << " RON\n";
    os << "Profit total (incl. nerealizat): " << obtine_profit_total() << " RON\n";
}

/*
double Portofel::obtine_cantitate_moneda(const std::string& simbol) const {
    for (const auto& pozitie : pozitii) {
        if (pozitie.simbol_cache == simbol) {
            return pozitie.cantitate;
        }
    }
    return 0.0;
}

void Portofel::adauga_pozitie_incarcata(const std::string& simbol, double cantitate, double pret_mediu) {
    pozitii.emplace_back(simbol, cantitate, pret_mediu);
}

void Portofel::adauga_tranzactie_incarcata(std::unique_ptr<Tranzactie> t) {
    tranzactii.push_back(std::move(t));
}

void Portofel::reconecteaza_la_piata(const std::vector<std::unique_ptr<Moneda>>& monede_piata) {
    for (auto& pozitie : pozitii) {
        for (const auto& moneda : monede_piata) {
            if (moneda->obtine_simbol() == pozitie.simbol_cache) {
                pozitie.moneda = moneda.get();
                break;
            }
        }
    }
}
*/

std::ostream& operator<<(std::ostream& os, const Portofel& portofel) {
    portofel.afiseaza_portofel(os);
    return os;
}
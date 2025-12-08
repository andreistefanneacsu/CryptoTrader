#include "Portofel.hpp"

inline double calc_pretMediu(double val_totala, double cant_totala) {
    if (cant_totala == 0) return 0;
    return val_totala / cant_totala;
}

Portofel::Portofel() = default;

Portofel::Portofel(const std::vector<Tranzactie*>& tranzactii_init)
        : tranzactii(tranzactii_init)
{
    for (const auto* tranzactie : tranzactii_init) {
        if (tranzactie->get_status() == SUCCES) {
            bool moneda_gasita = false;

            for (auto& moneda : monede) {
                if (moneda.first->get_simbol() == tranzactie->get_moneda()->get_simbol()) {

                    if (tranzactie->get_tip() == "CUMPARARE") {
                        double val_tot = moneda.second.first * moneda.second.second +
                                         tranzactie->get_cantitate() * tranzactie->get_pret();
                        double cant_tot = moneda.second.first + tranzactie->get_cantitate();

                        moneda.second.second = calc_pretMediu(val_tot, cant_tot);
                        moneda.second.first = cant_tot;
                    }
                    else if (tranzactie->get_tip() == "VANZARE") {
                        moneda.second.first -= tranzactie->get_cantitate();
                        if (moneda.second.first == 0)
                            moneda.second.second = 0;
                    }

                    moneda_gasita = true;
                    break;
                }
            }

            if (!moneda_gasita && tranzactie->get_tip() == "CUMPARARE") {
                monede.push_back({
                    tranzactie->get_moneda(),
                    { tranzactie->get_cantitate(), tranzactie->get_pret() }
                });
            }
        }
    }
}

Portofel::~Portofel() {
    for (auto* t : tranzactii)
        delete t;

    tranzactii.clear();
    monede.clear();
}

bool Portofel::cumparaMoneda(Moneda* m, double cantitate) {
    for (auto& moneda : monede) {
        if (moneda.first->get_simbol() == m->get_simbol()) {

            moneda.second.second = calc_pretMediu(
                moneda.second.first * moneda.second.second + cantitate * m->get_pret(),
                moneda.second.first + cantitate
            );

            moneda.second.first += cantitate;

            tranzactii.push_back(
                new Tranzactie(m, "CUMPARARE", m->get_pret(), cantitate, SUCCES)
            );

            return true;
        }
    }

    monede.push_back({ m, { cantitate, m->get_pret() } });

    tranzactii.push_back(
        new Tranzactie(m, "CUMPARARE", m->get_pret(), cantitate, SUCCES)
    );

    return true;
}

bool Portofel::vindeMoneda(Moneda* m, double cantitate) {

    for (auto& moneda : monede) {
        if (moneda.first->get_simbol() == m->get_simbol()) {

            if (moneda.second.first > cantitate) {
                moneda.second.first -= cantitate;

                tranzactii.push_back(
                    new Tranzactie(m, "VANZARE", m->get_pret(), cantitate, SUCCES)
                );
                return true;
            }
            else if (moneda.second.first == cantitate) {
                moneda.second.first = 0;
                moneda.second.second = 0;

                tranzactii.push_back(
                    new Tranzactie(m, "VANZARE", m->get_pret(), cantitate, SUCCES)
                );
                return true;
            }
            else {
                tranzactii.push_back(
                    new Tranzactie(m, "VANZARE", m->get_pret(), cantitate, EROARE_FONDURI)
                );
                return false;
            }
        }
    }

    tranzactii.push_back(
        new Tranzactie(m, "VANZARE", m->get_pret(), cantitate, EROARE_MONEDA)
    );
    return false;
}

double Portofel::get_ValoarePortofel() const {
    double total = 0;
    for (const auto& moneda : monede) {
        total += moneda.first->get_pret() * moneda.second.first;
    }
    return total;
}

double Portofel::get_ProfitTotal() const {
    double profit = 0;
    for (const auto& moneda : monede) {
        profit += (moneda.first->get_pret() - moneda.second.second) * moneda.second.first;
    }
    return profit;
}

std::pair<double, double> Portofel::estimareProfitTotal() const {
    double minP = 0, maxP = 0;

    for (const auto& moneda : monede) {
        double pret = moneda.first->get_pret();
        double vola = moneda.first->get_volatilitate() / 100.0;
        double cant = moneda.second.first;
        double pret_mediu = moneda.second.second;

        minP += ((pret * (1 - vola)) - pret_mediu) * cant;
        maxP += ((pret * (1 + vola)) - pret_mediu) * cant;
    }

    return { minP, maxP };
}

std::ostream& operator<<(std::ostream& os, const Portofel& p) {
    os << "Monede:\n";
    for (const auto& moneda : p.monede) {
        os << *moneda.first << " "
           << moneda.second.first << " "
           << moneda.second.second << " RON\n";
    }

    os << "Tranzactii:\n";
    for (const auto* t : p.tranzactii) {
        os << *t << "\n";
    }

    return os;
}
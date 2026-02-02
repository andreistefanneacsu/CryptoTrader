#ifndef CRYPTOTRADER_PORTOFEL_HPP
#define CRYPTOTRADER_PORTOFEL_HPP

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <iostream>
#include "Monede/Moneda.hpp"
#include "Tranzactii/Tranzactie.hpp"

class Portofel {
public:
    struct PozitieMoneda {
        const Moneda* moneda;
        std::string simbol_cache;
        double cantitate;
        double pret_mediu_achizitie;

        PozitieMoneda(const Moneda* m, double c, double pma)
            : moneda(m), cantitate(c), pret_mediu_achizitie(pma) {
            if(m) simbol_cache = m->obtine_simbol();
        }

        PozitieMoneda(const std::string& simbol, double c, double pma)
            : moneda(nullptr), simbol_cache(simbol), cantitate(c), pret_mediu_achizitie(pma) {}
    };

private:
    std::vector<PozitieMoneda> pozitii;
    std::vector<std::unique_ptr<Tranzactie>> tranzactii;
    double profit_total_realizat;

    double calculeaza_pret_mediu(double val_totala, double cant_totala) const;
    PozitieMoneda* gaseste_pozitie(const std::string& simbol);

public:
    Portofel();
    Portofel(const Portofel& alta);
    Portofel& operator=(Portofel alta);
    ~Portofel() = default;

    void interschimba(Portofel& alta) noexcept;

    bool adauga_moneda(const Moneda* moneda, double cantitate, double pret_achizitie);
    bool vinde_moneda(const std::string& simbol, double cantitate, double pret_vanzare);

    /*
    void adauga_pozitie_incarcata(const std::string& simbol, double cantitate, double pret_mediu);
    void adauga_tranzactie_incarcata(std::unique_ptr<Tranzactie> t);
    void reconecteaza_la_piata(const std::vector<std::unique_ptr<Moneda>>& monede_piata);
    */

    const std::vector<PozitieMoneda>& obtine_pozitii() const { return pozitii; }

    // const std::vector<std::unique_ptr<Tranzactie>>& obtine_tranzactii() const { return tranzactii; }

    // double obtine_cantitate_moneda(const std::string& simbol) const;

    double obtine_valoare_portofel() const;
    double obtine_profit_total() const;
    double obtine_profit_realizat() const { return profit_total_realizat; }

    void afiseaza_portofel(std::ostream& os = std::cout) const;
    void afiseaza_tranzactii(std::ostream& os = std::cout) const;
    void afiseaza_statistici(std::ostream& os = std::cout) const;

    friend std::ostream& operator<<(std::ostream& os, const Portofel& portofel);
};

#endif
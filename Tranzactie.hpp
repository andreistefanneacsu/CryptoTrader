#ifndef CRYPTOTRADER__V2_TRANZACTIE_HPP
#define CRYPTOTRADER__V2_TRANZACTIE_HPP

#include <iostream>
#include <string>
#include "Moneda.hpp"

enum Status {
    SUCCES,
    EROARE_FONDURI,
    EROARE_MONEDA
};

class Tranzactie {
private:
    Moneda* moneda;
    std::string tip;
    double pret;
    double cantitate;
    Status status;

public:
    Tranzactie();
    Tranzactie(Moneda*, const std::string&, double, double, Status);
    Tranzactie(const Tranzactie&);
    ~Tranzactie() = default;

    Moneda* get_moneda() const { return moneda; }
    const std::string& get_tip() const { return tip; }
    double get_pret() const { return pret; }
    const double& get_cantitate() const { return cantitate; }
    Status get_status() const { return status; }

    friend std::ostream& operator<<(std::ostream& os, const Tranzactie& t);
};

#endif
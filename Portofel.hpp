#ifndef CRYPTOTRADER__V2_PORTOFEL_HPP
#define CRYPTOTRADER__V2_PORTOFEL_HPP

#include <iostream>
#include <vector>
#include "Moneda.hpp"
#include "Tranzactie.hpp"

class Portofel {
private:
    std::vector<std::pair<Moneda*, std::pair<double,double>>> monede;
    std::vector<Tranzactie*> tranzactii;

public:
    Portofel();
    explicit Portofel(const std::vector<Tranzactie*>&);
    ~Portofel();

    const std::vector<Tranzactie*>& get_tranzactii() const { return tranzactii; }
    const std::vector<std::pair<Moneda*, std::pair<double,double>>>& get_monede() const { return monede; }

    bool cumparaMoneda(Moneda*, double);
    bool vindeMoneda(Moneda*, double);

    double get_ValoarePortofel() const;
    double get_ProfitTotal() const;
    std::pair<double,double> estimareProfitTotal() const;

    friend std::ostream& operator<<(std::ostream&, const Portofel&);
};

#endif
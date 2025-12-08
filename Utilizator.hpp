#ifndef CRYPTOTRADER__V2_UTILIZATOR_HPP
#define CRYPTOTRADER__V2_UTILIZATOR_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include "Portofel.hpp"

class Utilizator {
private:
    std::string nume;
    Portofel portofel;
    double balanteFiat;

public:
    Utilizator();
    explicit Utilizator(const std::string&);
    Utilizator(const std::string&, const Portofel&);
    ~Utilizator() = default;

    const std::string& get_nume() const { return nume; }
    const Portofel& get_portofel() const { return portofel; }
    double get_balanta_fiat() const { return balanteFiat; }

    bool cumpara(Moneda*, double suma);
    bool vinde(Moneda*, double cantitate);
    void topUpFiat(double suma);

    void determinareValoarePortofel();
    void determinareProfitPosibil();
    void determinareProfitTotal();

    friend std::ostream& operator<<(std::ostream&, const Utilizator&);
};

#endif
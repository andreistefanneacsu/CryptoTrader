#ifndef CRYPTOTRADER__V2_PIATA_HPP
#define CRYPTOTRADER__V2_PIATA_HPP

#include <iostream>
#include <vector>
#include <random>
#include "Moneda.hpp"

class Piata {
private:
    std::vector<Moneda*> monede;

public:
    Piata();
    explicit Piata(const std::vector<Moneda*>&);
    Piata(const Piata&) = delete;
    Piata& operator=(const Piata&) = delete;

    ~Piata();

    const std::vector<Moneda*>& get_monede() const { return monede; }
    const Moneda* find_moneda(const std::string&) const;

    Moneda* find_moneda(const std::string&);

    void actualizarePiata() const;

    friend std::ostream& operator<<(std::ostream&, const Piata&);
};

#endif
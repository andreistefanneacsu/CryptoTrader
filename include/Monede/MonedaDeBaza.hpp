#ifndef CRYPTOTRADER_MONEDADEBAZA_HPP
#define CRYPTOTRADER_MONEDADEBAZA_HPP

#include "Moneda.hpp"

class MonedaDeBaza : public Moneda {
private:
    double capital_piata;
    double factor_incredere;

public:
    MonedaDeBaza(const std::string& nume,
                 const std::string& simbol,
                 double pret,
                 double volatilitate,
                 double capital_piata,
                 double factor_incredere);

    double obtine_capital_piata() const { return capital_piata; }
    double obtine_factor_incredere() const { return factor_incredere; }

    void actualizare_pret() override;
    void afisare(std::ostream& os) const override;
    std::unique_ptr<Moneda> cloneaza() const override;
};

#endif
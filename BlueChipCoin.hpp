#ifndef CRYPTOTRADER___BLUECHIPCOIN_HPP
#define CRYPTOTRADER___BLUECHIPCOIN_HPP

#include "Moneda.hpp"

class BlueChipCoin : public Moneda {
private:
    double capital_piata;
    double factor_incredere;

public:
    BlueChipCoin(const std::string& nume,
                 const std::string& simbol,
                 double pret,
                 double volatilitate,
                 double capital_piata,
                 double factor_incredere);

    double get_capital_piata() const { return capital_piata; }
    double get_factor_incredere() const { return factor_incredere; }

    void actualizarePret() override;
    void afisare(std::ostream& os) const override;
};

#endif
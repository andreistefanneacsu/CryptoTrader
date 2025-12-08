#ifndef CRYPTOTRADER___MEMECOIN_HPP
#define CRYPTOTRADER___MEMECOIN_HPP

#include "Moneda.hpp"

class MemeCoin : public Moneda {
private:
    double factor_meme;
    double nivel_hype;
    double scor_comunitate;

public:
    MemeCoin(const std::string& nume,
             const std::string& simbol,
             double pret,
             double volatilitate,
             double factor_meme,
             double nivel_hype,
             double scor_comunitate);

    double get_factor_meme() const { return factor_meme; }
    double get_nivel_hype() const { return nivel_hype; }
    double get_scor_comunitate() const { return scor_comunitate; }

    void actualizarePret() override;
    void afisare(std::ostream& os) const override;
};

#endif
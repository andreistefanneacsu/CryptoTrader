#ifndef CRYPTOTRADER_MONEDAMEME_HPP
#define CRYPTOTRADER_MONEDAMEME_HPP

#include "Moneda.hpp"

class MonedaMeme : public Moneda {
private:
    double factor_meme;
    double nivel_hype;
    double scor_comunitate;

public:
    MonedaMeme(const std::string& nume,
               const std::string& simbol,
               double pret,
               double volatilitate,
               double factor_meme,
               double nivel_hype,
               double scor_comunitate);

    double obtine_factor_meme() const { return factor_meme; }
    double obtine_nivel_hype() const { return nivel_hype; }
    double obtine_scor_comunitate() const { return scor_comunitate; }

    void actualizare_pret() override;
    void afisare(std::ostream& os) const override;
    std::unique_ptr<Moneda> cloneaza() const override;
};

#endif
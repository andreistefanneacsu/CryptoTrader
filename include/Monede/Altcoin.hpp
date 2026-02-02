#ifndef CRYPTOTRADER_ALTCOIN_HPP
#define CRYPTOTRADER_ALTCOIN_HPP

#include "Moneda.hpp"

class Altcoin : public Moneda {
private:
    double scalabilitate;
    double interoperabilitate;
    double scor_tehnologie;

public:
    Altcoin(const std::string& nume,
            const std::string& simbol,
            double pret,
            double volatilitate,
            double scalabilitate,
            double interoperabilitate,
            double scor_tehnologie);

    double obtine_scalabilitate() const { return scalabilitate; }
    double obtine_interoperabilitate() const { return interoperabilitate; }
    double obtine_scor_tehnologie() const { return scor_tehnologie; }

    double calculeaza_risc() const;
    double calculeaza_potential() const;

    void actualizare_pret() override;
    void afisare(std::ostream& os) const override;
    std::unique_ptr<Moneda> cloneaza() const override;
};

#endif
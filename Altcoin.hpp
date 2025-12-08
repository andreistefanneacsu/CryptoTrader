#ifndef CRYPTOTRADER___ALTCOIN_HPP
#define CRYPTOTRADER___ALTCOIN_HPP

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

    Altcoin(const Altcoin& other);
    Altcoin& operator=(Altcoin other);
    ~Altcoin() override = default;

    void swap(Altcoin& other) noexcept;

    double get_scalabilitate() const { return scalabilitate; }
    double get_interoperabilitate() const { return interoperabilitate; }
    double get_scor_tehnologie() const { return scor_tehnologie; }

    void actualizarePret() override;
    void afisare(std::ostream& os) const override;

    double calculeazaRisc() const;
    double calculeazaPotential() const;
};

#endif

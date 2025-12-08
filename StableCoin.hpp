#ifndef CRYPTOTRADER___STABLECOIN_HPP
#define CRYPTOTRADER___STABLECOIN_HPP

#include "Moneda.hpp"

class StableCoin : public Moneda {
private:
    std::string emitent;
    std::string moneda_legatura;
    double raport_colateralizare;

public:
    StableCoin(const std::string& nume,
               const std::string& simbol,
               double pret,
               double volatilitate,
               const std::string& emitent,
               const std::string& moneda_legatura,
               double raport_colateralizare);

    const std::string& get_emitent() const { return emitent; }
    const std::string& get_moneda_legatura() const { return moneda_legatura; }
    double get_raport_colateralizare() const { return raport_colateralizare; }

    void actualizarePret() override;
    void afisare(std::ostream& os) const override;
};

#endif
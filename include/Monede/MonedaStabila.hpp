#ifndef CRYPTOTRADER_MONEDASTABILA_HPP
#define CRYPTOTRADER_MONEDASTABILA_HPP

#include "Moneda.hpp"
#include <string>

class MonedaStabila : public Moneda {
private:
    std::string emitent;
    std::string moneda_legatura;
    double raport_colateralizare;

public:
    MonedaStabila(const std::string& nume,
                  const std::string& simbol,
                  double pret,
                  double volatilitate,
                  const std::string& emitent,
                  const std::string& moneda_legatura,
                  double raport_colateralizare);

    const std::string& obtine_emitent() const { return emitent; }
    const std::string& obtine_moneda_legatura() const { return moneda_legatura; }
    double obtine_raport_colateralizare() const { return raport_colateralizare; }

    void actualizare_pret() override;
    void afisare(std::ostream& os) const override;
    std::unique_ptr<Moneda> cloneaza() const override;
};

#endif
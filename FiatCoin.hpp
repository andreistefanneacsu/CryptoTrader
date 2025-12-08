#ifndef CRYPTOTRADER___FIATCOIN_HPP
#define CRYPTOTRADER___FIATCOIN_HPP

#include "Moneda.hpp"

class FiatCoin : public Moneda {
private:
    std::string tara_emitenta;
    std::string cod_iso;
    double rata_dobanda;
    double inflatie;

public:
    FiatCoin(const std::string& nume,
             const std::string& simbol,
             double pret,
             double volatilitate,
             const std::string& tara_emitenta,
             const std::string& cod_iso,
             double rata_dobanda,
             double inflatie);

    const std::string& get_tara_emitenta() const { return tara_emitenta; }
    const std::string& get_cod_iso() const { return cod_iso; }
    double get_rata_dobanda() const { return rata_dobanda; }
    double get_inflatie() const { return inflatie; }

    double calculeazaPutereCumparare() const;

    void actualizarePret() override;
    void afisare(std::ostream& os) const override;
};

#endif
#ifndef CRYPTOTRADER_MONEDAFIAT_HPP
#define CRYPTOTRADER_MONEDAFIAT_HPP

#include "Moneda.hpp"
#include <string>

class MonedaFiat : public Moneda {
private:
    std::string tara_emitenta;
    std::string cod_iso;
    double rata_dobanda;
    double rata_inflatie;

public:
    MonedaFiat(const std::string& nume,
               const std::string& simbol,
               double pret,
               double volatilitate,
               const std::string& tara_emitenta,
               const std::string& cod_iso,
               double rata_dobanda,
               double rata_inflatie);

    const std::string& obtine_tara_emitenta() const { return tara_emitenta; }
    const std::string& obtine_cod_iso() const { return cod_iso; }
    double obtine_rata_dobanda() const { return rata_dobanda; }
    double obtine_rata_inflatie() const { return rata_inflatie; }

    double calculeaza_putere_cumparare() const;

    void actualizare_pret() override;
    void afisare(std::ostream& os) const override;
    std::unique_ptr<Moneda> cloneaza() const override;
};

#endif
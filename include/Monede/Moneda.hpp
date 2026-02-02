#ifndef CRYPTOTRADER_MONEDA_HPP
#define CRYPTOTRADER_MONEDA_HPP

#include <iostream>
#include <string>
#include <memory>

class Moneda {
protected:
    std::string nume;
    std::string simbol;
    double pret;
    double volatilitate;

public:
    Moneda();
    Moneda(const std::string& nume, const std::string& simbol, 
           double pret, double volatilitate);
    Moneda(const Moneda& alta);
    virtual ~Moneda() = default;

    const std::string& obtine_nume() const { return nume; }
    const std::string& obtine_simbol() const { return simbol; }
    double obtine_pret() const { return pret; }
    double obtine_volatilitate() const { return volatilitate; }

    void seteaza_pret(double pret_nou) { pret = pret_nou; }
    void seteaza_volatilitate(double vol_nou) { volatilitate = vol_nou; }

    virtual void afisare(std::ostream& os) const;
    virtual void actualizare_pret() = 0;
    virtual std::unique_ptr<Moneda> cloneaza() const = 0;

    Moneda& operator=(const Moneda& alta);
    friend void interschimba(Moneda& x, Moneda &y) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Moneda& moneda);
};

#endif
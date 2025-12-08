#ifndef CRYPTOTRADER__V2_MONEDA_HPP
#define CRYPTOTRADER__V2_MONEDA_HPP

#include <iostream>
#include <string>

class Moneda {
protected:
    std::string nume;
    std::string simbol;
    double pret;
    double volatilitate;

public:
    Moneda();
    Moneda(const std::string&, const std::string&, double, double);
    Moneda(const Moneda&);
    virtual ~Moneda() = default;

    const std::string& get_nume() const { return nume; }
    const std::string& get_simbol() const { return simbol; }
    double get_pret() const { return pret; }
    double get_volatilitate() const { return volatilitate; }

    void set_pret(double pret_nou) { pret = pret_nou; }

    virtual void afisare(std::ostream& os) const;
    virtual void actualizarePret() = 0;

    void swap(Moneda& other) noexcept;

    Moneda& operator=(const Moneda&);

    friend std::ostream& operator<<(std::ostream& os, const Moneda& moneda);
};

#endif
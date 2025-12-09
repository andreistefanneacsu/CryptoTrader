#ifndef CRYPTOTRADER___ANALIZATORPIATA_HPP
#define CRYPTOTRADER___ANALIZATORPIATA_HPP

#include <vector>
#include <memory>
#include "Moneda.hpp"

class AnalizatorPiata {
private:
    std::vector<std::shared_ptr<Moneda>> monede;
    static int numar_analizoare_active;

public:
    AnalizatorPiata() { numar_analizoare_active++; }
    explicit AnalizatorPiata(const std::vector<std::shared_ptr<Moneda>>& monede);
    AnalizatorPiata(const AnalizatorPiata& other);
    AnalizatorPiata& operator=(AnalizatorPiata other);
    ~AnalizatorPiata();

    void swap(AnalizatorPiata& other) noexcept;

    double calculeazaVolatilitateMedie() const;
    std::vector<std::shared_ptr<Moneda>> filtreazaMonedeDupaVolatilitate(double min, double max) const;
    void actualizeazaToateMonedele();

    static int get_numar_analizoare_active() { return numar_analizoare_active; }
    static void reseteaza_contor() { numar_analizoare_active = 0; }

    void adaugaMoneda(std::shared_ptr<Moneda> moneda);
};

#endif
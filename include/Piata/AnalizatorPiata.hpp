#ifndef CRYPTOTRADER_ANALIZATORPIATA_HPP
#define CRYPTOTRADER_ANALIZATORPIATA_HPP

#include <vector>
#include <memory>
#include <string>

#include "Monede/Moneda.hpp"
#include "Sabloane/ContainerSablon.hpp"

class AnalizatorPiata {
private:
    std::vector<std::shared_ptr<Moneda>> monede;
    static int numar_analizoare_active;

    ContainerSablon<std::shared_ptr<Moneda>> monede_sortate_volatilitate;

public:
    AnalizatorPiata();
    explicit AnalizatorPiata(const std::vector<std::shared_ptr<Moneda>>& monede);
    AnalizatorPiata(const AnalizatorPiata& alta);
    AnalizatorPiata& operator=(AnalizatorPiata alta);
    ~AnalizatorPiata();

    void interschimba(AnalizatorPiata& alta) noexcept;

    double calculeaza_volatilitate_medie() const;
    double calculeaza_volatilitate_maxima() const;
    double calculeaza_volatilitate_minima() const;

    std::vector<std::shared_ptr<Moneda>>
    filtreaza_dupa_volatilitate(double min, double max) const;

    void afiseaza_statistici() const;
    void afiseaza_monede_volatile(double prag = 5.0) const;

    static int obtine_numar_analizoare_active();
};

#endif

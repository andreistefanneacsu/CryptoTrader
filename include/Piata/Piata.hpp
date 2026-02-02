#ifndef CRYPTOTRADER_PIATA_HPP
#define CRYPTOTRADER_PIATA_HPP

#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>
#include <map>
#include "Sabloane/SingletonSablon.hpp"
#include "Monede/Moneda.hpp"
#include "Monede/MonedaDeBaza.hpp"
#include "Monede/MonedaStabila.hpp"
#include "Monede/MonedaMeme.hpp"
#include "Monede/MonedaFiat.hpp"
#include "Monede/Altcoin.hpp"

class Piata : public SingletonSablon<Piata> {
private:
    std::vector<std::unique_ptr<Moneda>> monede;
    mutable std::mutex mutex_piata;
    std::atomic<bool> ruleaza_actualizare;
    std::string fisier_date;
    std::thread fir_actualizare;
    std::map<std::string, std::string> istoric_preturi;
    
    Piata();
    ~Piata() override;
    
    void incarca_din_fisier();
    void salveaza_in_fisier();
    void actualizare_automata();
    void actualizeaza_toate_monedele();
    
public:
    void initializeaza();
    
    void adauga_moneda(std::unique_ptr<Moneda> moneda);
    void elimina_moneda(const std::string& simbol);
    
    Moneda* cauta_moneda(const std::string& simbol) const;
    const std::vector<std::unique_ptr<Moneda>>& obtine_monede() const;
    
    void afiseaza_piata() const;
    void afiseaza_detalii_moneda(const std::string& simbol) const;
    
    double calculeaza_valoare_totala_piata() const;
    double calculeaza_volatilitate_medie() const;
    
    void actualizeaza_piata();
    void opreste_actualizare();
    
    friend class SingletonSablon<Piata>;
};

#endif

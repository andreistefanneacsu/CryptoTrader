#ifndef CRYPTOTRADER_FABRICASABLON_HPP
#define CRYPTOTRADER_FABRICASABLON_HPP

#include <memory>
#include <map>
#include <functional>
#include <string>
#include "Monede/Moneda.hpp"
#include "Monede/MonedaDeBaza.hpp"
#include "Monede/MonedaStabila.hpp"
#include "Monede/MonedaMeme.hpp"
#include "Monede/MonedaFiat.hpp"
#include "Monede/Altcoin.hpp"

class FabricaMonede {
private:
    using Creator = std::function<std::unique_ptr<Moneda>()>;
    std::map<std::string, Creator> creatori;
    
    FabricaMonede() {
        inregistreaza_creatori();
    }
    
    void inregistreaza_creatori() {
        inregistreaza_moneda("BITCOIN", []() {
            return std::make_unique<MonedaDeBaza>(
                "Bitcoin", "BTC", 30000.0, 3.0, 900.0, 0.9
            );
        });
        
        inregistreaza_moneda("ETHEREUM", []() {
            return std::make_unique<MonedaDeBaza>(
                "Ethereum", "ETH", 1800.0, 4.0, 450.0, 0.85
            );
        });
        
        inregistreaza_moneda("TETHER", []() {
            return std::make_unique<MonedaStabila>(
                "Tether", "USDT", 1.0, 0.2, "Tether Ltd", "USD", 110.0
            );
        });
        
        inregistreaza_moneda("DOGECOIN", []() {
            return std::make_unique<MonedaMeme>(
                "Dogecoin", "DOGE", 0.1, 15.0, 80.0, 0.6, 0.9
            );
        });
        
        inregistreaza_moneda("EURO", []() {
            return std::make_unique<MonedaFiat>(
                "Euro", "EUR", 4.9, 0.5, "Uniunea Europeană", "EUR", 4.5, 2.1
            );
        });
        
        inregistreaza_moneda("CARDANO", []() {
            return std::make_unique<Altcoin>(
                "Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85.0
            );
        });
    }
    
public:
    static FabricaMonede& obtine_instanta() {
        static FabricaMonede instanta;
        return instanta;
    }
    
    std::unique_ptr<Moneda> creeaza_moneda(const std::string& tip) {
        auto it = creatori.find(tip);
        if (it != creatori.end()) {
            return it->second();
        }
        return nullptr;
    }
    
    void inregistreaza_moneda(const std::string& tip, Creator creator) {
        creatori[tip] = creator;
    }
    
    std::vector<std::string> obtine_tipuri_disponibile() const {
        std::vector<std::string> tipuri;
        for (const auto& [tip, creator] : creatori) {
            tipuri.push_back(tip);
        }
        return tipuri;
    }
};

#endif
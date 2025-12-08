#include <iostream>
#include <ctime>
#include <memory>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "Moneda.hpp"
#include "BlueChipCoin.hpp"
#include "StableCoin.hpp"
#include "MemeCoin.hpp"
#include "FiatCoin.hpp"
#include "Altcoin.hpp"
#include "Portofel.hpp"
#include "Utilizator.hpp"
#include "Piata.hpp"
#include "AnalizatorPiata.hpp"
#include "Exceptions.hpp"

// Functie pentru a crea monede cu smart pointers pentru AnalizatorPiata
std::vector<std::shared_ptr<Moneda>> creazaMonedeShared() {
    std::vector<std::shared_ptr<Moneda>> monede;

    monede.push_back(std::make_shared<BlueChipCoin>("Bitcoin", "BTC", 30000, 3.0, 900, 0.9));
    monede.push_back(std::make_shared<BlueChipCoin>("Ethereum", "ETH", 1800, 4.0, 450, 0.85));
    monede.push_back(std::make_shared<StableCoin>("Tether", "USDT", 1.0, 0.2, "Tether Ltd", "USD", 110));
    monede.push_back(std::make_shared<MemeCoin>("Dogecoin", "DOGE", 0.1, 15, 80, 0.6, 0.9));
    monede.push_back(std::make_shared<FiatCoin>("Euro", "EUR", 4.9, 0.5, "Uniunea Europeana", "EUR", 4.5, 2.1));
    monede.push_back(std::make_shared<Altcoin>("Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85));

    return monede;
}

// Functie veche pentru compatibilitate cu restul codului
Piata* creazaPiata() {
    std::vector<Moneda*> monede;

    monede.push_back(new BlueChipCoin("Bitcoin", "BTC", 30000, 3.0, 900, 0.9));
    monede.push_back(new BlueChipCoin("Ethereum", "ETH", 1800, 4.0, 450, 0.85));
    monede.push_back(new StableCoin("Tether", "USDT", 1.0, 0.2, "Tether Ltd", "USD", 110));
    monede.push_back(new MemeCoin("Dogecoin", "DOGE", 0.1, 15, 80, 0.6, 0.9));
    monede.push_back(new FiatCoin("Euro", "EUR", 4.9, 0.5, "Uniunea Europeana", "EUR", 4.5, 2.1));
    monede.push_back(new Altcoin("Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85));

    return new Piata(monede);
}

void demoIerarhieMonede() {
    std::cout << "\n=== DEMO IERARHIE MONEDE ===\n";

    auto monede = creazaMonedeShared();

    for (const auto& moneda : monede) {
        std::cout << "\n" << *moneda;
    }
}

void demoFunctiiVirtuale() {
    std::cout << "\n=== DEMO FUNCTII VIRTUALE ===\n";

    Piata* piata = creazaPiata();

    std::cout << "Actualizare preturi (functie virtuala pura):\n";
    for (auto* moneda : piata->get_monede()) {
        double pretVechi = moneda->get_pret();
        moneda->actualizarePret();
        double pretNou = moneda->get_pret();
        std::cout << moneda->get_simbol() << ": " << pretVechi << " -> " << pretNou << " RON\n";
    }

    delete piata;
}

void demoClasaCuPointeriLaBaza() {
    std::cout << "\n=== DEMO CLASA CU POINTERI LA BAZA ===\n";

    Piata* piata = creazaPiata();
    Utilizator trader("Alex");
    trader.topUpFiat(5000);

    std::cout << "Cumparare monede folosind pointer la baza:\n";
    trader.cumpara(piata->find_moneda("BTC"), 1000);
    trader.cumpara(piata->find_moneda("ETH"), 500);
    trader.cumpara(piata->find_moneda("ADA"), 200);

    std::cout << "Portofel:\n" << trader.get_portofel();
    std::cout << "Valoare totala: " << trader.get_portofel().get_ValoarePortofel() << " RON\n";

    delete piata;
}

void demoDynamicCast() {
    std::cout << "\n=== DEMO DYNAMIC_CAST ===\n";

    Piata* piata = creazaPiata();

    for (const auto* moneda : piata->get_monede()) {
        if (const auto* fiat = dynamic_cast<const FiatCoin*>(moneda)) {
            std::cout << "FiatCoin gasit: " << fiat->get_nume()
                      << " (" << fiat->get_cod_iso() << ")\n";
            std::cout << "Putere cumparare: " << fiat->calculeazaPutereCumparare() << "\n";
            std::cout << "Tara emitenta: " << fiat->get_tara_emitenta() << "\n";
            std::cout << "Rata dobanda: " << fiat->get_rata_dobanda() << "%\n";
            std::cout << "Inflatie: " << fiat->get_inflatie() << "%\n";
        }

        if (const auto* stable = dynamic_cast<const StableCoin*>(moneda)) {
            std::cout << "StableCoin: " << stable->get_nume()
                      << " colateralizat la " << stable->get_raport_colateralizare() << "%\n";
            std::cout << "Emitent: " << stable->get_emitent() << "\n";
            std::cout << "Moneda legatura: " << stable->get_moneda_legatura() << "\n";
        }

        if (const auto* altcoin = dynamic_cast<const Altcoin*>(moneda)) {
            std::cout << "Altcoin: " << altcoin->get_nume()
                      << " - scor tehnologie: " << altcoin->get_scor_tehnologie() << "\n";
            std::cout << "Scalabilitate: " << altcoin->get_scalabilitate() << "\n";
            std::cout << "Interoperabilitate: " << altcoin->get_interoperabilitate() << "\n";
            std::cout << "Risc calculat: " << altcoin->calculeazaRisc() << "\n";
            std::cout << "Potential calculat: " << altcoin->calculeazaPotential() << "\n";
        }

        if (const auto* meme = dynamic_cast<const MemeCoin*>(moneda)) {
            std::cout << "MemeCoin: " << meme->get_nume()
                      << " - factor meme: " << meme->get_factor_meme() << "\n";
            std::cout << "Nivel hype: " << meme->get_nivel_hype() << "\n";
            std::cout << "Scor comunitate: " << meme->get_scor_comunitate() << "\n";
        }

        if (const auto* bluechip = dynamic_cast<const BlueChipCoin*>(moneda)) {
            std::cout << "BlueChipCoin: " << bluechip->get_nume()
                      << " - capital piata: " << bluechip->get_capital_piata() << "B\n";
            std::cout << "Factor incredere: " << bluechip->get_factor_incredere() << "\n";
        }
    }

    delete piata;
}

void demoCopySwap() {
    std::cout << "\n=== DEMO COPY AND SWAP ===\n";

    // Folosim smart pointers pentru a evita heap-use-after-free
    auto monede = creazaMonedeShared();

    AnalizatorPiata analizator1(monede);

    // Test copy constructor
    AnalizatorPiata analizator2 = analizator1;

    // Test copy assignment
    AnalizatorPiata analizator3;
    analizator3 = analizator2;

    std::cout << "Analizoare active: " << AnalizatorPiata::get_numar_analizoare_active() << "\n";
    std::cout << "Volatilitate medie: " << analizator3.calculeazaVolatilitateMedie() << "%\n";

    analizator2.actualizeazaToateMonedele();
    AnalizatorPiata::reseteaza_contor();
}

void demoExceptii() {
    std::cout << "\n=== DEMO EXCEPTII ===\n";

    Utilizator trader("Andrei");
    trader.topUpFiat(100);

    Piata* piata = creazaPiata();

    try {
        trader.cumpara(piata->find_moneda("BTC"), 1000);
        std::cout << "Tranzactie reusita\n";
    }
    catch (const EroareFonduriInsuficiente& e) {
        std::cout << "Exceptie prinsa: " << e.what() << "\n";
    }
    catch (const CryptoException& e) {
        std::cout << "Exceptie generala: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Exceptie standard: " << e.what() << "\n";
    }

    delete piata;
}

void demoSmartPointers() {
    std::cout << "\n=== DEMO SMART POINTERS ===\n";

    auto monede = creazaMonedeShared();

    for (const auto& moneda : monede) {
        std::cout << *moneda << "\n";
    }
}

void demoSTL() {
    std::cout << "\n=== DEMO UTILIZARE STL ===\n";

    // Creaza monede separate pentru acest demo
    auto monede = creazaMonedeShared();
    AnalizatorPiata analizator(monede);

    auto volatileMonede = analizator.filtreazaMonedeDupaVolatilitate(5, 20);
    std::cout << "Monede volatile (5%-20%):\n";
    for (const auto& moneda : volatileMonede) {
        std::cout << "  " << moneda->get_simbol() << ": " << moneda->get_volatilitate() << "%\n";
    }
}

void demoFunctiiNivelInalt() {
    std::cout << "\n=== DEMO FUNCTII NIVEL INALT ===\n";

    Piata* piata = creazaPiata();
    Utilizator trader("Cristi");
    trader.topUpFiat(10000);

    trader.cumpara(piata->find_moneda("BTC"), 3000);
    trader.cumpara(piata->find_moneda("ETH"), 2000);
    trader.cumpara(piata->find_moneda("ADA"), 1000);

    std::cout << "Statistici trader:\n";
    std::cout << "Valoare portofel: " << trader.get_portofel().get_ValoarePortofel() << " RON\n";
    std::cout << "Profit total: " << trader.get_portofel().get_ProfitTotal() << " RON\n";
    std::cout << "Balanța RON: " << trader.get_balanta_fiat() << " RON\n";

    auto estimare = trader.get_portofel().estimareProfitTotal();
    std::cout << "Estimare profit: " << estimare.first << " ... " << estimare.second << " RON\n";

    trader.determinareValoarePortofel();
    trader.determinareProfitPosibil();
    trader.determinareProfitTotal();

    delete piata;
}

void demoNouaClasaDerivata() {
    std::cout << "\n=== DEMO NOUA CLASA DERIVATA (ALTCOIN) ===\n";

    auto ada = std::make_shared<Altcoin>("Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85);
    auto sol = std::make_shared<Altcoin>("Solana", "SOL", 20, 12, 0.8, 0.6, 88);

    std::cout << *ada << "\n";
    std::cout << *sol << "\n";

    ada->actualizarePret();
    sol->actualizarePret();

    std::cout << "Dupa actualizare:\n";
    std::cout << *ada << "\n";
    std::cout << *sol << "\n";
}

void testareSetPret() {
    std::cout << "\n=== TESTARE SET_PRET ===\n";

    Piata* piata = creazaPiata();
    Moneda* moneda = piata->find_moneda("BTC");

    double pretVechi = moneda->get_pret();
    moneda->set_pret(31000);
    double pretNou = moneda->get_pret();

    std::cout << "BTC pret modificat: " << pretVechi << " -> " << pretNou << " RON\n";

    delete piata;
}

void testareTranzactii() {
    std::cout << "\n=== TESTARE TRANZACTII ===\n";

    Piata* piata = creazaPiata();
    Utilizator trader("Test");
    trader.topUpFiat(10000);

    trader.cumpara(piata->find_moneda("BTC"), 5000);
    trader.cumpara(piata->find_moneda("ETH"), 2000);

    const auto& tranzactii = trader.get_portofel().get_tranzactii();
    std::cout << "Numar tranzactii: " << tranzactii.size() << "\n";

    for (const auto* tranzactie : tranzactii) {
        std::cout << "Tranzactie: " << *tranzactie << "\n";
    }

    delete piata;
}

void testarePiataActualizare() {
    std::cout << "\n=== TESTARE ACTUALIZARE PIATA ===\n";

    Piata* piata = creazaPiata();

    std::cout << "Preturi inainte de actualizare:\n";
    for (const auto* moneda : piata->get_monede()) {
        std::cout << moneda->get_simbol() << ": " << moneda->get_pret() << " RON\n";
    }

    piata->actualizarePiata();

    std::cout << "\nPreturi dupa actualizare:\n";
    for (const auto* moneda : piata->get_monede()) {
        std::cout << moneda->get_simbol() << ": " << moneda->get_pret() << " RON\n";
    }

    delete piata;
}

void testarePortofelFunctii() {
    std::cout << "\n=== TESTARE FUNCTII PORTOFEL ===\n";

    Piata* piata = creazaPiata();
    Utilizator trader("TestPortofel");
    trader.topUpFiat(15000);

    trader.cumpara(piata->find_moneda("BTC"), 5000);
    trader.cumpara(piata->find_moneda("ETH"), 3000);
    trader.cumpara(piata->find_moneda("ADA"), 1000);

    const auto& monedePortofel = trader.get_portofel().get_monede();
    std::cout << "Monede in portofel: " << monedePortofel.size() << "\n";

    for (const auto& moneda : monedePortofel) {
        std::cout << moneda.first->get_simbol() << ": " << moneda.second.first << " bucati\n";
    }

    delete piata;
}

int main() {
    std::cout << "=== CRYPTOTRADER++ - PROIECT OOP COMPLET ===\n";

    srand(static_cast<unsigned>(time(nullptr)));

    try {
        demoIerarhieMonede();
        demoFunctiiVirtuale();
        demoClasaCuPointeriLaBaza();
        demoDynamicCast();
        demoCopySwap();
        demoExceptii();
        demoSmartPointers();
        demoSTL();
        demoFunctiiNivelInalt();
        demoNouaClasaDerivata();
        testareSetPret();
        testareTranzactii();
        testarePiataActualizare();
        testarePortofelFunctii();
        
    } catch (const std::exception& e) {
        std::cerr << "\nEROARE: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

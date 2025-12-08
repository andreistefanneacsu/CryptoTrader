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

    std::vector<Moneda*> monede;
    monede.push_back(new BlueChipCoin("Bitcoin", "BTC", 30000, 3.0, 900, 0.9));
    monede.push_back(new StableCoin("Tether", "USDT", 1.0, 0.2, "Tether Ltd", "USD", 110));
    monede.push_back(new MemeCoin("Dogecoin", "DOGE", 0.1, 15, 80, 0.6, 0.9));
    monede.push_back(new FiatCoin("Euro", "EUR", 4.9, 0.5, "Uniunea Europeana", "EUR", 4.5, 2.1));
    monede.push_back(new Altcoin("Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85));

    for (const auto* moneda : monede) {
        std::cout << "\n" << *moneda;
    }

    for (auto* m : monede) delete m;
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
        }

        if (const auto* stable = dynamic_cast<const StableCoin*>(moneda)) {
            std::cout << "StableCoin: " << stable->get_nume()
                      << " colateralizat la " << stable->get_raport_colateralizare() << "%\n";
        }

        if (const auto* altcoin = dynamic_cast<const Altcoin*>(moneda)) {
            std::cout << "Altcoin: " << altcoin->get_nume()
                      << " - scor tehnologie: " << altcoin->get_scor_tehnologie() << "\n";
        }
    }

    delete piata;
}

void demoCopySwap() {
    std::cout << "\n=== DEMO COPY AND SWAP ===\n";

    AnalizatorPiata analizator1;
    {
        Piata* piata = creazaPiata();
        for (auto* moneda : piata->get_monede()) {
            analizator1.adaugaMoneda(moneda);
        }
        delete piata;
    }

    AnalizatorPiata analizator2 = analizator1;
    AnalizatorPiata analizator3;
    analizator3 = analizator2;

    std::cout << "Analizoare active: " << AnalizatorPiata::get_numar_analizoare_active() << "\n";
    std::cout << "Volatilitate medie: " << analizator3.calculeazaVolatilitateMedie() << "%\n";
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

    std::vector<std::unique_ptr<Moneda>> monede;
    monede.push_back(std::make_unique<BlueChipCoin>("Bitcoin", "BTC", 30000, 3.0, 900, 0.9));
    monede.push_back(std::make_unique<StableCoin>("Tether", "USDT", 1.0, 0.2, "Tether Ltd", "USD", 110));
    monede.push_back(std::make_unique<Altcoin>("Solana", "SOL", 20, 12, 0.8, 0.6, 88));

    for (const auto& moneda : monede) {
        std::cout << *moneda << "\n";
    }
}

void demoSTL() {
    std::cout << "\n=== DEMO UTILIZARE STL ===\n";

    Piata* piata = creazaPiata();
    AnalizatorPiata analizator(piata->get_monede());

    auto volatileMonede = analizator.filtreazaMonedeDupaVolatilitate(5, 20);
    std::cout << "Monede volatile (5%-20%):\n";
    for (const auto* moneda : volatileMonede) {
        std::cout << "  " << moneda->get_simbol() << ": " << moneda->get_volatilitate() << "%\n";
    }

    delete piata;
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

    auto estimare = trader.get_portofel().estimareProfitTotal();
    std::cout << "Estimare profit: " << estimare.first << " ... " << estimare.second << " RON\n";

    trader.determinareProfitPosibil();

    delete piata;
}

void demoNouaClasaDerivata() {
    std::cout << "\n=== DEMO NOUA CLASA DERIVATA (ALTCOIN) ===\n";

    Altcoin* ada = new Altcoin("Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85);
    Altcoin* sol = new Altcoin("Solana", "SOL", 20, 12, 0.8, 0.6, 88);

    std::cout << *ada << "\n";
    std::cout << *sol << "\n";

    ada->actualizarePret();
    sol->actualizarePret();

    std::cout << "Dupa actualizare:\n";
    std::cout << *ada << "\n";
    std::cout << *sol << "\n";

    delete ada;
    delete sol;
}

void testareFunctiiUtilizate() {
    Piata* piata = creazaPiata();
    Utilizator trader("Test");
    trader.topUpFiat(5000);

    trader.cumpara(piata->find_moneda("BTC"), 1000);

    double balanta = trader.get_balanta_fiat();
    trader.determinareValoarePortofel();
    trader.determinareProfitTotal();

    Moneda* moneda = piata->find_moneda("EUR");
    if (auto* fiat = dynamic_cast<FiatCoin*>(moneda)) {
        std::string tara = fiat->get_tara_emitenta();
        double dobanda = fiat->get_rata_dobanda();
        double inflatie = fiat->get_inflatie();
    }

    if (auto* meme = dynamic_cast<MemeCoin*>(piata->find_moneda("DOGE"))) {
        double factor = meme->get_factor_meme();
        double hype = meme->get_nivel_hype();
        double comunitate = meme->get_scor_comunitate();
    }

    if (auto* bluechip = dynamic_cast<BlueChipCoin*>(piata->find_moneda("BTC"))) {
        double capital = bluechip->get_capital_piata();
        double incredere = bluechip->get_factor_incredere();
    }

    if (auto* stable = dynamic_cast<StableCoin*>(piata->find_moneda("USDT"))) {
        std::string emitent = stable->get_emitent();
        std::string legatura = stable->get_moneda_legatura();
    }

    const auto& tranzactii = trader.get_portofel().get_tranzactii();

    moneda->set_pret(31000);

    piata->actualizarePiata();

    AnalizatorPiata::reseteaza_contor();

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

        testareFunctiiUtilizate();

    } catch (const std::exception& e) {
        std::cerr << "\nEROARE: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
#include "Piata/Piata.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <locale>

namespace fs = std::filesystem;

Piata::Piata() : ruleaza_actualizare(true) {
    fisier_date = "date/piata/piata.dat";

    std::string dir_path = "date/piata";
    if (!fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

    if (fs::exists(fisier_date)) {
        incarca_din_fisier();
    }

    if (monede.empty()) {
        initializeaza();
    }

}

Piata::~Piata() {
    opreste_actualizare();
    salveaza_in_fisier();
}

void Piata::incarca_din_fisier() {
    std::lock_guard<std::mutex> blocare(mutex_piata);

    std::ifstream fisier(fisier_date);
    fisier.imbue(std::locale::classic());

    if (!fisier.is_open()) return;

    std::string linie;
    monede.clear();

    while (std::getline(fisier, linie)) {
        try {
            std::istringstream iss(linie);
            iss.imbue(std::locale::classic());

            std::string tip, nume, simbol;
            double pret, volatilitate;

            if (!(iss >> tip >> nume >> simbol >> pret >> volatilitate)) {
                continue;
            }

            std::replace(nume.begin(), nume.end(), '_', ' ');

            if (tip == "MONEDADEBAZA") {
                double capital, incredere;
                if (iss >> capital >> incredere) {
                    monede.push_back(std::make_unique<MonedaDeBaza>(
                        nume, simbol, pret, volatilitate, capital, incredere
                    ));
                }
            }
            else if (tip == "MONEDASTABILA") {
                std::string emitent, moneda_legatura;
                double raport;
                if (iss >> emitent >> moneda_legatura >> raport) {
                    std::replace(emitent.begin(), emitent.end(), '_', ' ');
                    monede.push_back(std::make_unique<MonedaStabila>(
                        nume, simbol, pret, volatilitate, emitent, moneda_legatura, raport
                    ));
                }
            }
            else if (tip == "MONEDAMEME") {
                double factor_meme, nivel_hype, scor_comunitate;
                if (iss >> factor_meme >> nivel_hype >> scor_comunitate) {
                    monede.push_back(std::make_unique<MonedaMeme>(
                        nume, simbol, pret, volatilitate, factor_meme, nivel_hype, scor_comunitate
                    ));
                }
            }
            else if (tip == "MONEDAFIAT") {
                std::string tara_emitenta, cod_iso;
                double rata_dobanda, rata_inflatie;
                if (iss >> tara_emitenta >> cod_iso >> rata_dobanda >> rata_inflatie) {
                    std::replace(tara_emitenta.begin(), tara_emitenta.end(), '_', ' ');
                    monede.push_back(std::make_unique<MonedaFiat>(
                        nume, simbol, pret, volatilitate, tara_emitenta, cod_iso, rata_dobanda, rata_inflatie
                    ));
                }
            }
            else if (tip == "ALTCOIN") {
                double scalabilitate, interoperabilitate, scor_tehnologie;
                if (iss >> scalabilitate >> interoperabilitate >> scor_tehnologie) {
                    monede.push_back(std::make_unique<Altcoin>(
                        nume, simbol, pret, volatilitate, scalabilitate, interoperabilitate, scor_tehnologie
                    ));
                }
            }
        } catch (...) {
        }
    }
    fisier.close();
}

void Piata::salveaza_in_fisier() {
    std::lock_guard<std::mutex> blocare(mutex_piata);

    fs::path cale_fisier(fisier_date);
    fs::path director = cale_fisier.parent_path();
    if (!fs::exists(director)) fs::create_directories(director);

    std::ofstream fisier(fisier_date);
    fisier.imbue(std::locale::classic());

    if (!fisier.is_open()) return;

    fisier << std::fixed << std::setprecision(5);

    for (const auto& moneda : monede) {
        std::string nume_salvare = moneda->obtine_nume();
        std::replace(nume_salvare.begin(), nume_salvare.end(), ' ', '_');

        if (const auto* de_baza = dynamic_cast<const MonedaDeBaza*>(moneda.get())) {
            fisier << "MONEDADEBAZA " << nume_salvare << " "
                   << de_baza->obtine_simbol() << " "
                   << de_baza->obtine_pret() << " "
                   << de_baza->obtine_volatilitate() << " "
                   << de_baza->obtine_capital_piata() << " "
                   << de_baza->obtine_factor_incredere() << "\n";
        }
        else if (const auto* stabila = dynamic_cast<const MonedaStabila*>(moneda.get())) {
            std::string emitent_salvare = stabila->obtine_emitent();
            std::replace(emitent_salvare.begin(), emitent_salvare.end(), ' ', '_');

            fisier << "MONEDASTABILA " << nume_salvare << " "
                   << stabila->obtine_simbol() << " "
                   << stabila->obtine_pret() << " "
                   << stabila->obtine_volatilitate() << " "
                   << emitent_salvare << " "
                   << stabila->obtine_moneda_legatura() << " "
                   << stabila->obtine_raport_colateralizare() << "\n";
        }
        else if (const auto* meme = dynamic_cast<const MonedaMeme*>(moneda.get())) {
            fisier << "MONEDAMEME " << nume_salvare << " "
                   << meme->obtine_simbol() << " "
                   << meme->obtine_pret() << " "
                   << meme->obtine_volatilitate() << " "
                   << meme->obtine_factor_meme() << " "
                   << meme->obtine_nivel_hype() << " "
                   << meme->obtine_scor_comunitate() << "\n";
        }
        else if (const auto* fiat = dynamic_cast<const MonedaFiat*>(moneda.get())) {
            std::string tara_salvare = fiat->obtine_tara_emitenta();
            std::replace(tara_salvare.begin(), tara_salvare.end(), ' ', '_');

            fisier << "MONEDAFIAT " << nume_salvare << " "
                   << fiat->obtine_simbol() << " "
                   << fiat->obtine_pret() << " "
                   << fiat->obtine_volatilitate() << " "
                   << tara_salvare << " "
                   << fiat->obtine_cod_iso() << " "
                   << fiat->obtine_rata_dobanda() << " "
                   << fiat->obtine_rata_inflatie() << "\n";
        }
        else if (const auto* altcoin = dynamic_cast<const Altcoin*>(moneda.get())) {
            fisier << "ALTCOIN " << nume_salvare << " "
                   << altcoin->obtine_simbol() << " "
                   << altcoin->obtine_pret() << " "
                   << altcoin->obtine_volatilitate() << " "
                   << altcoin->obtine_scalabilitate() << " "
                   << altcoin->obtine_interoperabilitate() << " "
                   << altcoin->obtine_scor_tehnologie() << "\n";
        }
    }
    fisier.close();
}

/*
void Piata::actualizare_automata() {
    while (ruleaza_actualizare) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        actualizeaza_piata();
        salveaza_in_fisier();
    }
}
*/

void Piata::initializeaza() {
    if (!monede.empty()) return;

    monede.push_back(std::make_unique<MonedaDeBaza>(
        "Bitcoin", "BTC", 30000.0, 3.0, 900.0, 0.9
    ));
    monede.push_back(std::make_unique<MonedaDeBaza>(
        "Ethereum", "ETH", 1800.0, 4.0, 450.0, 0.85
    ));
    monede.push_back(std::make_unique<MonedaStabila>(
        "Tether", "USDT", 1.0, 0.2, "Tether Ltd", "USD", 110.0
    ));
    monede.push_back(std::make_unique<MonedaMeme>(
        "Dogecoin", "DOGE", 0.1, 15.0, 80.0, 0.6, 0.9
    ));
    monede.push_back(std::make_unique<MonedaFiat>(
        "Euro", "EUR", 4.9, 0.5, "Uniunea Europeana", "EUR", 4.5, 2.1
    ));
    monede.push_back(std::make_unique<Altcoin>(
        "Cardano", "ADA", 0.4, 8.5, 0.7, 0.4, 85.0
    ));

    salveaza_in_fisier();
}

Moneda* Piata::cauta_moneda(const std::string& simbol) const {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    for (const auto& moneda : monede) {
        if (moneda->obtine_simbol() == simbol) {
            return moneda.get();
        }
    }
    return nullptr;
}

void Piata::afiseaza_piata() const {
    std::lock_guard<std::mutex> blocare(mutex_piata);

    if (monede.empty()) {
        std::cout << "\n=== PIATA CRYPTO ===\n";
        std::cout << "Piata este goala!\n";
        return;
    }

    std::setlocale(LC_ALL, "ro_RO.UTF-8");

    std::cout << "\n=== PIATA CRYPTO ===\n";
    std::cout << std::left << std::setw(10) << "Simbol"
              << std::setw(20) << "Nume"
              << std::setw(15) << "Pret (RON)"
              << std::setw(15) << "Volatilitate%"
              << std::setw(15) << "Tip" << "\n";
    std::cout << std::string(75, '-') << "\n";

    for (const auto& moneda : monede) {
        std::string tip = "Necunoscut";
        if (dynamic_cast<const MonedaDeBaza*>(moneda.get())) tip = "De Baza";
        else if (dynamic_cast<const MonedaStabila*>(moneda.get())) tip = "Stabila";
        else if (dynamic_cast<const MonedaMeme*>(moneda.get())) tip = "Meme";
        else if (dynamic_cast<const MonedaFiat*>(moneda.get())) tip = "Fiat";
        else if (dynamic_cast<const Altcoin*>(moneda.get())) tip = "Altcoin";

        std::cout << std::left << std::setw(10) << moneda->obtine_simbol()
                  << std::setw(20) << moneda->obtine_nume()
                  << std::setw(15) << std::fixed << std::setprecision(2) << moneda->obtine_pret()
                  << std::setw(15) << std::setprecision(1) << moneda->obtine_volatilitate()
                  << std::setw(15) << tip << "\n";
    }

}

double Piata::calculeaza_valoare_totala_piata() const {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    if (monede.empty()) return 0.0;
    double total = 0.0;
    for (const auto& moneda : monede) {
        total += moneda->obtine_pret() * 1000.0;
    }
    return total;
}

double Piata::calculeaza_volatilitate_medie() const {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    if (monede.empty()) return 0.0;
    double suma = 0.0;
    for (const auto& moneda : monede) {
        suma += moneda->obtine_volatilitate();
    }
    return suma / static_cast<double>(monede.size());
}

void Piata::actualizeaza_toate_monedele() {
    for (auto& moneda : monede) {
        moneda->actualizare_pret();
    }
}

void Piata::actualizeaza_piata() {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    actualizeaza_toate_monedele();
}

void Piata::opreste_actualizare() {
    ruleaza_actualizare = false;
    if (fir_actualizare.joinable()) {
        fir_actualizare.join();
    }
}

const std::vector<std::unique_ptr<Moneda>>& Piata::obtine_monede() const {
    return monede;
}

void Piata::adauga_moneda(std::unique_ptr<Moneda> moneda) {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    monede.push_back(std::move(moneda));
}

void Piata::elimina_moneda(const std::string& simbol) {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    auto it = std::remove_if(monede.begin(), monede.end(),
        [&](const std::unique_ptr<Moneda>& m) {
            return m->obtine_simbol() == simbol;
        });
    monede.erase(it, monede.end());
}

void Piata::afiseaza_detalii_moneda(const std::string& simbol) const {
    std::lock_guard<std::mutex> blocare(mutex_piata);
    for (const auto& moneda : monede) {
        if (moneda->obtine_simbol() == simbol) {
            moneda->afisare(std::cout);
            std::cout << "\n";
            return;
        }
    }
    std::cout << "Moneda cu simbolul " << simbol << " nu a fost gasita.\n";

}

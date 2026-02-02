#include "Utilitare/InterfataTerminal.hpp"
#include "Utilitare/CititorFisier.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <clocale>
#include <algorithm>
#include <chrono>

#ifdef _WIN32
    #define CURATA_ECRAN "cls"
#else
    #define CURATA_ECRAN "clear"
#endif

InterfataTerminal::InterfataTerminal()
    : piata(Piata::obtine_instanta()),
      manager_utilizatori(ManagerUtilizatori::obtine_instanta()),
      ruleaza(true),
      este_admin(false) {

    std::setlocale(LC_ALL, "ro_RO.UTF-8");

    CititorFisier::incarca_comenzi_din_fisier("tastatura.txt");

    bool este_ci = (std::getenv("CI") != nullptr);
    if (este_ci) {
        std::cout << "[CI] Mod CI activat\n";
        CititorFisier::seteaza_mod_testare(true);
    }

    std::vector<std::shared_ptr<Moneda>> monede_shared;
    for (const auto& moneda : piata.obtine_monede()) {
        monede_shared.push_back(std::shared_ptr<Moneda>(moneda->cloneaza()));
    }
    analizator = AnalizatorPiata(monede_shared);
}

InterfataTerminal::~InterfataTerminal() {
    opreste();
}

void InterfataTerminal::porneste() {
    auto start_time = std::chrono::steady_clock::now();
    bool este_ci = (std::getenv("CI") != nullptr);

    if (!este_ci) {
        curata_ecran();
        afiseaza_antet("CRYPTO TRADER PRO v1.0");
    }

    while (ruleaza) {
        if (este_ci) {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
            if (elapsed.count() > 30) {
                std::cout << "[CI] Timeout 30 secunde\n";
                ruleaza = false;
                break;
            }
        }

        try {
            if (!utilizator_curent) {
                afiseaza_meniu_autentificare();
            } else if (este_admin) {
                afiseaza_meniu_admin();
            } else {
                afiseaza_meniu_utilizator();
            }
        } catch (const std::exception& e) {
            afiseaza_eroare(e.what());
            asteapta_enter();
        }
    }
}

void InterfataTerminal::afiseaza_meniu_admin() {
    curata_ecran();
    afiseaza_antet("MENIU ADMIN");

    std::cout << "\n=== OPTIUNI ADMIN ===\n";
    std::cout << "1. Vizualizeaza piata\n";
    std::cout << "2. Statistici piata\n";
    std::cout << "3. Vezi toti utilizatorii\n";
    std::cout << "4. Logout admin\n";
    std::cout << "\nAlegeti o optiune: ";

    int opt = citeste_optiune(1, 4);

    switch (opt) {
        case 1:
            afiseaza_meniu_piata();
            break;
        case 2:
            afiseaza_meniu_statistici();
            break;
        case 3:
            manager_utilizatori.afiseaza_toti_utilizatorii();
            asteapta_enter();
            break;
        case 4:
            este_admin = false;
            break;
    }
}

void InterfataTerminal::afiseaza_detalii_moneda() {
    curata_ecran();
    afiseaza_antet("DETALII MONEDA");

    std::string simbol = CititorFisier::citeste_input<std::string>("Simbol moneda: ");
    const auto* moneda = piata.cauta_moneda(simbol);

    if (!moneda) {
        afiseaza_eroare("Moneda inexistenta!");
        asteapta_enter();
        return;
    }

    std::cout << "\n" << *moneda << "\n";
    asteapta_enter();
}

void InterfataTerminal::proceseaza_vanzare() {
    curata_ecran();
    afiseaza_antet("VANZARE MONEDA");

    std::string simbol = CititorFisier::citeste_input<std::string>("Simbol moneda: ");
    double cantitate = CititorFisier::citeste_input<double>("Cantitate: ");

    if (cantitate <= 0) {
        afiseaza_eroare("Cantitatea trebuie sa fie pozitiva!");
        asteapta_enter();
        return;
    }

    try {
        utilizator_curent->vinde(simbol, cantitate);
        manager_utilizatori.salveaza_toti_utilizatorii();
        afiseaza_mesaj("Vanzare realizata cu succes.");
    } catch (const std::exception& e) {
        afiseaza_eroare(e.what());
    }

    asteapta_enter();
}

void InterfataTerminal::proceseaza_retragere() {
    curata_ecran();
    afiseaza_antet("RETRAGERE RON");

    double suma = CititorFisier::citeste_input<double>("Suma de retras (RON): ");

    try {
        utilizator_curent->retrage_ron(suma);
        manager_utilizatori.salveaza_toti_utilizatorii();
        afiseaza_mesaj("Retragere efectuata.");
    } catch (const std::exception& e) {
        afiseaza_eroare(e.what());
    }

    asteapta_enter();
}

void InterfataTerminal::afiseaza_meniu_autentificare() {
    curata_ecran();
    afiseaza_antet("AUTENTIFICARE");

    std::cout << "\n=== MENIU PRINCIPAL ===\n";
    std::cout << "1. Autentificare utilizator\n";
    std::cout << "2. Inregistrare utilizator nou\n";
    std::cout << "3. Autentificare administrator\n";
    std::cout << "4. Iesire din aplicatie\n";
    std::cout << "\nAlegeti o optiune: ";

    int optiune = citeste_optiune(1, 4);

    switch (optiune) {
        case 1: {
            std::string nume = CititorFisier::citeste_input<std::string>("Nume utilizator: ");
            std::string parola = CititorFisier::citeste_input<std::string>("Parola: ");

            utilizator_curent = manager_utilizatori.autentificare(nume, parola);
            if (utilizator_curent) {
                afiseaza_mesaj("Autentificare reusita! Bun venit, " + nume + "!");
                asteapta_enter();
            } else {
                afiseaza_eroare("Autentificare esuata! Nume sau parola incorecte.");
                asteapta_enter();
            }
            break;
        }
        case 2: {
            std::string nume = CititorFisier::citeste_input<std::string>("Nume utilizator: ");
            std::string email = CititorFisier::citeste_input<std::string>("Email: ");
            std::string parola = CititorFisier::citeste_input<std::string>("Parola: ");
            double depozit = CititorFisier::citeste_input<double>("Depozit initial (RON): ");

            if (depozit < 0) {
                afiseaza_eroare("Depozitul trebuie sa fie pozitiv!");
                asteapta_enter();
                break;
            }

            try {
                utilizator_curent = manager_utilizatori.inregistrare(nume, email, parola, depozit);
                afiseaza_mesaj("Inregistrare reusita! Cont creat cu " +
                              std::to_string(depozit) + " RON.");
                asteapta_enter();
            } catch (const std::exception& e) {
                afiseaza_eroare(e.what());
                asteapta_enter();
            }
            break;
        }
        case 3: {
            std::string parola = CititorFisier::citeste_input<std::string>("Parola administrator: ");
            if (parola == "admin123") {
                este_admin = true;
                afiseaza_mesaj("Autentificare administrator reusita!");
                asteapta_enter();
            } else {
                afiseaza_eroare("Parola incorecta!");
                asteapta_enter();
            }
            break;
        }
        case 4:
            ruleaza = false;
            break;
    }
}

void InterfataTerminal::afiseaza_meniu_utilizator() {
    curata_ecran();
    afiseaza_antet("MENIU UTILIZATOR");

    std::cout << "\nUtilizator: " << utilizator_curent->obtine_nume() << "\n";
    std::cout << "Balanta RON: " << std::fixed << std::setprecision(2)
              << utilizator_curent->obtine_balanta_ron() << " RON\n";
    std::cout << "Valoare portofel: "
              << utilizator_curent->obtine_portofel().obtine_valoare_portofel()
              << " RON\n\n";

    std::cout << "=== OPTIUNI DISPONIBILE ===\n";
    std::cout << "1. Vizualizeaza piata\n";
    std::cout << "2. Detalii moneda\n";
    std::cout << "3. Cumpara moneda\n";
    std::cout << "4. Vinde moneda\n";
    std::cout << "5. Portofel\n";
    std::cout << "6. Depunere RON\n";
    std::cout << "7. Retragere RON\n";
    std::cout << "8. Statistici\n";
    std::cout << "9. Deconectare\n";
    std::cout << "0. Iesire\n";
    std::cout << "\nAlegeti o optiune: ";

    int optiune = citeste_optiune(0, 9);

    switch (optiune) {
        case 1:
            afiseaza_meniu_piata();
            break;
        case 2:
            afiseaza_detalii_moneda();
            break;
        case 3:
            proceseaza_cumparare();
            break;
        case 4:
            proceseaza_vanzare();
            break;
        case 5:
            gestioneaza_portofel();
            break;
        case 6:
            proceseaza_depunere();
            break;
        case 7:
            proceseaza_retragere();
            break;
        case 8:
            afiseaza_meniu_statistici();
            break;
        case 9:
            utilizator_curent.reset();
            break;
        case 0:
            ruleaza = false;
            break;
    }
}

void InterfataTerminal::afiseaza_meniu_piata() {
    curata_ecran();
    afiseaza_antet("PIATA CRYPTO");

    piata.afiseaza_piata();

    std::cout << "\n=== OPTIUNI ===\n";
    std::cout << "1. Actualizeaza preturi\n";
    std::cout << "2. Inapoi\n";
    std::cout << "\nAlegeti o optiune: ";

    int optiune = citeste_optiune(1, 2);

    if (optiune == 1) {
        piata.actualizeaza_piata();
        afiseaza_mesaj("Preturile au fost actualizate!");
        asteapta_enter();
        afiseaza_meniu_piata();
    }
}

void InterfataTerminal::proceseaza_cumparare() {
    curata_ecran();
    afiseaza_antet("CUMPARARE MONEDA");

    bool este_ci = (std::getenv("CI") != nullptr);

    if (este_ci) {
        std::cout << "\nMonede disponibile:\n";
        const auto& monede = piata.obtine_monede();
        for (const auto& moneda : monede) {
            std::cout << "  " << moneda->obtine_simbol() << " - "
                      << moneda->obtine_nume() << "\n";
        }
    } else {
        piata.afiseaza_piata();
    }

    if (piata.obtine_monede().empty()) {
        afiseaza_eroare("Piata este goala, nu se pot efectua cumparari.");
        asteapta_enter();
        return;
    }

    std::cout << "\n=== INTRODUCETI DATELE DE CUMPARARE ===\n";

    std::string simbol = CititorFisier::citeste_input<std::string>("Introdu Simbolul monedei (ex: BTC, ETH): ");

    if (simbol.empty()) {
         afiseaza_eroare("Simbolul nu poate fi gol!");
         asteapta_enter();
         return;
    }

    std::transform(simbol.begin(), simbol.end(), simbol.begin(), ::toupper);

    const auto* moneda = piata.cauta_moneda(simbol);
    if (!moneda) {
        afiseaza_eroare("Moneda cu simbolul " + simbol + " nu exista in piata.");
        asteapta_enter();
        return;
    }

    double cantitate = CititorFisier::citeste_input<double>("Introdu Cantitatea dorita (ex: 0.5): ");

    if (cantitate <= 0) {
        afiseaza_eroare("Cantitatea trebuie sa fie pozitiva!");
        asteapta_enter();
        return;
    }

    double pret = moneda->obtine_pret();
    double suma_necesara = cantitate * pret;
    double balanta = utilizator_curent->obtine_balanta_ron();

    std::cout << "\n=== REZUMAT TRANZACTIE ===\n";
    std::cout << "Moneda: " << moneda->obtine_nume() << " (" << simbol << ")\n";
    std::cout << "Pret unitar: " << pret << " RON\n";
    std::cout << "Cantitate: " << cantitate << "\n";
    std::cout << "Total de plata: " << suma_necesara << " RON\n";
    std::cout << "Balanta ta disponibila: " << balanta << " RON\n";

    if (suma_necesara > balanta) {
        afiseaza_eroare("Fonduri insuficiente! Ai nevoie de " +
                       std::to_string(suma_necesara) + " RON, dar ai doar " +
                       std::to_string(balanta) + " RON.");
        asteapta_enter();
        return;
    }

    if (este_ci) {
        std::cout << "[CI] Confirmare automata pentru cumparare\n";
    } else {
        std::string confirmare = CititorFisier::citeste_input<std::string>("Confirmi cumpararea? (da/nu): ");
        std::transform(confirmare.begin(), confirmare.end(), confirmare.begin(), ::tolower);

        if (!(confirmare == "da" || confirmare == "d" || confirmare == "1")) {
            afiseaza_mesaj("Cumparare anulata.");
            asteapta_enter();
            return;
        }
    }

    try {
        bool succes = utilizator_curent->cumpara(moneda, suma_necesara);
        if (succes) {
            manager_utilizatori.salveaza_toti_utilizatorii();
            afiseaza_mesaj("Cumparare reusita! Ai cumparat " +
                          std::to_string(cantitate) + " " + simbol);
        } else {
            afiseaza_eroare("Cumpararea a esuat dintr-un motiv necunoscut.");
        }
    } catch (const std::exception& e) {
        afiseaza_eroare(e.what());
    }

    asteapta_enter();
}

void InterfataTerminal::proceseaza_depunere() {
    curata_ecran();
    afiseaza_antet("DEPUNERE RON");

    double suma = CititorFisier::citeste_input<double>("Suma de depus (RON): ");

    try {
        utilizator_curent->depune_ron(suma);
        manager_utilizatori.salveaza_toti_utilizatorii();
        afiseaza_mesaj("Depunere reusita!");
    } catch (const std::exception& e) {
        afiseaza_eroare(e.what());
    }

    asteapta_enter();
}

void InterfataTerminal::gestioneaza_portofel() {
    curata_ecran();
    afiseaza_antet("PORTOFEL");

    utilizator_curent->obtine_portofel().afiseaza_portofel();

    std::cout << "\n=== OPTIUNI ===\n";
    std::cout << "1. Afiseaza tranzactii\n";
    std::cout << "2. Afiseaza statistici\n";
    std::cout << "3. Inapoi\n";
    std::cout << "\nAlegeti o optiune: ";

    int optiune = citeste_optiune(1, 3);

    if (optiune == 1) {
        utilizator_curent->obtine_portofel().afiseaza_tranzactii();
        asteapta_enter();
        gestioneaza_portofel();
    } else if (optiune == 2) {
        utilizator_curent->obtine_portofel().afiseaza_statistici();
        asteapta_enter();
        gestioneaza_portofel();
    }
}

void InterfataTerminal::afiseaza_meniu_statistici() {
    curata_ecran();
    afiseaza_antet("STATISTICI PIATA");

    std::vector<std::shared_ptr<Moneda>> monede_snapshot;
    const auto& monede_curente = piata.obtine_monede();

    if (monede_curente.empty()) {
        afiseaza_eroare("Nu exista monede in piata pentru analiza.");
        asteapta_enter();
        return;
    }

    for (const auto& moneda : monede_curente) {
        monede_snapshot.push_back(std::shared_ptr<Moneda>(moneda->cloneaza()));
    }

    analizator = AnalizatorPiata(monede_snapshot);

    analizator.afiseaza_statistici();

    std::cout << "\n=== OPTIUNI ===\n";
    std::cout << "1. Afiseaza monede volatile\n";
    std::cout << "2. Inapoi\n";
    std::cout << "\nAlegeti o optiune: ";

    int optiune = citeste_optiune(1, 2);

    if (optiune == 1) {
        bool este_ci = (std::getenv("CI") != nullptr);
        if (este_ci) {
            analizator.afiseaza_monede_volatile(5.0);
        } else {
            double prag = CititorFisier::citeste_input<double>("Introdu pragul de volatilitate (%): ");
            analizator.afiseaza_monede_volatile(prag);
        }
        asteapta_enter();
        afiseaza_meniu_statistici();
    }
}

void InterfataTerminal::curata_ecran() {
    bool este_ci = (std::getenv("CI") != nullptr);
    if (!este_ci) {
        system(CURATA_ECRAN);
    }
}

void InterfataTerminal::afiseaza_antet(const std::string& titlu) {
    std::cout << "================================================\n";
    std::cout << "               " << titlu << "\n";
    std::cout << "================================================\n";
}

void InterfataTerminal::afiseaza_mesaj(const std::string& mesaj) {
    std::cout << "\n[SUCCES] " << mesaj << "\n";
}

void InterfataTerminal::afiseaza_eroare(const std::string& eroare) {
    std::cout << "\n[EROARE] " << eroare << "\n";
}

void InterfataTerminal::asteapta_enter() {
    bool este_ci = (std::getenv("CI") != nullptr);
    if (este_ci || CititorFisier::is_test_mode()) {
        std::cout << "[AUTO] Continuare...\n";
        return;
    }

    std::cout << "\nApasati Enter pentru a continua...";

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int InterfataTerminal::citeste_optiune(int min, int max) {
    bool este_ci = (std::getenv("CI") != nullptr);

    while (true) {
        try {
            int optiune = CititorFisier::citeste_input<int>("");

            if (optiune >= min && optiune <= max) {
                return optiune;
            } else if (!este_ci) {
                std::cout << "Optiune invalida. Alegeti intre " << min << " si " << max << ": ";
            }
        } catch (...) {
            if (este_ci) {
                std::cout << "[CI] Eroare la citire, folosesc optiunea " << min << "\n";
                return min;
            } else {
                std::cout << "Input invalid. Incearca din nou: ";
            }
        }
    }
}

void InterfataTerminal::opreste() {
    ruleaza = false;
    if (utilizator_curent) {
        manager_utilizatori.salveaza_toti_utilizatorii();
    }
    piata.opreste_actualizare();
}
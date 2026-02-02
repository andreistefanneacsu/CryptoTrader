#include "Utilitare/InterfataTerminal.hpp"
#include "Utilitare/CititorFisier.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <thread>
#include <filesystem>
#include <clocale>
#include "tests/TestRunner.hpp"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "========================================\n";
    std::cout << "  TESTARE AUTOMATA CRYPTO TRADER++\n";
    std::cout << "  (pentru a face Cppcheck să tacă)\n";
    std::cout << "========================================\n\n";

    TestRunner::ruleaza_toate_testele();

    bool este_ci = std::getenv("CI") != nullptr;

    if (este_ci) {
        std::cout << "========================================================\n";
        std::cout << "  RULARE IN MOD CI/CD - CITIRE DIN FISIER\n";
        std::cout << "========================================================\n\n";

        CititorFisier::seteaza_mod_testare(true);
    } else {
        std::cout << "========================================================\n";
        std::cout << "                 CRYPTO TRADER++ v1.0                 \n";
        std::cout << "       Sistem Avansat de Trading Criptomonede           \n";
        std::cout << "========================================================\n\n";
    }

    std::cout << "Initializare aplicatie...\n";
    std::cout.flush();

    try {
        std::filesystem::create_directories("date/utilizatori");
        std::filesystem::create_directories("date/piata");
        std::filesystem::create_directories("date/logs");

        if (este_ci || std::filesystem::exists("tastatura.txt")) {
            CititorFisier::incarca_comenzi_din_fisier("tastatura.txt");
        }

        InterfataTerminal interfata;
        interfata.porneste();

        if (!este_ci) {
            std::cout << "\n========================================================\n";
            std::cout << "  Aplicatia s-a inchis. Toate datele au fost salvate.\n";
            std::cout << "  Va multumim ca ati folosit Crypto Trader++!\n";
            std::cout << "========================================================\n\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "\nEROARE CRITICA: " << e.what() << "\n";
        std::cerr << "Aplicatia se va inchide.\n";
        return 1;
    }

    return 0;
}

#include "Utilitare/CititorFisier.hpp"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<std::string> CititorFisier::comenzi;
size_t CititorFisier::index_curent = 0;
bool CititorFisier::mod_testare = false;

void CititorFisier::incarca_comenzi_din_fisier(const std::string& cale) {
    comenzi.clear();
    index_curent = 0;

    if (!fs::exists(cale)) {
        mod_testare = false;
        return;
    }

    std::ifstream fisier(cale);
    if (!fisier.is_open()) {
        mod_testare = false;
        return;
    }

    std::string linie;
    while (std::getline(fisier, linie)) {
        if (linie.empty() || linie[0] == '#') {
            continue;
        }

        linie.erase(0, linie.find_first_not_of(" \t"));
        linie.erase(linie.find_last_not_of(" \t") + 1);

        if (!linie.empty()) {
            comenzi.push_back(linie);
        }
    }

    fisier.close();
    mod_testare = !comenzi.empty();

    if (mod_testare) {
        std::cout << "[AUTO] Incarcat " << comenzi.size() << " comenzi\n";
    }
}

void CititorFisier::seteaza_mod_testare(bool testare) {
    mod_testare = testare;
}

bool CititorFisier::citeste_linie(std::string& linie) {
    if (!mod_testare || index_curent >= comenzi.size()) {
        return false;
    }

    linie = comenzi[index_curent++];
    return true;
}

void CititorFisier::reseteaza() {
    index_curent = 0;
}

template<typename T>
T CititorFisier::citeste_input(const std::string& prompt) {
    bool este_ci = (std::getenv("CI") != nullptr);

    if (!este_ci) {
        std::cout << prompt;
    }

    if constexpr (std::is_same_v<T, std::string>) {
        if (mod_testare && index_curent < comenzi.size()) {
            std::string valoare = comenzi[index_curent++];
            if (!este_ci) {
                std::cout << "[AUTO] " << valoare << std::endl;
            }
            return valoare;
        }

        if (std::cin.peek() == '\n') std::cin.ignore();
        std::string valoare;
        std::getline(std::cin, valoare);
        return valoare;

    } else {
        if (mod_testare && index_curent < comenzi.size()) {
            std::string input_str = comenzi[index_curent++];
            std::stringstream ss(input_str);
            T valoare;

            if (!(ss >> valoare)) {
                throw std::runtime_error("Nu pot converti '" + input_str + "' la tipul dorit");
            }

            if (!este_ci) {
                std::cout << "[AUTO] " << valoare << std::endl;
            }
            return valoare;
        }

        T valoare;
        while (!(std::cin >> valoare)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!este_ci) {
                std::cout << "Input invalid. " << prompt;
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return valoare;
    }
}

template int CititorFisier::citeste_input<int>(const std::string&);
template double CititorFisier::citeste_input<double>(const std::string&);
template std::string CititorFisier::citeste_input<std::string>(const std::string&);
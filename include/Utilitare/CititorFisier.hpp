#ifndef CRYPTOTRADER_CITITORFISIER_HPP
#define CRYPTOTRADER_CITITORFISIER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <memory>

class CititorFisier {
private:
    static std::vector<std::string> comenzi;
    static size_t index_curent;
    static bool mod_testare;

public:
    static void incarca_comenzi_din_fisier(const std::string& cale = "tastatura.txt");
    static void seteaza_mod_testare(bool testare);
    static bool citeste_linie(std::string& linie);
    static void reseteaza();

    static bool is_test_mode() { return mod_testare; }

    template<typename T>
    static T citeste_input(const std::string& prompt);
};

#endif
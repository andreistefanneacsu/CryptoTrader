#ifndef CRYPTOTRADER_TESTRUNNER_HPP
#define CRYPTOTRADER_TESTRUNNER_HPP

#include <iostream>
#include <memory>
#include "Monede/Moneda.hpp"
#include "Monede/MonedaDeBaza.hpp"
#include "Monede/MonedaStabila.hpp"
#include "Monede/MonedaMeme.hpp"
#include "Monede/MonedaFiat.hpp"
#include "Monede/Altcoin.hpp"
#include "Piata/Piata.hpp"
#include "Piata/AnalizatorPiata.hpp"
#include "Utilizator/Utilizator.hpp"
#include "Utilizator/Portofel.hpp"
#include "Utilizator/ManagerUtilizatori.hpp"
#include "Tranzactii/Tranzactie.hpp"
#include "Utilitare/ManagerFisiere.hpp"
#include "Sabloane/ContainerSablon.hpp"
#include "Exceptii/Exceptii.hpp"

class TestRunner {
private:
    static int teste_trecute;
    static int teste_total;

    static void verifica(bool conditie, const std::string& mesaj) {
        teste_total++;
        if (conditie) {
            teste_trecute++;
            std::cout << "✓ " << mesaj << "\n";
        } else {
            std::cout << "✗ " << mesaj << "\n";
        }
    }

public:
    static void ruleaza_toate_testele();
    static void afiseaza_rezultate();

    static void testeaza_exceptii();
    static void testeaza_monede();
    static void testeaza_piata();
    static void testeaza_analizator_piata();
    static void testeaza_utilizator();
    static void testeaza_portofel();
    static void testeaza_manager_utilizatori();
    static void testeaza_tranzactii();
    static void testeaza_manager_fisiere();
    static void testeaza_container_sablon();
    static void testeaza_functii_nefolosite();
};

#endif
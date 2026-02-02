#ifndef CRYPTOTRADER_INTERFATATERMINAL_HPP
#define CRYPTOTRADER_INTERFATATERMINAL_HPP

#include <memory>
#include <atomic>
#include <thread>
#include <functional>
#include "Piata/Piata.hpp"
#include "Utilizator/ManagerUtilizatori.hpp"
#include "Piata/AnalizatorPiata.hpp"

class InterfataTerminal {
private:
    Piata& piata;
    ManagerUtilizatori& manager_utilizatori;
    std::shared_ptr<Utilizator> utilizator_curent;
    std::atomic<bool> ruleaza;
    bool este_admin;
    AnalizatorPiata analizator;

    void afiseaza_meniu_autentificare();
    void afiseaza_meniu_utilizator();
    void afiseaza_meniu_piata();
    void afiseaza_meniu_statistici();
    void afiseaza_meniu_admin();

    void proceseaza_cumparare();
    void proceseaza_vanzare();
    void proceseaza_depunere();
    void proceseaza_retragere();
    void afiseaza_detalii_moneda();
    void gestioneaza_portofel();

    void curata_ecran();
    void afiseaza_antet(const std::string& titlu);
    void afiseaza_mesaj(const std::string& mesaj);
    void afiseaza_eroare(const std::string& eroare);
    void asteapta_enter();

    int citeste_optiune(int min, int max);
    
public:
    InterfataTerminal();
    ~InterfataTerminal();
    
    void porneste();
    void opreste();
};

#endif
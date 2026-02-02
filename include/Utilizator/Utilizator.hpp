#ifndef CRYPTOTRADER_UTILIZATOR_HPP
#define CRYPTOTRADER_UTILIZATOR_HPP

#include <string>
#include <memory>
#include <iostream>
#include "Portofel.hpp"
#include "Exceptii/Exceptii.hpp"

class Utilizator {
private:
    std::string nume;
    std::string email;
    std::string parola_hash;
    Portofel portofel;
    double balanta_ron;
    std::string id_utilizator;
    std::time_t data_inregistrare;

    static std::string genereaza_id();
    std::string hash_parola(const std::string& parola) const;
    bool verifica_parola(const std::string& parola) const;

public:
    Utilizator();
    Utilizator(const std::string& nume, const std::string& email,
               const std::string& parola, double balanta_initiala = 1000.0);

    Utilizator(const std::string& id, const std::string& nume,
               const std::string& email, const std::string& hash_existenta,
               double balanta);

    Utilizator(const Utilizator& alta);
    Utilizator& operator=(Utilizator alta);
    ~Utilizator() = default;

    void interschimba(Utilizator& alta) noexcept;

    const std::string& obtine_nume() const { return nume; }
    const std::string& obtine_email() const { return email; }
    const std::string& obtine_id() const { return id_utilizator; }
    const std::string& obtine_parola_hash() const { return parola_hash; }

    double obtine_balanta_ron() const { return balanta_ron; }
    const Portofel& obtine_portofel() const { return portofel; }
    Portofel& obtine_portofel() { return portofel; }
    std::time_t obtine_data_inregistrare() const { return data_inregistrare; }

    bool autentificare(const std::string& parola) const;

    bool cumpara(const Moneda* moneda, double suma_ron);
    bool vinde(const std::string& simbol_moneda, double cantitate);

    void depune_ron(double suma);
    bool retrage_ron(double suma);

    void afiseaza_profil(std::ostream& os = std::cout) const;
    void afiseaza_statistici(std::ostream& os = std::cout) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Utilizator& utilizator);
};

#endif
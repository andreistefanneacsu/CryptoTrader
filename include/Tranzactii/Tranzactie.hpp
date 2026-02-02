#ifndef CRYPTOTRADER_TRANZACTIE_HPP
#define CRYPTOTRADER_TRANZACTIE_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include "Monede/Moneda.hpp"

enum class StatusTranzactie {
    SUCCES,
    EROARE_FONDURI_INSUFICIENTE,
    EROARE_MONEDA_INEXISTENTA,
    EROARE_CANTITATE_INVALIDA
};

class Tranzactie {
private:
    [[maybe_unused]] const Moneda* moneda;

    std::string simbol_cache;
    std::string tip;
    double pret;
    double cantitate;
    StatusTranzactie status;
    std::time_t timestamp;
    std::string id_tranzactie;

    static std::string genereaza_id();

public:
    Tranzactie();
    Tranzactie(const Moneda* moneda, const std::string& tip,
               double pret, double cantitate, StatusTranzactie status);

    Tranzactie(const std::string& id, long long time, const std::string& simbol,
               const std::string& tip, double pret, double cantitate, int status_int);

    const std::string& obtine_simbol() const { return simbol_cache; }
    const std::string& obtine_tip() const { return tip; }
    double obtine_pret() const { return pret; }
    double obtine_cantitate() const { return cantitate; }
    StatusTranzactie obtine_status() const { return status; }
    std::time_t obtine_timestamp() const { return timestamp; }
    const std::string& obtine_id() const { return id_tranzactie; }

    double calculeaza_valoare() const { return pret * cantitate; }

    std::string serializeaza() const;
    std::string obtine_status_text() const;

    friend std::ostream& operator<<(std::ostream& os, const Tranzactie& tranzactie);
};

#endif
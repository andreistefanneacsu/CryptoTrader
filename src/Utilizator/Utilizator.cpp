#include "Utilizator/Utilizator.hpp"
#include <iostream>
#include <iomanip>
#include <atomic>
#include <ctime>
#include <algorithm>

static std::atomic<int> contor_utilizatori{500000};

std::string Utilizator::genereaza_id() {
    int id = contor_utilizatori.fetch_add(1);
    return "USER" + std::to_string(id);
}

std::string Utilizator::hash_parola(const std::string& parola) const {
    std::hash<std::string> hasher;
    return std::to_string(hasher(parola + "salt_crypto_trader"));
}

bool Utilizator::verifica_parola(const std::string& parola) const {
    return parola_hash == hash_parola(parola);
}

Utilizator::Utilizator()
    : nume(""), email(""), parola_hash(""), balanta_ron(0.0),
      id_utilizator(genereaza_id()),
      data_inregistrare(std::time(nullptr)) {
}

Utilizator::Utilizator(const std::string& nume, const std::string& email,
                       const std::string& parola, double balanta_initiala)
    : nume(nume), email(email), balanta_ron(balanta_initiala),
      id_utilizator(genereaza_id()),
      data_inregistrare(std::time(nullptr)) {

    parola_hash = hash_parola(parola);
}

Utilizator::Utilizator(const std::string& id, const std::string& nume,
                       const std::string& email, const std::string& hash_existenta,
                       double balanta)
    : nume(nume), email(email), parola_hash(hash_existenta), balanta_ron(balanta),
      id_utilizator(id), data_inregistrare(std::time(nullptr)) {
}

Utilizator::Utilizator(const Utilizator& alta)
    : nume(alta.nume), email(alta.email), parola_hash(alta.parola_hash),
      portofel(alta.portofel), balanta_ron(alta.balanta_ron),
      id_utilizator(alta.id_utilizator), data_inregistrare(alta.data_inregistrare) {}

void Utilizator::interschimba(Utilizator& alta) noexcept {
    using std::swap;
    swap(nume, alta.nume);
    swap(email, alta.email);
    swap(parola_hash, alta.parola_hash);
    swap(portofel, alta.portofel);
    swap(balanta_ron, alta.balanta_ron);
    swap(id_utilizator, alta.id_utilizator);
    swap(data_inregistrare, alta.data_inregistrare);
}

Utilizator& Utilizator::operator=(Utilizator alta) {
    interschimba(alta);
    return *this;
}

bool Utilizator::autentificare(const std::string& parola) const {
    return verifica_parola(parola);
}

bool Utilizator::cumpara(const Moneda* moneda, double suma_ron) {
    if (!moneda || suma_ron <= 0) {
        throw ExceptieTranzactie("Parametri invalizi pentru cumpărare");
    }

    if (balanta_ron < suma_ron) {
        throw ExceptieFonduriInsuficiente(suma_ron, balanta_ron);
    }

    double cantitate = suma_ron / moneda->obtine_pret();
    if (cantitate <= 0) {
        throw ExceptieTranzactie("Cantitate invalidă calculată");
    }

    bool succes = portofel.adauga_moneda(moneda, cantitate, moneda->obtine_pret());
    if (succes) {
        balanta_ron -= suma_ron;
    }

    return succes;
}

bool Utilizator::vinde(const std::string& simbol_moneda, double cantitate) {
    if (cantitate <= 0) {
        throw ExceptieTranzactie("Cantitate invalidă pentru vânzare");
    }

    double pret_curent = 0.0;

    bool gasit = false;
    for (const auto& pozitie : portofel.obtine_pozitii()) {
        if (pozitie.moneda->obtine_simbol() == simbol_moneda) {
            pret_curent = pozitie.moneda->obtine_pret();
            gasit = true;
            break;
        }
    }

    if (!gasit) {
        throw ExceptieMonedaInexistenta(simbol_moneda);
    }

    bool succes = portofel.vinde_moneda(simbol_moneda, cantitate, pret_curent);
    if (succes) {
        balanta_ron += cantitate * pret_curent;
    }

    return succes;
}

void Utilizator::depune_ron(double suma) {
    if (suma <= 0) {
        throw ExceptieTranzactie("Suma depozitului trebuie să fie pozitivă");
    }

    balanta_ron += suma;
    std::cout << "Depunere reușită! Balanță actuală: "
              << balanta_ron << " RON\n";
}

bool Utilizator::retrage_ron(double suma) {
    if (suma <= 0) {
        throw ExceptieTranzactie("Suma retragerii trebuie să fie pozitivă");
    }

    if (balanta_ron < suma) {
        throw ExceptieFonduriInsuficiente(suma, balanta_ron);
    }

    balanta_ron -= suma;
    std::cout << "Retragere reușită! Balanță actuală: "
              << balanta_ron << " RON\n";

    return true;
}

void Utilizator::afiseaza_profil(std::ostream& os) const {
    os << "\n=== PROFIL UTILIZATOR ===\n";
    os << "ID: " << id_utilizator << "\n";
    os << "Nume: " << nume << "\n";
    os << "Email: " << email << "\n";

    const std::tm* timeinfo = std::localtime(&data_inregistrare);

    if (timeinfo) {
        // Corectie: buffer mutat aici pentru scope redus (Cppcheck fix)
        char buffer[80];
        std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
        os << "Înregistrat la: " << buffer << "\n";
    } else {
        os << "Înregistrat la: NECUNOSCUT\n";
    }

    os << "Balanță RON: " << std::fixed << std::setprecision(2)
              << balanta_ron << " RON\n";
}

void Utilizator::afiseaza_statistici(std::ostream& os) const {
    afiseaza_profil(os);
    portofel.afiseaza_statistici(os);
}

std::ostream& operator<<(std::ostream& os, const Utilizator& utilizator) {
    utilizator.afiseaza_profil(os);
    return os;
}
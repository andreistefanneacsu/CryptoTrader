#include "Tranzactii/Tranzactie.hpp"
#include <iomanip>
#include <atomic>
#include <string>

static std::atomic<int> contor_tranzactii{100000};

std::string Tranzactie::genereaza_id() {
    int id = contor_tranzactii.fetch_add(1);
    return "TX" + std::to_string(id);
}

Tranzactie::Tranzactie()
    : moneda(nullptr), simbol_cache("N/A"), tip(""), pret(0.0), cantitate(0.0),
      status(StatusTranzactie::SUCCES), timestamp(std::time(nullptr)),
      id_tranzactie(genereaza_id()) {
}

Tranzactie::Tranzactie(const Moneda* moneda, const std::string& tip,
                       double pret, double cantitate, StatusTranzactie status)
    : moneda(moneda), tip(tip), pret(pret), cantitate(cantitate),
      status(status), timestamp(std::time(nullptr)),
      id_tranzactie(genereaza_id()) {

    if (moneda) {
        simbol_cache = moneda->obtine_simbol();
    } else {
        simbol_cache = "NECUNOSCUT";
    }
}

Tranzactie::Tranzactie(const std::string& id, long long time, const std::string& simbol,
                       const std::string& tip_tranz, double pret_tranz, double cant, int status_int)
    : moneda(nullptr), simbol_cache(simbol), tip(tip_tranz), pret(pret_tranz),
      cantitate(cant), timestamp(static_cast<std::time_t>(time)), id_tranzactie(id) {

    switch(status_int) {
        case 0: status = StatusTranzactie::SUCCES; break;
        case 1: status = StatusTranzactie::EROARE_FONDURI_INSUFICIENTE; break;
        case 2: status = StatusTranzactie::EROARE_MONEDA_INEXISTENTA; break;
        default: status = StatusTranzactie::EROARE_CANTITATE_INVALIDA; break;
    }
}

std::string Tranzactie::serializeaza() const {
    std::string s = id_tranzactie + " " +
                    std::to_string((long long)timestamp) + " " +
                    simbol_cache + " " +
                    tip + " " +
                    std::to_string(pret) + " " +
                    std::to_string(cantitate) + " " +
                    std::to_string((int)status);
    return s;
}

std::string Tranzactie::obtine_status_text() const {
    switch (status) {
        case StatusTranzactie::SUCCES: return "SUCCES";
        case StatusTranzactie::EROARE_FONDURI_INSUFICIENTE: return "FONDURI_INSUF";
        case StatusTranzactie::EROARE_MONEDA_INEXISTENTA: return "MONEDA_INEX";
        case StatusTranzactie::EROARE_CANTITATE_INVALIDA: return "CANTITATE_INV";
        default: return "NECUNOSCUT";
    }
}

std::ostream& operator<<(std::ostream& os, const Tranzactie& tranzactie) {
    const std::tm* timeinfo = std::localtime(&tranzactie.timestamp);
    char buffer[80];
    if (timeinfo) {
        std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    } else {
        // Fallback
        buffer[0] = '?'; buffer[1] = '\0';
    }

    os << "[" << tranzactie.id_tranzactie << "] "
       << buffer << " | "
       << tranzactie.tip << " "
       << tranzactie.simbol_cache
       << " | Cant: " << tranzactie.cantitate
       << " | Pret: " << tranzactie.pret << " RON"
       << " | Status: " << tranzactie.obtine_status_text();
    
    return os;
}
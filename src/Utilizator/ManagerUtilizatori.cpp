#include "Utilizator/ManagerUtilizatori.hpp"
#include "Piata/Piata.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <locale>
#include <algorithm>

namespace fs = std::filesystem;

ManagerUtilizatori::ManagerUtilizatori()
    : director_date("date/utilizatori/") {

    if (!fs::exists(director_date)) {
        fs::create_directories(director_date);
    }

    if (!fs::exists("date/tranzactii")) {
        fs::create_directories("date/tranzactii");
    }

    incarca_utilizatori();
}

ManagerUtilizatori::~ManagerUtilizatori() {
    salveaza_toti_utilizatorii();
}

std::string ManagerUtilizatori::obtine_cale_fisier(const std::string& id_utilizator) const {
    return director_date + id_utilizator + ".dat";
}

void ManagerUtilizatori::incarca_utilizatori() {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);

    if (!fs::exists(director_date)) return;

    for (const auto& entry : fs::directory_iterator(director_date)) {
        if (entry.path().extension() == ".dat") {
            try {
                std::ifstream fisier(entry.path());
                fisier.imbue(std::locale::classic());

                if (fisier.is_open()) {
                    std::string id, nume, email, hash;
                    double balanta;
                    std::getline(fisier, id);
                    std::getline(fisier, nume);
                    std::getline(fisier, email);
                    std::getline(fisier, hash);
                    fisier >> balanta;

                    auto user = std::make_shared<Utilizator>(id, nume, email, hash, balanta);
                    utilizatori[nume] = user;
                }
                fisier.close();
            } catch (...) {
                continue;
            }
        }
    }
}

void ManagerUtilizatori::salveaza_utilizator(const std::shared_ptr<Utilizator>& utilizator) {
    if (!utilizator) return;

    std::string cale = obtine_cale_fisier(utilizator->obtine_id());
    std::ofstream fisier(cale);
    fisier.imbue(std::locale::classic());

    if (fisier.is_open()) {
        fisier << utilizator->obtine_id() << "\n";
        fisier << utilizator->obtine_nume() << "\n";
        fisier << utilizator->obtine_email() << "\n";
        fisier << utilizator->obtine_parola_hash() << "\n";
        fisier << std::fixed << std::setprecision(2) << utilizator->obtine_balanta_ron() << "\n";
        fisier.close();
    }
}

std::shared_ptr<Utilizator> ManagerUtilizatori::autentificare(const std::string& nume,
                                          const std::string& parola) {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);

    auto it = utilizatori.find(nume);
    if (it != utilizatori.end()) {
        if (it->second->autentificare(parola)) {
            utilizator_curent = it->second;
            return utilizator_curent;
        }
    }
    return nullptr;
}

std::shared_ptr<Utilizator> ManagerUtilizatori::inregistrare(const std::string& nume,
                                         const std::string& email,
                                         const std::string& parola,
                                         double balanta_initiala) {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);

    if (utilizatori.find(nume) != utilizatori.end()) {
        throw ExceptieUtilizator("Utilizatorul există deja!");
    }

    auto nou_utilizator = std::make_shared<Utilizator>(nume, email, parola, balanta_initiala);
    utilizatori[nume] = nou_utilizator;
    utilizator_curent = nou_utilizator;

    salveaza_utilizator(nou_utilizator);
    return nou_utilizator;
}

bool ManagerUtilizatori::utilizator_exista(const std::string& nume) const {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);
    return utilizatori.find(nume) != utilizatori.end();
}

/* // FUNCȚII NEUTILIZATE COMENTATE PENTRU A EVITA CONFLICTELE CU HEADER-UL

void ManagerUtilizatori::deconectare() {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);
    utilizator_curent = nullptr;
}

void ManagerUtilizatori::sterge_utilizator(const std::string& id_utilizator) {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);

    auto it = std::find_if(utilizatori.begin(), utilizatori.end(),
        [&](const auto& pair) { return pair.second->obtine_id() == id_utilizator; });

    if (it != utilizatori.end()) {
        std::string cale = obtine_cale_fisier(id_utilizator);
        fs::remove(cale);
        utilizatori.erase(it);
    }
}
*/

void ManagerUtilizatori::salveaza_toti_utilizatorii() {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);
    for (const auto& [nume, utilizator] : utilizatori) {
        salveaza_utilizator(utilizator);
    }
}

void ManagerUtilizatori::afiseaza_toti_utilizatorii() const {
    std::lock_guard<std::mutex> blocare(mutex_utilizatori);

    std::cout << "\n=== TOTI UTILIZATORII INREGISTRATI ===\n";

    if (utilizatori.empty()) {
        std::cout << "Nu exista utilizatori inregistrati.\n";
        return;
    }

    std::cout << std::left << std::setw(15) << "ID"
              << std::setw(20) << "Nume"
              << std::setw(30) << "Email"
              << std::setw(15) << "Balanta RON" << "\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& [nume, utilizator] : utilizatori) {
        std::cout << std::left << std::setw(15) << utilizator->obtine_id()
                  << std::setw(20) << utilizator->obtine_nume()
                  << std::setw(30) << utilizator->obtine_email()
                  << std::setw(15) << std::fixed << std::setprecision(2) << utilizator->obtine_balanta_ron() << "\n";
    }
}
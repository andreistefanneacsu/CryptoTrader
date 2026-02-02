#include "Utilitare/ManagerFisiere.hpp"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

bool ManagerFisiere::fisier_exista(const std::string& cale) {
    return fs::exists(cale) && fs::is_regular_file(cale);
}

bool ManagerFisiere::director_exista(const std::string& cale) {
    return fs::exists(cale) && fs::is_directory(cale);
}

void ManagerFisiere::creeaza_director(const std::string& cale) {
    if (!director_exista(cale)) {
        fs::create_directories(cale);
    }
}

std::string ManagerFisiere::citeste_fisier(const std::string& cale) {
    if (!fisier_exista(cale)) {
        return "";
    }
    
    std::ifstream fisier(cale);
    if (!fisier.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << fisier.rdbuf();
    return buffer.str();
}

void ManagerFisiere::scrie_fisier(const std::string& cale, const std::string& continut) {
    fs::path cale_path(cale);
    if (cale_path.has_parent_path()) {
        creeaza_director(cale_path.parent_path().string());
    }
    
    std::ofstream fisier(cale);
    if (fisier.is_open()) {
        fisier << continut;
    }
}

void ManagerFisiere::adauga_in_fisier(const std::string& cale, const std::string& continut) {
    fs::path cale_path(cale);
    if (cale_path.has_parent_path()) {
        creeaza_director(cale_path.parent_path().string());
    }
    
    std::ofstream fisier(cale, std::ios::app);
    if (fisier.is_open()) {
        fisier << continut << "\n";
    }
}

std::vector<std::string> ManagerFisiere::listeaza_fisiere(const std::string& director) {
    std::vector<std::string> fisiere;
    
    if (!director_exista(director)) {
        return fisiere;
    }
    
    for (const auto& entry : fs::directory_iterator(director)) {
        if (entry.is_regular_file()) {
            fisiere.push_back(entry.path().filename().string());
        }
    }
    
    return fisiere;
}

void ManagerFisiere::salveaza_date_utilizator(const std::string& id_utilizator, 
                                              const std::string& date) {
    std::string cale = "date/utilizatori/" + id_utilizator + ".dat";
    scrie_fisier(cale, date);
}

std::string ManagerFisiere::incarca_date_utilizator(const std::string& id_utilizator) {
    std::string cale = "date/utilizatori/" + id_utilizator + ".dat";
    return citeste_fisier(cale);
}

void ManagerFisiere::salveaza_date_piata(const std::string& date) {
    scrie_fisier("date/piata/piata.dat", date);
}

std::string ManagerFisiere::incarca_date_piata() {
    return citeste_fisier("date/piata/piata.dat");
}

void ManagerFisiere::salveaza_log(const std::string& mesaj) {
    std::string dir_logs = "date/logs";
    if (!fs::exists(dir_logs)) {
        fs::create_directories(dir_logs);
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");

    std::string log_entry = "[" + ss.str() + "] " + mesaj;

    std::ofstream fisier(dir_logs + "/aplicatie.log", std::ios::app);
    if (fisier.is_open()) {
        fisier << log_entry << "\n";
    }
}
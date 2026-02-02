#ifndef CRYPTOTRADER_MANAGERFISIERE_HPP
#define CRYPTOTRADER_MANAGERFISIERE_HPP

#include <string>
#include <vector>
#include <memory>
#include <fstream>

class ManagerFisiere {
public:
    static bool fisier_exista(const std::string& cale);
    static bool director_exista(const std::string& cale);
    static void creeaza_director(const std::string& cale);
    
    static std::string citeste_fisier(const std::string& cale);
    static void scrie_fisier(const std::string& cale, const std::string& continut);
    static void adauga_in_fisier(const std::string& cale, const std::string& continut);
    
    static std::vector<std::string> listeaza_fisiere(const std::string& director);
    static std::vector<std::string> listeaza_directoare(const std::string& director);
    
    static bool sterge_fisier(const std::string& cale);
    static bool sterge_director(const std::string& cale);
    
    static size_t obtine_marime_fisier(const std::string& cale);
    static std::string obtine_extensie(const std::string& cale);
    
    static void salveaza_date_utilizator(const std::string& id_utilizator, 
                                         const std::string& date);
    static std::string incarca_date_utilizator(const std::string& id_utilizator);
    
    static void salveaza_date_piata(const std::string& date);
    static std::string incarca_date_piata();
    
    static void salveaza_tranzactie(const std::string& id_tranzactie, 
                                    const std::string& date);
    static std::vector<std::string> incarca_tranzactii_utilizator(
        const std::string& id_utilizator);
    
    static void salveaza_log(const std::string& mesaj);
};

#endif
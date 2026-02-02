#ifndef CRYPTOTRADER_MANAGERUTILIZATORI_HPP
#define CRYPTOTRADER_MANAGERUTILIZATORI_HPP

#include <map>
#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include "Sabloane/SingletonSablon.hpp"
#include "Utilizator/Utilizator.hpp"
#include "Utilitare/ManagerFisiere.hpp"

class ManagerUtilizatori : public SingletonSablon<ManagerUtilizatori> {
private:
    std::map<std::string, std::shared_ptr<Utilizator>> utilizatori;
    mutable std::mutex mutex_utilizatori;
    std::string director_date;
    std::shared_ptr<Utilizator> utilizator_curent;
    
    ManagerUtilizatori();
    ~ManagerUtilizatori() override;
    
    void incarca_utilizatori();
    void salveaza_utilizator(const std::shared_ptr<Utilizator>& utilizator);
    std::string obtine_cale_fisier(const std::string& id_utilizator) const;

public:
    std::shared_ptr<Utilizator> autentificare(const std::string& nume, 
                                              const std::string& parola);
    std::shared_ptr<Utilizator> inregistrare(const std::string& nume, 
                                             const std::string& email,
                                             const std::string& parola,
                                             double balanta_initiala = 1000.0);
    
    bool utilizator_exista(const std::string& nume) const;

    // void deconectare();

    std::shared_ptr<Utilizator> obtine_utilizator_curent() const {
        return utilizator_curent;
    }

    void salveaza_toti_utilizatorii();
    void afiseaza_toti_utilizatorii() const;

    // void sterge_utilizator(const std::string& id_utilizator);
    
    friend class SingletonSablon<ManagerUtilizatori>;
};

#endif
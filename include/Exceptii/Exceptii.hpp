#ifndef CRYPTOTRADER_EXCEPTII_HPP
#define CRYPTOTRADER_EXCEPTII_HPP

#include <exception>
#include <string>

class ExceptieCrypto : public std::exception {
protected:
    std::string mesaj;
    
public:
    explicit ExceptieCrypto(const std::string& mesaj) : mesaj(mesaj) {}
    virtual ~ExceptieCrypto() = default;
    
    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class ExceptieTranzactie : public ExceptieCrypto {
public:
    explicit ExceptieTranzactie(const std::string& mesaj) 
        : ExceptieCrypto("Eroare tranzacție: " + mesaj) {}
};

class ExceptieFonduriInsuficiente : public ExceptieTranzactie {
private:
    double cerut;
    double disponibil;
    
public:
    ExceptieFonduriInsuficiente(double cerut, double disponibil)
        : ExceptieTranzactie("Fonduri insuficiente"),
          cerut(cerut), disponibil(disponibil) {
        mesaj += " - Cerut: " + std::to_string(cerut) + 
                ", Disponibil: " + std::to_string(disponibil);
    }
    
    double obtine_cerut() const { return cerut; }
    double obtine_disponibil() const { return disponibil; }
};

class ExceptieMonedaInexistenta : public ExceptieTranzactie {
private:
    std::string simbol;
    
public:
    explicit ExceptieMonedaInexistenta(const std::string& simbol)
        : ExceptieTranzactie("Monedă inexistentă"),
          simbol(simbol) {
        mesaj += " - Simbol: " + simbol;
    }
    
    const std::string& obtine_simbol() const { return simbol; }
};

class ExceptieUtilizator : public ExceptieCrypto {
public:
    explicit ExceptieUtilizator(const std::string& mesaj)
        : ExceptieCrypto("Eroare utilizator: " + mesaj) {}
};

class ExceptiePiata : public ExceptieCrypto {
public:
    explicit ExceptiePiata(const std::string& mesaj)
        : ExceptieCrypto("Eroare piață: " + mesaj) {}
};

class ExceptieSistemFisiere : public ExceptieCrypto {
public:
    explicit ExceptieSistemFisiere(const std::string& mesaj)
        : ExceptieCrypto("Eroare sistem fișiere: " + mesaj) {}
};

#endif
#ifndef CRYPTOTRADER___EXCEPTIONS_HPP
#define CRYPTOTRADER___EXCEPTIONS_HPP

#include <iostream>
#include <string>
#include <exception>

class CryptoException : public std::exception {
protected:
    std::string mesaj;

public:
    explicit CryptoException(const std::string& msg) : mesaj(msg) {}
    virtual ~CryptoException() = default;
    
    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class EroareTranzactie : public CryptoException {
public:
    explicit EroareTranzactie(const std::string& msg) : CryptoException("Eroare tranzactie: " + msg) {}
};

class EroareFonduriInsuficiente : public EroareTranzactie {
public:
    explicit EroareFonduriInsuficiente(double cantitate, double disponibil)
        : EroareTranzactie("Fonduri insuficiente. Cerut: " + 
                          std::to_string(cantitate) + ", Disponibil: " + 
                          std::to_string(disponibil)) {}
};

class EroareMonedaInexistenta : public EroareTranzactie {
public:
    explicit EroareMonedaInexistenta(const std::string& simbol)
        : EroareTranzactie("Moneda cu simbolul '" + simbol + "' nu exista") {}
};

#endif
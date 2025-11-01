#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Moneda {
private:
    string nume;
    string simbol;
    double pret;
    double volatilitate;

public:
    Moneda()
        : nume(""), simbol(""), pret(0), volatilitate(0) {
    }
    Moneda(const string &nume, const string &simbol, const double pret, const double volatilitate)
        : nume(nume), simbol(simbol), pret(pret), volatilitate(volatilitate) {
    }

    Moneda(const Moneda &moneda)
        : nume(moneda.nume), simbol(moneda.simbol), pret(moneda.pret), volatilitate(moneda.volatilitate) {
    }

    ~Moneda() = default;

    void set_nume(const string &nume) {
        this->nume = nume;
    }

    void set_simbol(const string &simbol) {
        this->simbol = simbol;
    }

    void set_pret(const double pret) {
        this->pret = pret;
    }

    void set_volatilitate(const double volatilitate) {
        this->volatilitate = volatilitate;
    }

    [[nodiscard]] string get_nume() const {
        return nume;
    }

    [[nodiscard]] string get_simbol() const {
        return simbol;
    }

    [[nodiscard]] double get_pret() const {
        return pret;
    }

    [[nodiscard]] double get_volatilitate() const {
        return volatilitate;
    }

    void actualizarePret(double procent) {
        this->pret = pret * (1 + procent / 100.0);
    }

    Moneda &operator=(const Moneda &moneda) {
        if (this != &moneda) {  // Self-assignment check
            this->nume = moneda.nume;
            this->simbol = moneda.simbol;
            this->pret = moneda.pret;
            this->volatilitate = moneda.volatilitate;
        }
        return *this;
    }

    friend ostream &operator<<(ostream &os, const Moneda &moneda) {
        os << moneda.nume << " (" << moneda.simbol << "): " << moneda.pret << " EUR";
        return os;
    }
};

class Tranzactie {
private:
    Moneda moneda;
    string tip;
    double cantitate;
    bool status;

public:
    Tranzactie() = default;
    Tranzactie(const Moneda &moneda, const string &tip, const double cantitate, const bool status)
        : moneda(moneda), tip(tip), cantitate(cantitate), status(status) {
    }

    Tranzactie(const Tranzactie &tranzactie)
        : moneda(tranzactie.moneda), tip(tranzactie.tip), cantitate(tranzactie.cantitate), status(tranzactie.status) {
    }

    ~Tranzactie() = default;

    void set_moneda(const Moneda &moneda) {
        this->moneda = moneda;
    }

    void set_tip(const string &tip) {
        this->tip = tip;
    }

    void set_cantitate(const double cantitate) {
        this->cantitate = cantitate;
    }

    void set_status(const bool status) {
        this->status = status;
    }

    [[nodiscard]] Moneda get_moneda() const {
        return moneda;
    }

    [[nodiscard]] string get_tip() const {
        return tip;
    }

    [[nodiscard]] double get_cantitate() const {
        return cantitate;
    }

    [[nodiscard]] bool get_status() const {
        return status;
    }

    double get_valoareTranzactie() const {
        return cantitate * moneda.get_pret();
    }

    friend ostream &operator<<(ostream &os, const Tranzactie &tranzactie) {
        os << "[" << tranzactie.tip << "]: " << tranzactie.moneda.get_simbol() << " " << tranzactie.cantitate << " " << tranzactie.status;
        return os;
    }
};

class Portofel {
private:
    vector<pair<Moneda, double>> monede;
    vector<Tranzactie> tranzactii;

public:
    Portofel() = default;

    Portofel(const vector<Tranzactie> &tranzactii)
        : tranzactii(tranzactii) {
        for (const auto &tranzactie : tranzactii) {
            if (tranzactie.get_status()) {
                bool moneda_gasita = false;
                for (auto &moneda : monede) {
                    if (moneda.first.get_simbol() == tranzactie.get_moneda().get_simbol()) {
                        if (tranzactie.get_tip() == "BUY")
                            moneda.second += tranzactie.get_cantitate();
                        else if (tranzactie.get_tip() == "SELL")
                            moneda.second -= tranzactie.get_cantitate();
                        moneda_gasita = true;
                        break;
                    }
                }
                if (!moneda_gasita && tranzactie.get_tip() == "BUY") {
                    monede.push_back({tranzactie.get_moneda(), tranzactie.get_cantitate()});
                }
            }
        }
    }

    ~Portofel() {
        monede.clear();
        tranzactii.clear();
    }

    bool cumparaMoneda(const Moneda &m, double cantitate) {
        for (auto &moneda : monede) {
            if (moneda.first.get_simbol() == m.get_simbol()) {
                moneda.second += cantitate;
                tranzactii.push_back(Tranzactie(m, "BUY", cantitate, true));
                return true;
            }
        }
        monede.push_back({m, cantitate});
        tranzactii.push_back(Tranzactie(m, "BUY", cantitate, true));
        return true;
    }

    bool vindeMoneda(const Moneda &m, double cantitate) {
        for (auto &moneda : monede) {
            if (moneda.first.get_simbol() == m.get_simbol()) {
                if (moneda.second >= cantitate) {
                    moneda.second -= cantitate;
                    tranzactii.push_back(Tranzactie(m, "SELL", cantitate, true));
                    return true;
                } else {
                    cout << "Tranzactia a esuat intrucat nu ai suficiente monezi!\n";
                    tranzactii.push_back(Tranzactie(m, "SELL", cantitate, false));
                    return false;
                }
            }
        }
        cout << "Tranzactia a esuat intrucat moneda nu este in portofel!\n";
        tranzactii.push_back(Tranzactie(m, "SELL", cantitate, false));
        return false;
    }

    friend ostream &operator<<(ostream &os, const Portofel &portofel) {
        os << "Monede:\n";
        for (const auto &moneda : portofel.monede)
            os << moneda.first << " " << moneda.second << "\n";
        os << "Tranzactii:\n";
        for (const auto &tranzactie : portofel.tranzactii)
            os << tranzactie << "\n";
        return os;
    }
};

class Utilizator {
private:
    string nume;
    Portofel portofel;

public:
    Utilizator() = default;

    Utilizator(const string &nume)
        : nume(nume) {
    }

    Utilizator(const string &nume, const Portofel &portofel)
        : nume(nume), portofel(portofel) {
    }

    ~Utilizator() = default;

    void cumpara(const Moneda &moneda, double cantitate) {
        portofel.cumparaMoneda(moneda, cantitate);
    }

    void vinde(const Moneda &moneda, double cantitate) {
        portofel.vindeMoneda(moneda, cantitate);
    }

    friend ostream &operator<<(ostream &os, const Utilizator &utilizator) {
        os << utilizator.nume << ":\n";
        os << utilizator.portofel;
        return os;
    }
};

int main() {
    Moneda btc("Bitcoin", "BTC", 68000, 5.2);
    Moneda new_btc;
    new_btc = btc;
    btc.actualizarePret(-2.5);
    cout << btc << endl;
    cout << new_btc << endl;

    Tranzactie t1(btc, "BUY", 5, true);
    cout << t1.get_valoareTranzactie() << " EUR" << endl;
    cout << t1 << endl;

    Utilizator user("John Doe");
    user.cumpara(btc, 2);
    cout << user;

    user.vinde(btc, 3);
    cout << user;
    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

inline double calc_pretMediu(double val_totala, double cant_totala) {
    return val_totala / cant_totala;
}

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

    [[nodiscard]] const string& get_simbol() const {
        return simbol;
    }
    [[nodiscard]] const double& get_pret() const {
        return pret;
    }
    [[nodiscard]] double get_volatilitate() const {
        return volatilitate;
    }

    void actualizarePret(double procent) {
        this->pret = pret * (1 + procent / 100.0);
    }

    Moneda &operator=(const Moneda &moneda) {
        if (this != &moneda) {
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
    double pret;
    double cantitate;
    bool status;

public:
    Tranzactie()
        : tip(""), pret(0), cantitate(0), status(false) {
    }
    Tranzactie(const Moneda &moneda, const string &tip, const double pret, const double cantitate, const bool status)
        : moneda(moneda), tip(tip), pret(pret), cantitate(cantitate), status(status) {
    }
    Tranzactie(const Tranzactie &tranzactie)
        : moneda(tranzactie.moneda), tip(tranzactie.tip), pret(tranzactie.pret), cantitate(tranzactie.cantitate), status(tranzactie.status) {
    }
    ~Tranzactie() = default;

    [[nodiscard]] const Moneda& get_moneda() const {
        return moneda;
    }
    [[nodiscard]] const string& get_tip() const {
        return tip;
    }
    [[nodiscard]] double get_pret() const {
        return pret;
    }
    [[nodiscard]] const double& get_cantitate() const {
        return cantitate;
    }
    [[nodiscard]] bool get_status() const {
        return status;
    }

/*    double get_ValoareTranzactie() const {
        return cantitate * pret;
    }*/

    friend ostream &operator<<(ostream &os, const Tranzactie &tranzactie) {
        os << "[" << tranzactie.tip << "]: " << tranzactie.moneda.get_simbol() << " " << tranzactie.cantitate << " " << tranzactie.status;
        return os;
    }
};

class Portofel {
private:
    vector<pair<Moneda,pair<double,double>>> monede;
    vector<Tranzactie> tranzactii;

public:
    Portofel() = default;
    explicit Portofel(const vector<Tranzactie> &tranzactii)
        : tranzactii(tranzactii) {
        for (vector<Tranzactie>::const_iterator tranzactie=tranzactii.begin(); tranzactie!=tranzactii.end(); ++tranzactie) {
            if (tranzactie->get_status()) {
                bool moneda_gasita = false;
                for (vector<pair<Moneda,pair<double,double>>>::iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
                    if (moneda->first.get_simbol() == tranzactie->get_moneda().get_simbol()) {
                        if (tranzactie->get_tip() == "BUY") {
                            moneda->second.second = calc_pretMediu(moneda->second.first*moneda->second.second+tranzactie->get_cantitate()*tranzactie->get_pret(),moneda->second.first+tranzactie->get_cantitate()*tranzactie->get_cantitate());
                            moneda->second.first += tranzactie->get_cantitate();
                        }
                        else if (tranzactie->get_tip() == "SELL") {
                            moneda->second.first -= tranzactie->get_cantitate();
                            if (tranzactie->get_cantitate() == 0) moneda->second.second += 0;
                        }

                        moneda_gasita = true;
                        break;
                    }
                }
                if (!moneda_gasita && tranzactie->get_tip() == "BUY") {
                    monede.push_back({tranzactie->get_moneda(), {tranzactie->get_cantitate(), tranzactie->get_pret()}});
                }
            }
        }
    }
    ~Portofel() {
        monede.clear();
        tranzactii.clear();
    }

    bool cumparaMoneda(const Moneda &m, double cantitate) {
        for (vector<pair<Moneda,pair<double,double>>>::iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            if (moneda->first.get_simbol() == m.get_simbol()) {
                moneda->second.second = calc_pretMediu(moneda->second.first*moneda->second.second+cantitate*m.get_pret(),moneda->second.first+cantitate);
                moneda->second.first += cantitate;
                tranzactii.push_back(Tranzactie(m, "BUY", m.get_pret(), cantitate, true));
                return true;
            }
        }
        monede.push_back({m, {cantitate, m.get_pret()}});
        tranzactii.push_back(Tranzactie(m, "BUY", m.get_pret(), cantitate, true));
        return true;
    }
    bool vindeMoneda(const Moneda &m, double cantitate) {
        for (vector<pair<Moneda,pair<double,double>>>::iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            if (moneda->first.get_simbol() == m.get_simbol()) {
                if (moneda->second.first > cantitate) {
                    moneda->second.first -= cantitate;
                    tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, true));
                    return true;
                }
                else if (moneda->second.first == cantitate) {
                    moneda->second.first = 0;
                    moneda->second.second = 0;
                    tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, true));
                    return true;
                }
                else {
                    cout << "Tranzactia a esuat intrucat nu ai suficiente monezi!\n";
                    tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, false));
                    return false;
                }
            }
        }
        cout << "Tranzactia a esuat intrucat moneda nu este in portofel!\n";
        tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, false));
        return false;
    }

    double get_ValoarePortofel() const {
        double valoarePortofel = 0;
        for (vector<pair<Moneda,pair<double,double>>>::const_iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            valoarePortofel+=moneda->first.get_pret()*moneda->second.first;
        }
        return valoarePortofel;
    }

    double get_ProfitTotal() const {
        double profit = 0;
        for (vector<pair<Moneda,pair<double,double>>>::const_iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            profit+=(moneda->first.get_pret()-moneda->second.second)*moneda->second.first;
        }
        return profit;
    }

    pair <double,double> estimareProfitTotal() const {
        double profit_min=0,profit_max=0;
        for (vector<pair<Moneda,pair<double,double>>>::const_iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            profit_min+=((moneda->first.get_pret()*(1-moneda->first.get_volatilitate()/100.0))-moneda->second.second)*moneda->second.first;
            profit_max=((moneda->first.get_pret()*(1+moneda->first.get_volatilitate()/100.0))-moneda->second.second)*moneda->second.first;
        }
        return {profit_min,profit_max};
    }

    friend ostream &operator<<(ostream &os, const Portofel &portofel) {
        os << "Monede:\n";
        for (vector<pair<Moneda,pair<double,double>>>::const_iterator moneda=portofel.monede.begin(); moneda!=portofel.monede.end(); ++moneda)
            os << moneda->first << " " << moneda->second.first << " " << moneda->second.second << " EUR\n";
        os << "Tranzactii:\n";
        for (vector<Tranzactie>::const_iterator tranzactie=portofel.tranzactii.begin(); tranzactie!=portofel.tranzactii.end(); ++tranzactie)
            os << *tranzactie << "\n";
        return os;
    }
};

class Utilizator {
private:
    string nume;
    Portofel portofel;

public:
    Utilizator() = default;
    explicit Utilizator(const string &nume)
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

    void determinareValoarePortofel() {
        cout<<portofel.get_ValoarePortofel()<<"\n";
    }

    void determinareProfitPosibil() {
        pair<double,double> profit_posibil=portofel.estimareProfitTotal();
        cout<<profit_posibil.first<<" "<<profit_posibil.second<<"\n";
    }

    void determinateProfitTotal() {
        cout<<portofel.get_ProfitTotal()<<"\n";
    }

    friend ostream &operator<<(ostream &os, const Utilizator &utilizator) {
        os << utilizator.nume << ":\n";
        os << utilizator.portofel;
        return os;
    }
};

int main() {
    Moneda bitcoin("Bitcoin", "BTC", 20000, 5.0);
    Moneda ethereum("Ethereum", "ETH", 1500, 7.5);
    Moneda litecoin("Litecoin", "LTC", 100, 3.0);

    Utilizator utilizator("John Doe");

    utilizator.cumpara(bitcoin, 2);

    utilizator.cumpara(ethereum, 10);

    cout << "Dupa achizitie:\n";
    utilizator.determinareValoarePortofel();
    utilizator.determinateProfitTotal();
    utilizator.determinareProfitPosibil();

    utilizator.vinde(bitcoin, 1);

    utilizator.vinde(ethereum, 5);

    bitcoin.actualizarePret(10.5);

    cout << "Dupa vanzare:\n";
    utilizator.determinareValoarePortofel();
    utilizator.determinateProfitTotal();
    utilizator.determinareProfitPosibil();

    utilizator.vinde(litecoin, 5);

    cout << "\nPortofoliu final:\n";
    cout << utilizator;

    return 0;
}

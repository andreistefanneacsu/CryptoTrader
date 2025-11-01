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
    Moneda() =default;
    Moneda(const string &nume, const string &simbol, double pret, double volatilitate)
        : nume(nume),
          simbol(simbol),
          pret(pret),
          volatilitate(volatilitate) {
    }
    Moneda(const Moneda &moneda)
        : nume(moneda.nume),
          simbol(moneda.simbol),
          pret(moneda.pret),
          volatilitate(moneda.volatilitate) {
    }
    ~Moneda() = default;

    void set_nume(const string &nume) {
        this->nume = nume;
    }

    void set_simbol(const string &simbol) {
        this->simbol = simbol;
    }

    void set_pret(double pret) {
        this->pret = pret;
    }

    void set_volatilitate(double volatilitate) {
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

    void actualizarePret (double procent) {
        this->pret = pret*(1+procent/100.0);
    }

    friend ostream& operator << (ostream& os, const Moneda& moneda) {
        os << moneda.nume << " (" << moneda.simbol << ")" << ": " << moneda.pret << "EUR";
        return os;
    }

    Moneda& operator=(const Moneda& moneda) {
        this->nume = moneda.nume;
        this->simbol = moneda.simbol;
        this->pret = moneda.pret;
        this->volatilitate = moneda.volatilitate;
        return *this;
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
    Tranzactie(const Moneda moneda, const string tip, const double cantitate, const bool status)
        : moneda(moneda),
          tip(tip),
          cantitate(cantitate),
          status(status) {
    }

    [[nodiscard]] Moneda get_moneda() const {
        return moneda;
    }

    void set_moneda(const Moneda &moneda) {
        this->moneda = moneda;
    }

    [[nodiscard]] string get_tip() const {
        return tip;
    }

    void set_tip(const string &tip) {
        this->tip = tip;
    }

    [[nodiscard]] double get_cantitate() const {
        return cantitate;
    }

    void set_cantitate(double cantitate) {
        this->cantitate = cantitate;
    }

    double get_valoareTranzactie() const {
        return cantitate*moneda.get_pret();
    }

    [[nodiscard]] bool get_status() const {
        return status;
    }

    void set_status(bool status) {
        this->status = status;
    }

    friend ostream& operator << (ostream& os, const Tranzactie& tranzactie) {
        os << "[" << tranzactie.tip << "]: " << tranzactie.moneda.get_simbol() << " " << tranzactie.cantitate<< " "<< tranzactie.status;
        return os;
    }
};

class Portofel {
private:
    vector <pair<Moneda,double>> monede;
public:
    Portofel() = default;

    Portofel(const vector<Tranzactie>& tranzactii) {
        for (vector<Tranzactie>:: const_iterator tranzactie = tranzactii.begin(); tranzactie != tranzactii.end(); ++tranzactie) {
            if (tranzactie->get_status()==true)
            {
                bool moneda_gasita = false;
                for (vector <pair<Moneda,double>>:: iterator moneda = monede.begin(); moneda != monede.end(); ++moneda) {
                    if (moneda->first.get_simbol()==tranzactie->get_moneda().get_simbol()) {
                        if (tranzactie->get_tip()=="BUY") moneda->second+=tranzactie->get_cantitate();
                        else if (tranzactie->get_tip()=="SELL") moneda->second-=tranzactie->get_cantitate();
                        moneda_gasita = true;
                        break;
                    }
                }
                if (!moneda_gasita&&tranzactie->get_tip()=="BUY") monede.push_back({tranzactie->get_moneda(),tranzactie->get_cantitate()});
            }
        }
    }

    bool cumparaMoneda(const Moneda& m, double cantitate) {
        for (vector <pair<Moneda,double>>:: iterator moneda = monede.begin(); moneda != monede.end(); ++moneda) {
            if (moneda->first.get_simbol()==m.get_simbol()) {
                moneda->second+=cantitate;
                return true;
            }
        }
        monede.push_back({m,cantitate});
        return true;
    }

    bool vindeMoneda(const Moneda& m, double cantitate) {
        for (vector <pair<Moneda,double>>:: iterator moneda = monede.begin(); moneda != monede.end(); ++moneda) {
            if (moneda->first.get_simbol()==m.get_simbol()) {
                if (moneda->second>=cantitate) {
                    moneda->second-=cantitate;
                    return true;
                }
                else {
                    cout << "Tranzactia a esuat intrucat nu ai suficiente monezi!\n";
                    return false;
                }
            }
        }
        cout << "Tranzactia a esuat intrucat moneda nu este in portofel!\n";
        return false;
    }

    friend ostream& operator << (ostream& os, const Portofel& portofel) {
        for (vector <pair<Moneda,double>>:: const_iterator moneda = portofel.monede.begin(); moneda != portofel.monede.end(); ++moneda)
            os<<moneda->first<<" "<<moneda->second<<"\n";
        return os;
    }
};

class Utilizator {
private:
    string nume;
    Portofel portofel;
    vector <Tranzactie> tranzactii;

public:
    Utilizator() = default;
    Utilizator(const string &nume)
        :nume(nume) {
    }

    void cumpara (const Moneda& moneda, double cantitate) {
        bool status=portofel.cumparaMoneda(moneda,cantitate);
        tranzactii.push_back(Tranzactie(moneda,"BUY",cantitate,status));
    }
    void vinde (const Moneda& moneda, double cantitate) {
        bool status=portofel.vindeMoneda(moneda,cantitate);
        tranzactii.push_back(Tranzactie(moneda,"SELL",cantitate,status));
    }

    friend ostream& operator << (ostream& os, const Utilizator& utilizator) {
        os<<utilizator.nume<<":\n";
        os<<utilizator.portofel;
        for (vector<Tranzactie>:: const_iterator tranzactie = utilizator.tranzactii.begin(); tranzactie != utilizator.tranzactii.end(); ++tranzactie)
            os<<*tranzactie<<"\n";
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

    Tranzactie t1(btc, "BUY", 5,true);
    cout << t1.get_valoareTranzactie() << " EUR" << endl;
    cout << t1 << endl;

    Utilizator user("John Doe");
    user.cumpara(btc, 2);
    cout << user;

    user.vinde(btc, 3);
    cout << user;
    return 0;
}

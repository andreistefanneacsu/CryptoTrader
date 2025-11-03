#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

enum Status {
    SUCCES,
    EROARE_FONDURI,
    EROARE_MONEDA
};

inline double calc_pretMediu(double val_totala, double cant_totala) {
    if (cant_totala == 0) return 0;
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

    [[nodiscard]] string get_nume() const {
        return nume;
    }

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
    Status status;

public:
    Tranzactie()
        : tip(""), pret(0), cantitate(0), status(SUCCES) {
    }
    Tranzactie(const Moneda &moneda, const string &tip, const double pret, const double cantitate, const Status status)
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
    [[nodiscard]] Status get_status() const {
        return status;
    }

/*    double get_ValoareTranzactie() const {
        return cantitate * pret;
    } */

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
        for (vector<Tranzactie>::const_iterator tranzactie = tranzactii.begin(); tranzactie != tranzactii.end(); ++tranzactie) {
            if (tranzactie->get_status() == SUCCES) {
                bool moneda_gasita = false;
                for (vector<pair<Moneda, pair<double, double>>>::iterator moneda = monede.begin(); moneda != monede.end(); ++moneda) {
                    if (moneda->first.get_simbol() == tranzactie->get_moneda().get_simbol()) {
                        if (tranzactie->get_tip() == "BUY") {
                            double val_totala = moneda->second.first * moneda->second.second +
                                              tranzactie->get_cantitate() * tranzactie->get_pret();
                            double cant_totala = moneda->second.first + tranzactie->get_cantitate();
                            moneda->second.second = calc_pretMediu(val_totala, cant_totala);
                            moneda->second.first = cant_totala;
                        }
                        else if (tranzactie->get_tip() == "SELL") {
                            moneda->second.first -= tranzactie->get_cantitate();
                            if (moneda->second.first == 0)
                                moneda->second.second = 0;
                        }
                        moneda_gasita = true;
                        break;
                    }
                }
                if (!moneda_gasita && tranzactie->get_tip() == "BUY") {
                    monede.push_back(make_pair(tranzactie->get_moneda(),
                                    make_pair(tranzactie->get_cantitate(), tranzactie->get_pret())));
                }
            }
        }
    }
    ~Portofel() {
        monede.clear();
        tranzactii.clear();
    }

    [[nodiscard]] vector<Tranzactie> get_tranzactii() const {
        return tranzactii;
    }

    bool cumparaMoneda(const Moneda &m, double cantitate) {
        for (vector<pair<Moneda,pair<double,double>>>::iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            if (moneda->first.get_simbol() == m.get_simbol()) {
                moneda->second.second = calc_pretMediu(moneda->second.first*moneda->second.second+cantitate*m.get_pret(),moneda->second.first+cantitate);
                moneda->second.first += cantitate;
                tranzactii.push_back(Tranzactie(m, "BUY", m.get_pret(), cantitate, SUCCES));
                return true;
            }
        }
        monede.push_back({m, {cantitate, m.get_pret()}});
        tranzactii.push_back(Tranzactie(m, "BUY", m.get_pret(), cantitate, SUCCES));
        return true;
    }
    bool vindeMoneda(const Moneda &m, double cantitate) {
        for (vector<pair<Moneda,pair<double,double>>>::iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            if (moneda->first.get_simbol() == m.get_simbol()) {
                if (moneda->second.first > cantitate) {
                    moneda->second.first -= cantitate;
                    tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, SUCCES));
                    return true;
                }
                else if (moneda->second.first == cantitate) {
                    moneda->second.first = 0;
                    moneda->second.second = 0;
                    tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, SUCCES));
                    return true;
                }
                else {
                    tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, EROARE_FONDURI));
                    return false;
                }
            }
        }
        tranzactii.push_back(Tranzactie(m, "SELL", m.get_pret(), cantitate, EROARE_MONEDA));
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
            profit_max+=((moneda->first.get_pret()*(1+moneda->first.get_volatilitate()/100.0))-moneda->second.second)*moneda->second.first;
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
//    string statut;

public:
    Utilizator() = default;
    explicit Utilizator(const string &nume)
        : nume(nume) {
    }
    Utilizator(const string &nume, const Portofel &portofel)
        : nume(nume), portofel(portofel) {
    }
    ~Utilizator() = default;

    [[nodiscard]] string get_nume() const {
        return nume;
    }

    [[nodiscard]] Portofel get_portofel() const {
        return portofel;
    }

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

    void determinareProfitTotal() {
        cout<<portofel.get_ProfitTotal()<<"\n";
    }

    friend ostream &operator<<(ostream &os, const Utilizator &utilizator) {
        os << utilizator.nume << ":\n";
        os << utilizator.portofel;
        return os;
    }
};

class Piata {
private:
    vector<Moneda> monede;
    vector<Utilizator> utilizatori;

public:
    Piata() = default;
    explicit Piata(const vector<Moneda> &monede, const vector<Utilizator> &utilizatori)
        : monede(monede),
          utilizatori(utilizatori) {
    }
    ~Piata() = default;

    [[nodiscard]] vector<Moneda> get_monede() const {
        return monede;
    }

    [[nodiscard]] vector<Utilizator> get_utilizatori() const {
        return utilizatori;
    }

/*    bool adaugaMoneda(const Moneda& m) {
        for (vector<Moneda>::const_iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) if (moneda->get_simbol()==m.get_simbol()) return false;
        monede.push_back(m);
        return true;
    } */

/*    bool stergeMoneda(const string &simbol) {
        for (vector<Moneda>::iterator moneda=monede.begin(); moneda!=monede.end(); ) {
            if (moneda->get_simbol() == simbol) {
                moneda=monede.erase(moneda);
                return true;
            }
            else ++moneda;
        }
        return false;
    } */

    void actualizarePiata() {
        for (vector<Moneda>::iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
            moneda->actualizarePret(((rand()%2001)-1000)/1000.0*moneda->get_volatilitate());
        }
    }

    friend ostream& operator << (ostream& os, const Piata& piata) {
        os << "Piata curenta:\n";
        for (vector<Moneda>::const_iterator moneda=piata.monede.begin(); moneda!=piata.monede.end(); ++moneda)
            os << *moneda << "\n";
        return os;
    }
};

Piata incarcaPiata(const string& fisier) {
    ifstream fin(fisier);

    int nMonede;
    fin >> nMonede;
    vector<Moneda> monede;
    for (int i = 0; i < nMonede; ++i) {
        string nume;
        string simbol;
        double pret, volatilitate;
        fin >> nume >> simbol >> pret >> volatilitate;
        monede.push_back(Moneda(nume, simbol, pret, volatilitate));
    }

    int nUsers;
    fin >> nUsers;
    vector<Utilizator> utilizatori;
    for (int i = 0; i < nUsers; ++i) {
        string nume;
        fin >> nume;

        int nTranzactii;
        fin >> nTranzactii;
        vector<Tranzactie> tranzactii;
        for (int j = 0; j < nTranzactii; ++j) {
            string simbol, tip;
            double pret, cantitate;
            int status;
            fin >> simbol >> tip >> pret >> cantitate >> status;
            for (vector<Moneda>::const_iterator moneda=monede.begin(); moneda!=monede.end(); ++moneda) {
                if (moneda->get_simbol() == simbol) tranzactii.push_back(Tranzactie(*moneda,tip,pret,cantitate,(Status)status));
            }
        }
        Utilizator utilizator (nume, Portofel(tranzactii));
        utilizatori.push_back(utilizator);
    }

    Piata piata(monede,utilizatori);
    fin.close();
    return piata;
}

void salveazaPiata(const string& fisier, const Piata& piata) {
    ofstream fout(fisier);

    const vector<Moneda>& monede = piata.get_monede();
    fout << monede.size() << "\n";
    for (vector<Moneda>::const_iterator moneda = monede.begin(); moneda != monede.end(); ++moneda) {
        fout << moneda->get_nume() << " " << moneda->get_simbol() << " "
             << moneda->get_pret() << " " << moneda->get_volatilitate() << "\n";
    }

    const vector<Utilizator>& utilizatori = piata.get_utilizatori();
    fout << utilizatori.size() << "\n";
    for (vector<Utilizator>::const_iterator utilizator = utilizatori.begin(); utilizator != utilizatori.end(); ++utilizator) {
        fout << utilizator->get_nume() << "\n";

        const vector<Tranzactie>& tranzactii = utilizator->get_portofel().get_tranzactii();
        fout << tranzactii.size() << "\n";
        for (vector<Tranzactie>::const_iterator tranzactie = tranzactii.begin(); tranzactie != tranzactii.end(); ++tranzactie) {
            fout << tranzactie->get_moneda().get_simbol() << " "
                 << tranzactie->get_tip() << " " << tranzactie->get_pret() << " "
                 << tranzactie->get_cantitate() << " " << (int)tranzactie->get_status() << "\n";
        }
    }

    fout.close();
}

int main() {
    srand(time(NULL));

    Moneda btc("Bitcoin", "BTC", 30000, 5.0);
    Moneda eth("Ethereum", "ETH", 2000, 7.5);
    Moneda ada("Cardano", "ADA", 0.35, 10.0);

    vector<Moneda> monede = {btc, eth, ada};

    Utilizator user("Alice");

    user.cumpara(btc, 1.5);    // buy 1.5 BTC
    user.cumpara(eth, 10);     // buy 10 ETH
    user.cumpara(ada, 2000);   // buy 2000 ADA

    cout << "\n=== Dupa achizitii ===\n";
    user.determinareValoarePortofel();
    user.determinareProfitTotal();
    user.determinareProfitPosibil();

    user.vinde(eth, 5);
    user.vinde(btc, 1);
    user.vinde(ada, 2500); // should fail (not enough ADA)

    cout << "\n=== Dupa vanzari ===\n";
    user.determinareValoarePortofel();
    user.determinareProfitTotal();
    user.determinareProfitPosibil();

    vector<Utilizator> users = {user};
    Piata piata(monede, users);

    cout << "\n=== Piata initiala ===\n" << piata;

    piata.actualizarePiata();
    cout << "\n=== Piata actualizata ===\n" << piata;

    string fisier = "logs.txt";
    salveazaPiata(fisier, piata);


    Piata piata2 = incarcaPiata(fisier);

    cout << "\n=== Portofel final ===\n" << user;

    cout << "\n=== Piata noua ===\n" << piata2;

    return 0;
}

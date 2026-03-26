#include <iostream>
#include <vector>
#include <ctime>
using namespace std;


/* Legenda:
    X - zakryte
    * - pudło
    o - trafiony
    z - zatopiony
*/

class Statek {
public:
    int rozmiar;
    int trafienia;
    vector<pair<int, int>> wspolrzedne;

    Statek(int r) : rozmiar(r), trafienia(0){}

    bool czyZatopiony() const {
        return trafienia == rozmiar;
    }

};

class Plansza {
private:
    const int r = 11;
    vector<vector<char>> pola;
    vector<Statek> statki;
    int losuj_wsp() {
        int min = 1;
        int max = 10;
        int losowa = min + rand() % (max - min + 1);
        return losowa;
    }
    int losuj_zwrot() {
        int min = 0;        //0 - dol, 1 - prawo
        int max = 1;
        int losowa = min + rand() % (max - min + 1);
        return losowa;
    }

    void umiescStatki() {
        ustawStatek(4);
        ustawStatek(3);
        ustawStatek(3);
        ustawStatek(2);
        ustawStatek(2);
        ustawStatek(2);
        ustawStatek(1);
        ustawStatek(1);
        ustawStatek(1);
        ustawStatek(1);

/*
1 x 4M
2 x 3M
3 x 2M
4 x 1M
Plansza zakryta składa się TYLKO z * i o

*/

    }

public:

    Plansza() : pola(r, vector<char>(r, 'X')) {
        for (int i=1; i < 11; i++) {
            pola[0][i] = (char) (i+64);
        }
        for (int i=0; i < 10; i++) {
            pola[i+1][0] = (char) (i+48);
        }
    }

    Plansza(bool dummy) : pola(r, vector<char>(r, 'X')) {
        for (int i=1; i < 11; i++) {
            pola[0][i] = (char) (i+64);
        }
        for (int i=0; i < 10; i++) {
            pola[i+1][0] = (char) (i+48);
        }
        umiescStatki();
    }


    void wyswietl() const {
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < r; ++j) {
                cout << pola[i][j] << " ";
            }
            cout << endl;
        }
    }
    bool czyJuzStrzelano(char kol, int wiersz) {
        wiersz++;
        int kol_int = (int) kol - 64;
        if (pola[wiersz][kol_int] == 'X') return false;
        return true;
    }

    bool sprawdzStrzal(int wiersz, int kol) {
        if (pola[wiersz][kol] == 'o') return true;
        else return false;
    }
    void oznacz(int wiersz, int kol, char symb) {
        pola[wiersz][kol] = symb;
    }
    //wektor dba o pamięć więc bez destruktora

    bool czy_miesci_na_planszy(int i, int j) {
        if (i<1 || i >10 || j < 1 || j > 10) return false;
        return true;
    }
    bool czy_puste(int i, int j) {
        if (pola[i][j] !='X') return false;
        return true;
    }

    bool zapiszTrafienie(int w, int k, Plansza& kompPok) {
    for (auto& s : statki) {
        for (auto p : s.wspolrzedne) {
            if (p.first == w && p.second == k) {
                s.trafienia++;
                if (s.czyZatopiony()) {
                    for (int i=0; i < s.rozmiar; i++) {
                        kompPok.oznacz(s.wspolrzedne[i].first, s.wspolrzedne[i].second, 'z');
                    }
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}
private:
    bool moznaPostawic(int w, int k, int rozmiar, int zwrot) {
        for (int i = 0; i < rozmiar; i++) {
            int w_spr = (zwrot == 1) ? w + i : w;
            int k_spr = (zwrot == 0) ? k + i : k;

            if (!czy_puste(w_spr,k_spr) || !czy_miesci_na_planszy(w_spr,k_spr)) return false;

            for (int dw = -1; dw <= 1; dw++) {      //przeszukujemy czy sasiednie puste
                for (int dk = -1; dk <= 1; dk++) {
                    int sasiad_w = w_spr + dw;
                    int sasiad_k = k_spr + dk;

                    if (czy_miesci_na_planszy(sasiad_w, sasiad_k)) {       //jezeli sasiad poza brzegiem planszy to nie sprawdzamy
                        if (!czy_puste(sasiad_w, sasiad_k)) return false;
                    }
                }
            }
        }
        return true;
    }

    void ustawStatek(int rozmiar) {
        bool ustawiony = false;
        while (!ustawiony) {
            int w = losuj_wsp();
            int k = losuj_wsp();
            int z = losuj_zwrot();

            if (moznaPostawic(w, k, rozmiar, z)) {
                Statek nowy(rozmiar);
                for (int i = 0; i < rozmiar; i++) {
                    int wiersz = (z == 1) ? w + i : w;
                    int kol = (z == 0) ? k + i : k;

                    pola[wiersz][kol] = 'o';
                    nowy.wspolrzedne.push_back({wiersz, kol});
                }
                statki.push_back(nowy);
                ustawiony = true;
            }
        }
    }
};

class Gra {
private:
    Plansza kompUkr;
    Plansza kompPok;
    int trafienia;
    int pudla;
    const int maksPudel = 10;
    const int celTrafien = 20;

public:
    Gra() : kompUkr(true), kompPok(), trafienia(0), pudla(0) {}

    void start() {
        bool koniec = false;
        while (!koniec) {
            kompPok.wyswietl();
            koniec = turaGracza();

            if (pudla >= maksPudel) {
                cout << "Przegrana" << endl;
                koniec = true;
            }
            if (trafienia >= celTrafien) {
                cout << "Wygrana" << endl;
                koniec = true;
            }
        }
    }

    void pokazPlanszeKomputera() const {
        cout << "plansza ukryta komp" << endl;
        kompUkr.wyswietl();
    }
private:
    bool turaGracza() {
        char kol_char;
        int wiersz;
        bool incorrect = true;
        while (incorrect) {
            cin >> kol_char >> wiersz;
            if ((int)kol_char >=65 && (int)kol_char <= 74 && wiersz >= 0 && wiersz <= 9 && !kompPok.czyJuzStrzelano(kol_char, wiersz)) incorrect = false;
            else cout << "Błędne współrzędne" <<endl;   //można dodać komentarz już to strzelałeś
        }
        int kol_int = (int) kol_char - 64;
        wiersz++;

        if (kompUkr.sprawdzStrzal(wiersz, kol_int)) {
            cout << "Trafiony!" << endl;
            kompPok.oznacz(wiersz, kol_int, 'o');
            trafienia++;
            if (kompUkr.zapiszTrafienie(wiersz, kol_int, kompPok)) {
                cout << "Trafiony i ZATOPIONY!" << endl;
            }
        } else {
            cout << "Pudło!" << endl;
            kompPok.oznacz(wiersz, kol_int, '*');
            pudla++;
        }
        return false;
    }
};


int main() {
    srand(time(0));
    Gra statki;
    statki.pokazPlanszeKomputera();
    statki.start();

    return 0;
}



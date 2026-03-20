#include <iostream>
#include <vector>
using namespace std;


/* Legenda:
    X - zakryte
    * - pudło
    o - trafiony
    S - statek - potrzebne przy planszy uzytkownika
*/

class Plansza {
private:
    const int r = 11;
    vector<vector<char>> pola;
    void umiescStatki() {

    pola[2][2] = 'o';       //plansza testowa
    pola[3][2] = 'o';
    pola[4][2] = 'o';
    pola[5][2] = 'o';

    pola[8][4] = 'o';
    pola[8][5] = 'o';
    pola[8][6] = 'o';

    pola[1][5] = 'o'; pola[1][6] = 'o';
    pola[5][8] = 'o'; pola[6][8] = 'o';

    pola[10][10] = 'o';
    pola[3][7] = 'o';
    pola[9][1] = 'o';
    pola[9][3] = 'o';

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

        if (kompUkr.sprawdzStrzal(wiersz, kol_int)) {   //co jak dwa razy w to samo miejsce, albo zatopiony - statki jako klasy?
            cout << "Trafiony!" << endl;
            kompPok.oznacz(wiersz, kol_int, 'o');
            trafienia++;
        } else {
            cout << "Pudło!" << endl;
            kompPok.oznacz(wiersz, kol_int, '*');
            pudla++;
        }
        return false;
    }
};


int main() {
    Gra statki;
    statki.start();
    return 0;
}


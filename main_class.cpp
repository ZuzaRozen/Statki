#include <iostream>
#include <vector>
using namespace std;

int ilosc_pudel = 0;
int ilosc_trafien = 0;
int const maks_il_pudel = 10;

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
            cout << "\n";
        }
    }
    //wektor dba o pamięć więc bez destruktora
};

    void strzal(char i, char j, Plansza ukryta, Plansza pokazana) {
        int int_i = (int) i - 64, int_j = (int) j - 47;     //i - kolumna, j - wiersz
        //cout << int_i<< " " <<int_j<<endl;
        pokazana.pola[j][i] = ukryta.pola[j][i];        //trzeba w klasie
        if (KompUkr[j][i] == '*') ilosc_pudel++;
        if (KompUkr[j][i] == 'o') ilosc_trafien++;
    }

int main() {
    Plansza KompUkr(true);
    Plansza KompPok;
    //Plansza Uzyt(true);

    KompUkr.wyswietl();
    KompPok.wyswietl();
    //Uzyt.wyswietl();

    bool gra = true;
    char i, j;
    int int_i, int_j;

    while (gra) {
        //ruch gracza
        //ruch komputera
        //wyświetlenie obu plansz
        //sprawdzenie czy ktoś wygrał
        bool incorrect = true;
        while (incorrect) {
            cin >> i >> j;
            //jesli poprawne to correct i idziemy dalej
            if ((int)i >=65 && (int)i <= 74 && (int) j >= 49 && (int) j <= 51) incorrect = false;
            else cout << "Błędne współrzędne" <<endl;
        }
        strzal(i, j, KompPok, KompUkr);
        if (ilosc_pudel == maks_il_pudel) {     //możliwe że gra vs komp
            cout <<"Przegrana"<<endl;
            gra = false;
        }

        if (ilosc_trafien == 20) {     //możliwe że gra vs komp
            cout <<"Wygrana"<<endl;
            gra = false;
        }
    }
    return 0;
}

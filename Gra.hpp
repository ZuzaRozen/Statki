#ifndef GRA_HPP
#define GRA_HPP
//jakimś cudem to jest header file?
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Statek {
public:
    int rozmiar;
    int trafienia;
    std::vector<std::pair<int, int>> wspolrzedne;

    Statek(int r) : rozmiar(r), trafienia(0) {}
    bool czyZatopiony() const { return trafienia == rozmiar; }
};

class Plansza {
private:
    vector<vector<char>> pola;
    vector<Statek> statki;

    int losuj_wsp(int r) { return 1 + rand() % (r-1); }
    int losuj_zwrot() { return rand() % 2; }

    void umiescStatki(int rozmiarPlanszy) {
        int schemat[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
        for (int s : schemat) ustawStatek(s, rozmiarPlanszy);
    }

    void umiescStatkiDemo(int rozmiarPlanszy) {
        int schemat[] = { 2, 1, 1 };
        for (int s : schemat) ustawStatek(s, rozmiarPlanszy);
    }

public:
    Plansza(bool generuj, int rozmiarPlanszy) : pola(rozmiarPlanszy, vector<char>(rozmiarPlanszy, 'X')) {
        pola[0][0] = ' ';
        //cout << rozmiarPlanszy;
        for (int i = 1; i < rozmiarPlanszy; i++) pola[0][i] = (char)(i + 64);
        for (int i = 1; i < rozmiarPlanszy; i++) pola[i][0] = (char)(i + 47);
        if (generuj && rozmiarPlanszy == 11) umiescStatki(rozmiarPlanszy);
        if (generuj && rozmiarPlanszy != 11) umiescStatkiDemo(rozmiarPlanszy);
    }

    char getPole(int w, int k) const { return pola[w][k]; }

    bool czyJuzStrzelano(int w, int k) { return pola[w][k] != 'X'; }

    bool sprawdzStrzal(int w, int k) { return pola[w][k] == 'o'; }

    void oznacz(int w, int k, char symb) { pola[w][k] = symb; }

    bool zapiszTrafienie(int w, int k, Plansza& kompPok) {
        for (auto& s : statki) {
            for (auto p : s.wspolrzedne) {
                if (p.first == w && p.second == k) {
                    s.trafienia++;
                    if (s.czyZatopiony()) {
                        for (auto wsp : s.wspolrzedne) kompPok.oznacz(wsp.first, wsp.second, 'z');
                        return true;
                    }
                    return false;
                }
            }
        }
        return false;
    }

    void ustawStatekNaWsp(int wiersz, int kol) {
        pola[wiersz][kol] = 'o';
    }

    void wyczyscPlansze(int rozmiarPlanszy) {
        for (int i = 1; i < rozmiarPlanszy; i++) {
            for (int j = 1; j < rozmiarPlanszy; j++) {
                pola[i][j] = 'X';
            }
        }
    }

    bool poprawnieUstawione(int rozmiarPlanszy) {
        vector<vector<bool>> odwiedzone(rozmiarPlanszy, vector<bool>(rozmiarPlanszy, false));
        vector<int> znalezioneStatki; //rozmiary znalezionych statkow

        for (int i = 1; i < rozmiarPlanszy; i++) {
            for (int j = 1; j < rozmiarPlanszy; j++) {
                //jeśli pole jeszcze nieodwiedzone
                if (pola[i][j] == 'o' && !odwiedzone[i][j]) {
                    int rozmiar = 0;
                    if (!liczRozmiarStatku(i, j, odwiedzone, rozmiar, rozmiarPlanszy)) return false; //błąd, gdy ma sąsiada na rogu
                    znalezioneStatki.push_back(rozmiar);
                }
            }
        }

        // Czy odpowiednia ilosc statków
        // 1x4, 2x3, 3x2, 4x1
        int s1 = 0, s2 = 0, s3 = 0, s4 = 0;
        if (rozmiarPlanszy == 11) {
            for (int s : znalezioneStatki) {
                if (s == 1) s1++;
                else if (s == 2) s2++;
                else if (s == 3) s3++;
                else if (s == 4) s4++;
                else return false;
            }
            return (s4 == 1 && s3 == 2 && s2 == 3 && s1 == 4);
        }
        if (rozmiarPlanszy == 6) {
            for (int s : znalezioneStatki) {
                if (s == 1) s1++;
                else if (s == 2) s2++;
                else return false;
            }
            return (s2 == 1 && s1 == 2);

        }
    }

    bool maSasiadowSkos(int w, int k, int rozmiarPlanszy) {
        for (int dw = -1; dw <= 1; dw+=2) {
            for (int dk = -1; dk <= 1; dk+=2) {
                int sw = w + dw, sk = k + dk;
                if (sw >= 1 && sw < rozmiarPlanszy && sk >= 1 && sk < rozmiarPlanszy && pola[sw][sk] == 'o') return true;
            }
        }
        return false;
    }

    bool zaDuzoRozmiaru(int w, int k, int rozmiarPlanszy) {
        if (rozmiarPlanszy == 11) return zaDuzoRozmiaru10(w, k);
        else return zaDuzoRozmiaruDemo(w, k);
    }

    bool zaDuzoRozmiaru10(int w, int k) {
        vector<vector<char>> kopiaPol = pola;
        kopiaPol[w][k] = 'o'; // udawany statek

        vector<vector<bool>> odwiedzone(11, vector<bool>(11, false));
        int s1 = 0, s2 = 0, s3 = 0, s4 = 0;

        for (int i = 1; i <= 10; i++) {
            for (int j = 1; j <= 10; j++) {
                if (kopiaPol[i][j] == 'o' && !odwiedzone[i][j]) {
                    int rozmiar = 0;
                    if (!liczRozmiarNaKopii(i, j, odwiedzone, rozmiar, kopiaPol, 11)) return true;

                    if (rozmiar == 1) s1++;
                    else if (rozmiar == 2) s2++;
                    else if (rozmiar == 3) s3++;
                    else if (rozmiar == 4) s4++;
                    else return true; //za duzy
                }
            }
        }

        if (s4 > 1 || s3 + s4 > 3 || s4 + s3 + s2 > 6 || s4 + s3 + s2 + s1 > 10) return true;

        int sumaMasztow = s4 * 4 + s3 * 3 + s2 * 2 + s1 * 1;
        if (sumaMasztow > 20) return true;      //może zbędne

        return false;
    }

    bool zaDuzoRozmiaruDemo(int w, int k) {
        vector<vector<char>> kopiaPol = pola;
        kopiaPol[w][k] = 'o'; // udawany statek

        vector<vector<bool>> odwiedzone(6, vector<bool>(6, false));
        int s1 = 0, s2 = 0;

        for (int i = 1; i < 6; i++) {
            for (int j = 1; j < 6; j++) {
                if (kopiaPol[i][j] == 'o' && !odwiedzone[i][j]) {
                    int rozmiar = 0;
                    if (!liczRozmiarNaKopii(i, j, odwiedzone, rozmiar, kopiaPol, 6)) return true; //dostosowac

                    if (rozmiar == 1) s1++;
                    else if (rozmiar == 2) s2++;
                    else return true; //za duzy
                }
            }
        }

        if (s2 > 1 ||  s2 + s1 > 3) return true;

        int sumaMasztow = s2 * 2 + s1 * 1;
        if (sumaMasztow > 4) return true;      //może zbędne

        return false;
    }

private:
    private:
        bool liczRozmiarNaKopii(int w, int k, vector<vector<bool>>& odwiedzone, int& rozmiar, 
            const vector<vector<char>>& kopia, int rozmiarPlanszy) {
            if (w < 1 || w > rozmiarPlanszy-1 || k < 1 || k > rozmiarPlanszy - 1 || kopia[w][k] != 'o' || odwiedzone[w][k]) return true;

            odwiedzone[w][k] = true;
            rozmiar++;

            //skosy dla kopii
            for (int dw = -1; dw <= 1; dw++) {
                for (int dk = -1; dk <= 1; dk++) {
                    if (dw == 0 && dk == 0) continue;
                    int nw = w + dw, nk = k + dk;
                    if (nw >= 1 && nw < rozmiarPlanszy && nk >= 1 && nk < rozmiarPlanszy && kopia[nw][nk] == 'o' && !odwiedzone[nw][nk]) {
                        if (abs(dw) + abs(dk) == 2) return false;
                    }
                }
            }
            
            return liczRozmiarNaKopii(w + 1, k, odwiedzone, rozmiar, kopia, rozmiarPlanszy) &&
                liczRozmiarNaKopii(w - 1, k, odwiedzone, rozmiar, kopia, rozmiarPlanszy) &&
                liczRozmiarNaKopii(w, k + 1, odwiedzone, rozmiar, kopia, rozmiarPlanszy) &&
                liczRozmiarNaKopii(w, k - 1, odwiedzone, rozmiar, kopia, rozmiarPlanszy);
        }

    bool liczRozmiarStatku(int w, int k, vector<vector<bool>>& odwiedzone, int& rozmiar, int rozmiarPlanszy) {
        if (w < 1 || w > rozmiarPlanszy-1 || k < 1 || k > rozmiarPlanszy-1 || pola[w][k] != 'o' || odwiedzone[w][k]) return true;

        odwiedzone[w][k] = true;
        rozmiar++;

        //Czy ma sąsiadów na rogach?
        for (int dw = -1; dw <= 1; dw++) {
            for (int dk = -1; dk <= 1; dk++) {
                if (dw == 0 && dk == 0) continue;
                int nw = w + dw;
                int nk = k + dk;
                if (nw >= 1 && nw < rozmiarPlanszy && nk >= 1 && nk < rozmiarPlanszy && pola[nw][nk] == 'o' && !odwiedzone[nw][nk]) {
                    //jeśli tak, to błąd
                    if (abs(dw) + abs(dk) == 2) return false;
                }
            }
        }

        //szukaj statku góra/dół/lewo/prawo
        if (!liczRozmiarStatku(w + 1, k, odwiedzone, rozmiar, rozmiarPlanszy)) return false;
        if (!liczRozmiarStatku(w - 1, k, odwiedzone, rozmiar, rozmiarPlanszy)) return false;
        if (!liczRozmiarStatku(w, k + 1, odwiedzone, rozmiar, rozmiarPlanszy)) return false;
        if (!liczRozmiarStatku(w, k - 1, odwiedzone, rozmiar, rozmiarPlanszy)) return false;

        return true;
    }

    bool moznaPostawic(int w, int k, int rozmiar, int zwrot, int rozmiarPlanszy) {
        for (int i = 0; i < rozmiar; i++) {
            int w_spr = (zwrot == 1) ? w + i : w;
            int k_spr = (zwrot == 0) ? k + i : k;
            if (w_spr < 1 || w_spr > rozmiarPlanszy-1 || k_spr < 1 || k_spr > rozmiarPlanszy-1 || pola[w_spr][k_spr] != 'X') return false;

            for (int dw = -1; dw <= 1; dw++) {
                for (int dk = -1; dk <= 1; dk++) {
                    int sw = w_spr + dw, sk = k_spr + dk;
                    if (sw >= 1 && sw < rozmiarPlanszy && sk >= 1 && sk < rozmiarPlanszy && pola[sw][sk] == 'o') return false;
                }
            }
        }
        return true;
    }

    void ustawStatek(int rozmiar, int rozmiarPlanszy) {
        bool ustawiony = false;
        while (!ustawiony) {
            int w = losuj_wsp(rozmiarPlanszy), k = losuj_wsp(rozmiarPlanszy), z = losuj_zwrot();
            if (moznaPostawic(w, k, rozmiar, z, rozmiarPlanszy)) {
                Statek nowy(rozmiar);
                for (int i = 0; i < rozmiar; i++) {
                    int wr = (z == 1) ? w + i : w, kl = (z == 0) ? k + i : k;
                    pola[wr][kl] = 'o';
                    nowy.wspolrzedne.push_back({ wr, kl });
                }
                statki.push_back(nowy);
                ustawiony = true;
            }
        }
    }
  
   
};

class Gra {
public:
    int r;
    Plansza kompUkr;
    Plansza kompPok;
    Plansza PlanszaUzytkownika;
    int trafienia =0, pudla=0;
    int trafieniaKomp =0;
    bool koniec;
    bool gotowa;
    bool wygrana=0;
    int celTrafien;
    bool pierwszyStrzal = 1;


    Gra(int rozmiar, int cel) :
        r(rozmiar),
        celTrafien(cel), 
        kompUkr(true, rozmiar),
        kompPok(false, rozmiar),
        PlanszaUzytkownika(false, rozmiar),
        trafienia(0),
        pudla(0), 
        koniec(false),
        gotowa(false) {}

    /*bool poprawnyStrzal(int w, int k) {
        if (kompPok.czyJuzStrzelano(w, k)) return false;
        return true;
    }*/

    void wykonajStrzal(int w, int k) {
        //if (koniec || kompPok.czyJuzStrzelano(w, k)) return;

        if (kompUkr.sprawdzStrzal(w, k)) {
            kompPok.oznacz(w, k, 'o');
            trafienia++;
            kompUkr.zapiszTrafienie(w, k, kompPok);
        }
        else {
            kompPok.oznacz(w, k, '*');
            pudla++;
        }
        if (trafienia >= celTrafien) {
            koniec = true;
            wygrana = 1;
        }
    }

    pair<int, int> analizujPlansze() {
        int w, k;
        do {
            w = 1 + rand() % (r - 1);
            k = 1 + rand() % (r - 1);
        } while (PlanszaUzytkownika.getPole(w, k) == '*' ||
            PlanszaUzytkownika.getPole(w, k) == 'z');
        return make_pair(w, k);
    }

    void strzalKomputera() {
        if (koniec) return;

        int w, k;
        if (pierwszyStrzal) {
            do {
                w = 1 + rand() % (r-1);
                k = 1 + rand() % (r - 1);
            } while (PlanszaUzytkownika.getPole(w, k) == '*' ||
                PlanszaUzytkownika.getPole(w, k) == 'z');
        }
        else {
            pair <int, int> wspolrzedne = analizujPlansze();
            w = wspolrzedne.first;
            k = wspolrzedne.second;
        }

        if (PlanszaUzytkownika.getPole(w, k) == 'o') {
            PlanszaUzytkownika.oznacz(w, k, 'z');
            trafieniaKomp++;
        }
        else { 
            PlanszaUzytkownika.oznacz(w, k, '*');
        }

        if (trafieniaKomp >= celTrafien) {
            koniec = true;
            wygrana = false;
        }
    }
};

#endif

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "Gra.hpp"
#include <iostream>
using namespace sf;
using namespace std;

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(1900, 1100), "Statki SFML");

    sf::Font font;
    if (!font.loadFromFile("bahnschrift.ttf")) return -1;

    Gra gra;
    bool kolejGracza = 1;
    bool trybDemo = 0;
    float cellSize = 80.f;
    float margin = 40.f;
    float offsetPrzeciwnika = 950.f;
    int ile_statkow = 0;
    Text instrukcje;
    instrukcje.setFont(font);
    instrukcje.setCharacterSize(40);
    instrukcje.setPosition(margin, 11 * cellSize + 80);
    instrukcje.setString("Ustaw swoje statki (kliknij 20 razy)");


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
          
            if (event.type == Event::MouseButtonPressed && !gra.koniec && !gra.gotowa) {

                if (event.mouseButton.button == Mouse::Left) {
                    int kol = (event.mouseButton.x - margin) / cellSize;
                    int wiersz = (event.mouseButton.y - margin) / cellSize;

                    if (wiersz >= 1 && wiersz <= 10 && kol >= 1 && kol <= 10 && !gra.PlanszaUzytkownika.maSasiadowSkos(wiersz, kol)
                        && !gra.PlanszaUzytkownika.czyJuzStrzelano(wiersz, kol) && !gra.PlanszaUzytkownika.zaDuzoRozmiaru(wiersz, kol)) {
                        gra.PlanszaUzytkownika.ustawStatekNaWsp(wiersz, kol);
                        ile_statkow++;
                        instrukcje.setString("Ustaw swoje statki (kliknij 20 razy)");
                    }
                    else instrukcje.setString("Niepoprawnie zaznaczone pole.");
                }
                if (ile_statkow == 20) {
                    if (gra.PlanszaUzytkownika.poprawnieUstawione()) {
                        instrukcje.setString("Twoja kolej! Strzelaj w plansze po prawej stronie.");
                        gra.gotowa = true;
                    }
                    else {
                        instrukcje.setString("Ustaw swoje statki poprawnie");
                        gra.PlanszaUzytkownika.wyczyscPlansze();
                        ile_statkow = 0;
                    }
                }

            }


            if (event.type == Event::MouseButtonPressed && !gra.koniec && gra.gotowa && kolejGracza) {
                if (event.mouseButton.button == Mouse::Left) {
                    //zmieniona obsługa myszki
                    int kol = (event.mouseButton.x - margin - offsetPrzeciwnika) / cellSize;
                    int wiersz = (event.mouseButton.y - margin) / cellSize;

                    if (wiersz >= 1 && wiersz <= 10 && kol >= 1 && kol <= 10) {
                        if (!gra.kompPok.czyJuzStrzelano(wiersz, kol)) {

                            gra.wykonajStrzal(wiersz, kol);

                            if (!gra.koniec) {
                                kolejGracza = false; 
                            }
                        }
                    }
                }
            }
            if (!kolejGracza && !gra.koniec) {
                gra.strzalKomputera();
                kolejGracza = true; 
            }
        }

        window.clear(Color(30, 30, 30));

        // Rysowanie planszy użytkownika (lewa strona)
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                char stan = gra.PlanszaUzytkownika.getPole(i, j);
                RectangleShape cell(Vector2f(cellSize - 2, cellSize - 2));
                cell.setPosition(j * cellSize + margin, i * cellSize + margin);

                if (i == 0 || j == 0) cell.setFillColor(Color(100, 100, 100));
                else if (stan == 'X') cell.setFillColor(Color(70, 70, 150));
                else if (stan == '*') cell.setFillColor(Color::Blue);
                else if (stan == 'o') cell.setFillColor(Color::White);
                else if (stan == 'z') cell.setFillColor(Color::Magenta);

                window.draw(cell);

                if (i == 0 || j == 0) {
                    Text text(string(1, stan), font, 36);
                    text.setPosition(j * cellSize + margin + 20, i * cellSize + margin + 10);
                    window.draw(text);
                }
            }
        }

        //Rysowanie planszy komputera (prawa strona)
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11; j++) {
                char stan = gra.kompPok.getPole(i, j);
                RectangleShape cell(Vector2f(cellSize - 2, cellSize - 2));
                //przesuwamy o offsetPrzeciwnika!!!
                cell.setPosition(j * cellSize + margin + offsetPrzeciwnika, i * cellSize + margin);

                if (i == 0 || j == 0) cell.setFillColor(Color(100, 100, 100));
                else if (stan == 'X') cell.setFillColor(Color(70, 70, 150));
                else if (stan == '*') cell.setFillColor(Color::Blue);
                else if (stan == 'o') cell.setFillColor(Color::Red);
                else if (stan == 'z') cell.setFillColor(Color::Yellow);

                window.draw(cell);

                if (i == 0 || j == 0) {
                    Text text(string(1, stan), font, 36);
                    text.setPosition(j * cellSize + margin + offsetPrzeciwnika + 20, i * cellSize + margin + 10);
                    window.draw(text);
                }
            }
        }

        // Status gry
        Text status;
        status.setFont(font);
        status.setCharacterSize(40);
        status.setPosition(margin+1000, 11 * cellSize + 80);        //napis przesunięty pod planszę komputera, Przemyśleć?
        if (gra.koniec) {
            status.setString(gra.trafienia >= gra.celTrafien ? "WYGRANA!" : "PRZEGRANA!");
            status.setFillColor(gra.trafienia >= gra.celTrafien ? Color::Green : Color::Red);
        }
        /*else {
            status.setString("Pudla: " + to_string(gra.pudla) + " / " + to_string(gra.maksPudel));
        }*/
        window.draw(status);
        window.draw(instrukcje);

        window.display();
    }

    return 0;
}
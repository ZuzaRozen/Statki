#include <iostream>
using namespace std;
int* tab_komp_ukr = new int[121];
int* tab_komp_pokaz = new int[121];

void generuj_tab_komp() {
    for (int i = 0; i < 121; i++) {
        *(tab_komp_ukr + i) = 'X';
    }
    for (int i=1; i <11; i++) {
        *(tab_komp_ukr+i) = (char) (i+64);
    }
}

void drukuj_tab(int* tab) {
    for (int i=0; i < 121; i++){
        cout << (char)*(tab + i) << " ";
        if (i!=0 && (i+1)%11==0) cout <<endl;
    }
}

/*
1 x 4M
2 x 3M
3 x 2M
*/

int main() {
    generuj_tab_komp();
    drukuj_tab(tab_komp_ukr);
}

/*
 * "BOULDER DASH"
 *
 * Program zaliczeniowy do zajęć laboratoryjnych
 *
 * Program umożliwia grę w "Boulder Dasha" w trybie wsadowym.
 *
 * autor: Mateusz Sulimowicz <ms429603@students.mimuw.edu.pl>
 * wersja: 2.0.0
 * data: 20 grudnia 2020
 * */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* struktury danych */

/* Współrzędne pola na planszy: */
typedef struct {
    int wiersz;
    int kolumna;
} wspolrzedne;

/* Aktualny stan planszy: */
typedef struct {
    int diamenty;
    int ile_wierszy;
    int ile_kolumn;
    char *PLANSZA;
}plansza;

/* Aktualny stan postaci: */
typedef struct {
    wspolrzedne miejsce;
    int diamenty;
    bool jest_na_planszy;
}postac;


int WczytajRozmiar(void) {
    int w = 0;
    scanf("%d", &w);
    return w;
}

/*tworzymy planszę o wymiarach wczytanych z pliku*/
void inicjalizuj(plansza *p) {
    p->ile_wierszy = WczytajRozmiar();
    p->ile_kolumn = WczytajRozmiar();
    if((p->ile_wierszy != 0) && (p->ile_kolumn != 0)) {
        p->PLANSZA = malloc(((unsigned int) (p->ile_wierszy * p->ile_kolumn)) *  sizeof (p->PLANSZA));
    }
}

/* Stan poczatkowy planszy*/
void ustal_stan_poczatkowy(plansza *p, postac *rockford) {
    p->diamenty = 0;
    rockford->diamenty = 0;
    char znak = (char) getchar();
    for(int wiersz = 0; wiersz < p->ile_wierszy; ++wiersz) {
        for(int kolumna = 0; kolumna < p->ile_kolumn; ++kolumna) {
            znak = (char) getchar();
            if(znak == '$')
                ++(p->diamenty);
            if(znak == '@') {
                rockford->miejsce.wiersz = wiersz;
                rockford->miejsce.kolumna = kolumna;
                rockford->jest_na_planszy = true;
            }
            p->PLANSZA[wiersz * p->ile_kolumn + kolumna] = znak;
        }
        znak = (char) getchar();
    }
}
/*Zwalniamy pamięć*/
void zwolnij_pamiec(plansza *p) {
    free(p->PLANSZA);
}

/*Pobranie z danych ruchu rockforda.*/
char pobierz_ruch(void) {
    char ruch = (char) getchar();
    return ruch;
}

/* wektor przesunięcia Rockforda. */
wspolrzedne rozpoznaj(int c) {
    int w = 0;
    int k = 0;
    wspolrzedne nowe;
    switch (c) {
        case 'a':
            --k;
            break;
        case 'd':
            ++k;
            break;
        case 'w':
            --w;
            break;
        case 's':
            ++w;
            break;
        default:
            break;
    }
    nowe.wiersz = w;
    nowe.kolumna = k;
    return nowe;
}

/*sprawdzamy co znajduje się na polu, na które chce wejść Rockford. */
char przeszkoda(plansza *p, postac *rockford, wspolrzedne ruch) {
    wspolrzedne nowa_pozycja;
    nowa_pozycja.wiersz = rockford->miejsce.wiersz + ruch.wiersz;
    nowa_pozycja.kolumna = rockford->miejsce.kolumna + ruch.kolumna;
    char obiekt = p->PLANSZA[nowa_pozycja.wiersz * p->ile_kolumn + nowa_pozycja.kolumna];
    return obiekt;
}

/*zmiana pozycji Rockforda o wektor przesunięcia*/
void przesun_rockforda(plansza *p, postac *rockford, wspolrzedne ruch) {
    wspolrzedne nowa_pozycja;
    p->PLANSZA[rockford->miejsce.wiersz * p->ile_kolumn + rockford->miejsce.kolumna] = ' ';
    nowa_pozycja.wiersz = rockford->miejsce.wiersz + ruch.wiersz;
    nowa_pozycja.kolumna = rockford->miejsce.kolumna + ruch.kolumna;
    rockford->miejsce.wiersz = nowa_pozycja.wiersz;
    rockford->miejsce.kolumna = nowa_pozycja.kolumna;
    p->PLANSZA[rockford->miejsce.wiersz * p->ile_kolumn + rockford->miejsce.kolumna] = '@';
}
/* Przesunięcie kamienia przez Rockforda. */
bool przesun_kamien(plansza *p, postac *rockford, wspolrzedne ruch) {
    wspolrzedne pozycja_kamienia;
    wspolrzedne nowa_pozycja;
    pozycja_kamienia.wiersz = rockford->miejsce.wiersz + ruch.wiersz;
    pozycja_kamienia.kolumna = rockford->miejsce.kolumna + ruch.kolumna;
    nowa_pozycja.kolumna = pozycja_kamienia.kolumna + ruch.kolumna;
    nowa_pozycja.wiersz = pozycja_kamienia.wiersz + ruch.wiersz;
    if(pozycja_kamienia.kolumna + ruch.kolumna >= 0 &&
       pozycja_kamienia.kolumna + ruch.kolumna < p->ile_kolumn &&
       p->PLANSZA[nowa_pozycja.wiersz * p->ile_kolumn + nowa_pozycja.kolumna] == ' ') {
        p->PLANSZA[pozycja_kamienia.wiersz * p->ile_kolumn + pozycja_kamienia.kolumna] = ' ';
        p->PLANSZA[nowa_pozycja.wiersz * p->ile_kolumn + nowa_pozycja.kolumna] = 'O';
        return true;
    }
    else
        return false;
}

/* Wyjście Rockforda z planszy*/
void wyjscie_rockforda(plansza *p, postac *rockford) {
    p->PLANSZA[rockford->miejsce.wiersz * p->ile_kolumn + rockford->miejsce.kolumna] = ' ';
    rockford->jest_na_planszy = false;
}

/* Interpretacja ruchu rockforda */
void ruch_rockforda(plansza *p, postac *rockford, wspolrzedne ruch) {
    char obiekt = przeszkoda(p, rockford, ruch);
    switch (obiekt) {
        case ' ':
            przesun_rockforda(p, rockford, ruch);
            break;
        case '+':
            przesun_rockforda(p, rockford, ruch);
            break;
        case '$':
            przesun_rockforda(p, rockford, ruch);
            ++(rockford->diamenty);
            break;
        case 'O':
            if(ruch.wiersz == 0) {
                bool t = przesun_kamien(p, rockford, ruch);
                if(t)
                    przesun_rockforda(p, rockford, ruch);
            }
            break;
        case 'X':
            if(rockford->diamenty == p->diamenty) {
                wyjscie_rockforda(p, rockford);
            }
            break;
        default:
            break;
    }
}

/* Wypisanie aktualnego stanu planszy. */
void wypisz_plansze(plansza *p) {
    printf("%d %d\n", p->ile_wierszy, p->ile_kolumn);
    for(int wiersz = 0; wiersz < p->ile_wierszy; ++wiersz) {
        for(int kolumna = 0; kolumna < p->ile_kolumn; ++kolumna)
            printf("%c", p->PLANSZA[wiersz * p->ile_kolumn + kolumna]);
        printf("\n");
    }
}

/* Rodzaj obiektu spadającego przed przejściem w stan stabilny. */
char co_spada(plansza *p, wspolrzedne w) {
    char obiekt = p->PLANSZA[w.wiersz * p->ile_kolumn + w.kolumna];
    /*printf("spada: %c", obiekt);*/
    return obiekt;
}

/* Upadek obiektu przy przejściu w stan stabilny planszy. */
void spada(plansza *p, char obiekt, wspolrzedne w) {
    int gdziespada = w.wiersz + 1;
    while(gdziespada < p->ile_wierszy && p->PLANSZA[gdziespada * p->ile_kolumn + w.kolumna] == ' ') {
        p->PLANSZA[gdziespada * p->ile_kolumn + w.kolumna] = obiekt;
        p->PLANSZA[(gdziespada -1) * p->ile_kolumn + w.kolumna] = ' ';
        /*printf("Obiekt: %c spada na pozycje %d, %d", obiekt, gdziespada, w.kolumna);*/
        ++gdziespada;
    }
}

/*Stan stabilny planszy po ruchu. Wszystko co może spaść, spada.*/
void ustal_stan_stabilny(plansza *p) {
    wspolrzedne w;
    char obiekt;
    for(int wiersz = p->ile_wierszy-1; wiersz >= 0; --wiersz)
        for(int kolumna = 0; kolumna < p->ile_kolumn; ++kolumna)
        {
            w.wiersz = wiersz;
            w.kolumna = kolumna;
            /*printf("wiersz: %d, kolumna: %d", wiersz, kolumna);*/
            obiekt = co_spada(p, w);
            if((obiekt == '$') || (obiekt == 'O'))
                spada(p, obiekt, w);
        }
}

/* Rozgrywka na danych wejściowych w trybie wsadowym. */
void rozgrywka_wsad(plansza *p, postac *rockford) {
    char ruch = pobierz_ruch();
    while(ruch != EOF) {
        if(ruch != '\n') {
            if(rockford->jest_na_planszy) {
                ruch_rockforda(p, rockford, rozpoznaj(ruch));
                ustal_stan_stabilny(p);
            }
        }
        else {
            wypisz_plansze(p);
        }
        ruch = pobierz_ruch();
    }
}


int main(void) {
    plansza p;
    postac rockford;
    inicjalizuj(&p);
    if((p.ile_wierszy != 0) && (p.ile_kolumn != 0)) {
        ustal_stan_poczatkowy(&p, &rockford);
        ustal_stan_stabilny(&p);
        wypisz_plansze(&p);
        rozgrywka_wsad(&p, &rockford);
        zwolnij_pamiec(&p);
    }

    return 0;
}
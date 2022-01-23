#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define MAX 2048
#define DL_LINII 1024

typedef struct {
  int **tab;                    //wskaznik na tablice pikseli
  int wymx,wymy,szarosci;       //parametry obrazka
  int magiczny;
  
  } Obraz;

typedef struct {
  FILE *plik_we, *plik_wy;        //uchwyty do plikow wej i wyj
  int negatyw, progowanie, konturowanie, wyswietl, konwersja, rozmycie, r, g, b;      //opcje wywolania programu
  float w_progu;              //wartosc progu
  int w_rozmycia;
  char *zapis_nazwa;          //nazwa zapis
} t_opcje;


int zapisz(Obraz *obrazek1, t_opcje *opcje);
int czytaj(Obraz *obrazek1, t_opcje *opcje);
int progowanie(Obraz *obrazek1, t_opcje *opcje);
int konturowanie(Obraz *obrazek1);
int negatyw(Obraz *obrazek1);
int zapisz_tymczasowo(Obraz *obrazek1);
int przetwarzaj_opcje(int argc, char **argv, t_opcje *opcje);
int konwersja(Obraz *obrazek1);
int rozmycie(Obraz *obrazek1, t_opcje *opcje);
void wyswietl(t_opcje *opcje);

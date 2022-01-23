#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "funkcje.h"

#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define MAX 2048           /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

int main(int argc, char ** argv) {

  Obraz obrazek1;              //parametry obrazu
  int odczytano = 0;           //gdy odczytano = 0 to nie wczytano pliku
  int a;                       

  t_opcje opcje;
  int kod_bledu = 0;
  kod_bledu=przetwarzaj_opcje(argc,argv,&opcje);

  if (kod_bledu!=0){
    printf("Blad nr %d\n", kod_bledu);
  }
  if (kod_bledu==0){
    /* Tworzenie plikow tymczasowych dla funkcji wyswietl (.ppm i .pgm).*/

    system("touch temporary_plik.ppm");
    system("touch temporary_plik.pgm");

    /*User podal argumenty, wiÄc przetwarzamy obraz tu*/

      czytaj(&obrazek1, &opcje);
      if(opcje.konwersja==1 && obrazek1.magiczny==3){
        konwersja(&obrazek1);
      } 
      if (opcje.konturowanie==1 && obrazek1.magiczny==2){
        konturowanie(&obrazek1);
      }
      if(opcje.rozmycie==1 && obrazek1.magiczny==2){
          rozmycie(&obrazek1, &opcje);
      }
      if (opcje.negatyw==1){
        negatyw(&obrazek1); 
      }
      if (opcje.progowanie==1 && obrazek1.magiczny==2){
        progowanie(&obrazek1, &opcje);
      }
      if (opcje.wyswietl==1){  /*User chce wyswietliÄ? Zapisz efekt i wyswietl zapisane*/
        if (opcje.plik_wy==stdout){  /*tymczasowo zamieniamy strumien wyjsciowy, zeby display sobie poradzil*/
          if (obrazek1.magiczny==2){ /*dla P2 bedzie to temporary_plik.pgm*/
            opcje.zapis_nazwa="temporary_plik.pgm";
            opcje.plik_wy=fopen("temporary_plik.pgm", "w");
            zapisz(&obrazek1, &opcje); /*Musi tu byc, zeby bylo co wyswietlic w pliku*/
            wyswietl(&opcje); /*Wyswietlamy pod strumieniem tymczasowym*/
            fclose(opcje.plik_wy); 
            opcje.plik_wy=stdout; /*Przywracamy jak bylo*/
            zapisz(&obrazek1, &opcje);
          }
          if (obrazek1.magiczny==3){ /*dla P3 bedzie to temporary_plik.ppm*/
            opcje.zapis_nazwa="temporary_plik.ppm";
            opcje.plik_wy=fopen("temporary_plik.ppm", "w");
            zapisz(&obrazek1, &opcje); /*Musi tu byc, zeby zeby bylo co wyswietlic w pliku*/
            wyswietl(&opcje); /*Wyswietlamy pod strumieniem tymczasowym*/
            fclose(opcje.plik_wy); 
            opcje.plik_wy=stdout; /*Przywracamy jak bylo*/
            zapisz(&obrazek1, &opcje);
          }

        } else {
          zapisz(&obrazek1, &opcje); /*Musi tu byc, zeby dalo sie wyswietlic nieedytowany plik*/
          wyswietl(&opcje); 
        }
      }
      if (opcje.wyswietl==0){  /*User nie chce wyswietliÄ? Tylko zapisz*/
        zapisz(&obrazek1, &opcje);
      }
    }

    /*Usuniecie plikow tymczasowych dla funkcji wyswietl przy konczeniu pracy programu.*/
    system("rm temporary_plik.pgm");
    system("rm temporary_plik.ppm");
    return 0;
  } /*koniec if (kodbledu==0)*/



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
#define MAX 2048            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */


int progowanie(Obraz *obrazek1, t_opcje *opcje){

 float wartosc_progu = opcje->w_progu; 

 wartosc_progu = (wartosc_progu/100)*(obrazek1->szarosci); //wartosc progu procentowo


  for (int i=0; i<obrazek1->wymy; i++) {
    for (int j=0; j<obrazek1->wymx; j++) {
      if (wartosc_progu < obrazek1->tab[i][j]){
        obrazek1->tab[i][j] = obrazek1->szarosci;
      }
    }
  }
  return obrazek1->wymy *  obrazek1->wymx;  // zwrocenie pikseli
}


int konturowanie(Obraz *obrazek1){

 for (int i=0; i<obrazek1->wymy-1; i++) {
    for (int j=0; j<obrazek1->wymx-1; j++) {
      obrazek1->tab[i][j] = (abs(obrazek1->tab[i+1][j] - obrazek1->tab[i][j]) + abs(obrazek1->tab[i][j+1] - obrazek1->tab[i][j]) );
    }
  }
  return obrazek1->wymy *  obrazek1->wymx;  // zwrocenie pikseli

}

int negatyw(Obraz *obrazek1){

  if(obrazek1->magiczny==2){  //dla pgm
    for (int i=0; i<obrazek1->wymy; i++) {
      for (int j=0; j<obrazek1->wymx; j++) {
        obrazek1->tab[i][j] = obrazek1->szarosci - obrazek1->tab[i][j];
      }
    }
  }

  /*negatywowanie, piksel po pikselu*/
  if(obrazek1->magiczny==3){  //dla ppm
    for (int i=0; i<obrazek1->wymy; i++) {
      for (int j=0; j<obrazek1->wymx*3; j++) {
        obrazek1->tab[i][j] = obrazek1->szarosci - obrazek1->tab[i][j];
      }
    }
  }

  return obrazek1->wymy * obrazek1->wymx;  /// zwrocenie pikseli
}



int konwersja(Obraz *obrazek1){
  int x=0;                 

  obrazek1->magiczny=2;  /*zamiana nr magicznego na .pgm*/


  for (int i=0; i<obrazek1->wymy;i++) {                                           //scalenie kolorow, usredniajac wartosc
    for (int j=0; j<obrazek1->wymx; j++) {
      obrazek1->tab[i][j] = ((obrazek1->tab[i][x]+obrazek1->tab[i][x+1]+obrazek1->tab[i][x+2])/3);
      x=x+3;
    }
     x=0; //zeby dzialalo dla kazdego wiersza
  }
  x=0; //zeby dzialala wiecej razy
}

int rozmycie(Obraz *obrazek1, t_opcje *opcje)
{
  int rozmycie = opcje->w_rozmycia;

  if(rozmycie<=0)
	  printf("ERROR - promien rozmycia mniejszy lub rowny zero");
  else
    {
	    for(int i=0; i<obrazek1->wymy; i++){
		    for(int j=0; j<obrazek1->wymx; j++){
		    obrazek1->tab[i][j]=(obrazek1->tab[i][j-rozmycie] + obrazek1->tab[i][j] + obrazek1->tab[i][j + rozmycie])/3;
        }
  	}
  }
  return obrazek1->wymy * obrazek1->wymx;  /// zwrocenie pikseli
}



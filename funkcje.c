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

int czytaj(Obraz *obrazek1, t_opcje *opcje){

  char nazwa[DL_LINII];    /* bufor do czytania nazwy pliku. */
  char buf[DL_LINII];      /* bufor do czytania naglowka i komentarzy. */
  int znak;                /* zmienna do czytania komentarzy. */
  int koniec=0;            /* czy napotkano koniec danych w pliku. */


  /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (opcje->plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  } else {
  /* Sprawdzenie "numeru magicznego" - powinien byc P2 lub P3*/
  if (fgets(buf,DL_LINII,opcje->plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || ( (buf[1]!='2') && (buf[1]!='3') ) || koniec) {  /* Czy jest magiczne "P2" lub "P3"? */
    fprintf(stderr,"Blad: To nie jest plik PGM, ani PPM\n");
    return(0);
  } else { /* plik jest "P2" lub "P3". Zapamietaj to!*/ 
    if(buf[1]=='2'){
    obrazek1->magiczny=2; 
    }
    if(buf[1]=='3'){
      obrazek1->magiczny=3;
    }
    if(buf[1]!='2' && buf[1]!='3'){
      fprintf(stderr,"bufor wczytywania textu zepsuty");
    }
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(opcje->plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,opcje->plik_we)==NULL)  /* Przeczytaj ja do bufora                */
        koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,opcje->plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                                /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(opcje->plik_we,"%d %d %d",&(obrazek1->wymx),&(obrazek1->wymy),&(obrazek1->szarosci))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }

  if(obrazek1->magiczny==2){  /*alokacja dla obrazow pgm*/
    obrazek1->tab=(int**)malloc(obrazek1->wymy*sizeof(int*));

    for (int k = 0; k < obrazek1->wymy; k++){
      obrazek1->tab[k]=(int*)malloc(obrazek1->wymx*sizeof(int));
    }
    if(obrazek1->tab==NULL){ //sprawdzenie czy udalo sie przydzielic pamiec
      fprintf(stderr,"Nie mozna przydzielic pamieci\n");
    } 
  }
  if(obrazek1->magiczny==3){ //alokacja dla obrazow ppm
    obrazek1->tab=(int**)malloc(obrazek1->wymy*sizeof(int*));
    for (int k = 0; k < obrazek1->wymy; k++){
      obrazek1->tab[k]=(int*)malloc(obrazek1->wymx*sizeof(int)*3); //potrajamy pamiec dla wymiaru wymx
    }
    if(obrazek1->tab==NULL){ /*sprawdzenie czy pamiec sie przydzielila*/
      fprintf(stderr,"Nie mozna przydzielic pamieci\n");
    } 
  }

  /* Pobranie obrazu i zapisanie w tablicy obraz*/
    if(obrazek1->magiczny==2){  //dla pgm
      for (int i=0;i<obrazek1->wymy;i++) {
        for (int j=0;j<obrazek1->wymx;j++) {
          if (fscanf(opcje->plik_we,"%d",&(obrazek1->tab[i][j]))!=1) {
            fprintf(stderr,"Blad: zle wymiary obrazu\n");
            return(0);
          }
        }
      }
    }
    if(obrazek1->magiczny==3){ //dla ppm
      for (int i=0;i<obrazek1->wymy;i++) {
        for (int j=0;j<obrazek1->wymx*3;j++) {
          if (fscanf(opcje->plik_we,"%d",&(obrazek1->tab[i][j]))!=1) {
            fprintf(stderr,"Blad: zle wymiary obrazu\n");
            return(0);
          }
        }
      }
    }
  return obrazek1->wymy *  obrazek1->wymx;   //zwrocenie wczytanych pikseli

  }
}



int zapisz(Obraz *obrazek1, t_opcje *opcje){
  int i,j;                     /* zmienne dla pentli for */
  char polecenie[DL_LINII];    /* bufor do zestawienia polecenia */

  /* zapis komentarza, znaku magicznego i parametrow obrazu  */
  fprintf(opcje->plik_wy, "P%d\n", obrazek1->magiczny);
  fprintf(opcje->plik_wy, "# edytowany programemem w trybie BASZ\n");
  fprintf(opcje->plik_wy, "%d %d\n %d\n", obrazek1->wymx, obrazek1->wymy, obrazek1->szarosci);

  /* zapis pikseli*/
  if(obrazek1->magiczny==2){  /*obraz jest pgm? zapisuj normalnie*/
    for (i=0;i<obrazek1->wymy;i++) {
      for (j=0;j<obrazek1->wymx;j++) {
        fprintf(opcje->plik_wy,"%d ",obrazek1->tab[i][j]);
      }
    }
  }
  if(obrazek1->magiczny==3){ /*obraz jest ppm? zapisuj potrĂłjny wymiar x*/
    for (i=0;i<obrazek1->wymy;i++) {
      for (j=0;j<obrazek1->wymx*3;j++) {
        fprintf(opcje->plik_wy,"%d ",obrazek1->tab[i][j]);
      }
    }
  }

  return obrazek1->wymx*obrazek1->wymy;  //zwrocenie wczytanych pikseli
}


void wyswietl(t_opcje *opcje){
    char polecenie[DL_LINII];

    strcpy(polecenie, "display ");
    strcat(polecenie, opcje->zapis_nazwa);
    strcat(polecenie, " &");
    system(polecenie);
}

int przetwarzaj_opcje(int argc, char ** argv, t_opcje *wybor) {
  int i, prog;
  char *nazwa_pliku_we;
  char *zapis_nazwa;
  char polecenie[DL_LINII];

  //zerowanie wszystkich opcji wyboru funkcji programu
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->wyswietl=0;


  wybor->plik_wy=stdout;          //gdy nie podano opcji "-o", 


  if (argc>1) {                 //sprawdza po kolei argumenty wywolania
  for (i=1; i<argc; i++) {
      if (argv[i][0] != '-'){  // jak nie ma - to zla opcja
        return B_NIEPOPRAWNAOPCJA;
      }
      switch (argv[i][1]) {
        case 'i': {                      //wczytanie pliku wej
            if (++i<argc) {                //nast arg nazwa pliku wej
              nazwa_pliku_we=argv[i];
              if (strcmp(nazwa_pliku_we,"-")==0){ // gdy nazwa jest "-" to dajemy wejscie na stdin
                wybor->plik_we=stdin;             
              } else {                               
                wybor->plik_we=fopen(nazwa_pliku_we,"r"); //otwarcie plik
              }
            } else {
              return B_BRAKNAZWY;                   // brak nazwy pliku
              }
            break;
        }
        case 'o': {                   
            if (++i<argc) {   //nazwa pliku wej
              zapis_nazwa=argv[i];
              if (strcmp(zapis_nazwa,"-")==0){ // gdy nazwa jest - to dajemy na stdout
                wybor->plik_wy=stdout;          
              } else{                             //otwarcie pliku
                wybor->plik_wy=fopen(zapis_nazwa,"w");
                }
            } else {
                return B_BRAKNAZWY;                   // brak nazwy pliku
              }
            break;
        }
        case 'p': {                           //progowanie
            if (++i<argc) {                   //wartosc progu
              if (sscanf(argv[i],"%d",&prog)==1) {
                wybor->progowanie=1;
                wybor->w_progu=prog;
              } else {
                return B_BRAKWARTOSCI;     
                }
            } else {
              return B_BRAKWARTOSCI;             //nie podano progu
              }
          break;
        }
        case 'm': {
            if (++i<argc) { //jaki kolor do obrobki
              if (argv[i][0]=='r') {
                wybor->r=1;
              } 
              else if (argv[i][0]=='g') {
                wybor->g=1;
              } 
              else if (argv[i][0]=='b') {
                wybor->b=1;
              } 
              else if (argv[i][0]=='s') {  //konwersja do szarosci
                wybor->konwersja=1;
              } 
              //
              else if (argv[i][0] != 'r' && argv[i][0] != 'g' && argv[i][0] != 'b' && argv[i][0] != 's') {
                return B_NIEPOPRAWNAOPCJA;   //zly znak
              }      
            }
            break;
        }

        case 'k': {                 //konturowanie
            wybor->konturowanie=1;
            break;
        }
        case 'n': {                 //negatyw
            wybor->negatyw=1;
            break;
        }
        case 'd': {                 //wyswietl
            wybor->wyswietl=1;
            break;
        }

        default: { 
          return B_NIEPOPRAWNAOPCJA;}    //nieznana opcja
      } 
      wybor->zapis_nazwa=zapis_nazwa;
    }  

     if (wybor->plik_we!=NULL){     //strumien zainicjowany
        return W_OK;
      }
      if (wybor->plik_we==NULL) {
        return B_BRAKPLIKU;          //plik nieotwarty
      }
  } 
} 

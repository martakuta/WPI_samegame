//Marta Markocka
//zadanie 'przeksztalcenie'
//4.12.2018r.

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MNOZNIK 3
#define DZIELNIK 2
#define PUSTA 1
#define NIEPUSTA 0

int max (int a, int b) {
	
	if (a>b)
		return a;
	else
		return b;
}

char *czytaj (int *ile_znakow) {
	
	char *wynik = NULL;
	int rozmiar = 0, i, znak;

	for (i=0; ((znak=getchar()) != EOF); ++i) {
		if ((i==0 && znak=='\n') || (i>0 && znak=='\n' && wynik[i-1]=='\n')) {
			i--;
		} else {
			assert (i<INT_MAX);
			if (i==rozmiar) {
				rozmiar = 1+rozmiar*MNOZNIK/DZIELNIK;
				wynik = realloc (wynik, rozmiar*sizeof*wynik);
				assert (wynik!=NULL);
			}
			wynik[i] = znak;
		}
	}
	
	*ile_znakow = i;
	return wynik;
}

int *szukaj_koncow_wierszy (int ile_znakow, char znaki[], int *ile_wierszy) {
	
	int *wynik = NULL;
	int rozmiar = 0, ktory_wiersz = 0;
	
	for (int i=0; i<ile_znakow; i++) {
		if (znaki[i]=='\n') {
			if (ktory_wiersz==rozmiar) {
				rozmiar = 1+rozmiar*MNOZNIK/DZIELNIK;
				wynik = realloc (wynik, rozmiar*sizeof*wynik);
				assert (wynik!=NULL);
			}
			wynik[ktory_wiersz] = i;
			ktory_wiersz++;
		}
	}
	
	*ile_wierszy = ktory_wiersz;
	return wynik;
}

int dlugosc_wiersza (int i, int konce_wierszy[]) {
	
	if (i==0)
		return konce_wierszy[i];
	else
		return konce_wierszy[i] - konce_wierszy[i-1] - 1;
}

int liczba_kolumn (int wiersze, int konce_wierszy[]) {
	
	int max_dlugosc_wiersza = 0;
	
	for (int i=0; i<wiersze; i++) {
		int dlugosc = dlugosc_wiersza (i, konce_wierszy);
		max_dlugosc_wiersza = max (dlugosc, max_dlugosc_wiersza);
	}
	
	return max_dlugosc_wiersza;
}

char *porzadkuj (char znaki1[], int wiersze[], int kolumny, int *kolumny2, int *wiersze2, int ile_wierszy) {
	
	char *wynik = malloc (ile_wierszy*kolumny*sizeof(char));
	int ile_kolumn = 0;
	
	for (int i=0; i<kolumny; i++) {
		//dla kazdej kolejnej kolumny sprawdza czy jest pusta
		int czy_pusta_kolumna = PUSTA;
		if (znaki1[i]!=' ' && dlugosc_wiersza (0, wiersze)>i)
			czy_pusta_kolumna = NIEPUSTA;
		for (int j=1; j<ile_wierszy && czy_pusta_kolumna==PUSTA; j++) {
			if (dlugosc_wiersza (j, wiersze)>i) {
				int id = wiersze[j-1]+i+1;
				if (znaki1[id]!=' ') {
					czy_pusta_kolumna = NIEPUSTA;
				}
			}
		}
		//jesli nie jest pusta to wpisuje ja do nowej tablicy
		if (czy_pusta_kolumna==NIEPUSTA) {
			if (dlugosc_wiersza (0, wiersze)>i) {
				wynik[ile_kolumn] = znaki1[i];
			} else {
				wynik[ile_kolumn] = ' '; 
			}
			for (int j=1; j<ile_wierszy; j++) {
				int id_wyniku = j*kolumny+ile_kolumn;
				int id_znaku;
				if (dlugosc_wiersza (j, wiersze)>i) {
					id_znaku = wiersze[j-1]+i+1;
					wynik[id_wyniku] = znaki1[id_znaku];
				} else {
					wynik[id_wyniku] = ' ';
				}
			}
			ile_kolumn++;
		}
	}
	
	*kolumny2 = ile_kolumn;
	//nastepnie zrzuca w dol wszystkie mozliwe znaki
	//oraz liczy maksymalna wysokosc kolumny, czyli liczbe wierszy po uporzadkowaniu
	int najmniej_pustych = ile_wierszy;
	for (int i=0; i<ile_kolumn; i++) {
		int szuka = (ile_wierszy-1)*kolumny+i;
		int wpisuje;
		while (szuka>=i && wynik[szuka]!=' ')
			szuka -= kolumny;
		wpisuje = szuka;
		while (szuka>=i) {
			if (wynik[szuka]!=' ') {
				wynik[wpisuje] = wynik[szuka];
				wpisuje -= kolumny;
			}
			szuka -= kolumny;
		}
		int licz_puste = 0;
		while (wpisuje>=i) {
			wynik[wpisuje] = ' ';
			licz_puste++;
			wpisuje -= kolumny;
		}
		if (licz_puste<najmniej_pustych)
			najmniej_pustych = licz_puste;
	}
	
	*wiersze2 = ile_wierszy-najmniej_pustych;
	return wynik;
}

char *skaluj_tablice (char znaki[], int kolumny1, int wiersze1, int kolumny2, int wiersze2) {
	
	char *wynik = malloc (kolumny2*wiersze2*sizeof(char));
	int roznica_wiersze = wiersze1-wiersze2;
	
	for (int i=0; i<wiersze2; i++) {
		for (int j=0; j<kolumny2; j++) {
			int id1 = (i+roznica_wiersze)*kolumny1+j;
			int id2 = i*kolumny2+j;
			wynik[id2] = znaki[id1];
		}
	}
	
	return wynik;
}

void usuwaj (char **znaki, int kolumny, int wiersze, int id, char ten_znak) {
	
	*(*znaki+id) = ' ';
	
	if (id%kolumny!=0) { //tzn ze ma lewego sasiada w tabeli
		if ((*(*znaki+id-1)) == ten_znak)
			usuwaj (znaki, kolumny, wiersze, id-1, ten_znak);
	}
	if (id%kolumny!=kolumny-1) { //tzn ze ma prawego sasiada w tabeli
		if ((*(*znaki+id+1)) == ten_znak)
			usuwaj (znaki, kolumny, wiersze, id+1, ten_znak);
	}
	if (id>=kolumny) { //tzn ze ma gornego sasiada
		if ((*(*znaki+id-kolumny)) == ten_znak)
			usuwaj (znaki, kolumny, wiersze, id-kolumny, ten_znak);
	}
	if (id<(wiersze-1)*kolumny) { //tzn ze ma dolnego sasiada
		if ((*(*znaki+id+kolumny)) == ten_znak)
			usuwaj (znaki, kolumny, wiersze, id+kolumny, ten_znak);
	}
}

char *ostatnie_poprawki (char *znaki, int kolumny2, int wiersze2, int *kolumny3, int *wiersze3) {
	
	char *wynik = malloc (wiersze2*kolumny2*sizeof(char));
	int najmniej_pustych = wiersze2;
	int puste_kolumny = 0;
	
	for (int i=0; i<kolumny2; i++) {
		int szuka = (wiersze2-1)*kolumny2+i;
		int wpisuje;
		while (szuka>=i && znaki[szuka]!=' ') {
			wynik[szuka-puste_kolumny] = znaki[szuka];
			szuka -= kolumny2;
		}
		wpisuje = szuka-puste_kolumny;
		while (szuka>=i) {
			if (znaki[szuka]!=' ') {
				wynik[wpisuje] = znaki[szuka];
				wpisuje -= kolumny2;
			}
			szuka -= kolumny2;
		}
		if (wpisuje==(wiersze2-1)*kolumny2+i-puste_kolumny) {
			puste_kolumny ++;
		} else {
			int licz_puste = 0;
			while (wpisuje>=i-puste_kolumny) {
				wynik[wpisuje] = ' ';
				licz_puste++;
				wpisuje -= kolumny2;
			}
			if (licz_puste<najmniej_pustych)
				najmniej_pustych = licz_puste;
		}
	}
	
	*wiersze3 = wiersze2-najmniej_pustych;
	*kolumny3 = kolumny2-puste_kolumny;
	return wynik;
}

void pisz_wynik (char znaki[], int wiersze, int kolumny) {
	
	for (int i=0; i<wiersze; i++) {
		int ostatni_znak_niebialy;
		for (int j=0; j<kolumny; j++)  {
			int id = i*kolumny+j;
			if (znaki[id]!=' ')
				ostatni_znak_niebialy = j;
		}
		for (int j=0; j<=ostatni_znak_niebialy; j++)
			printf ("%c", znaki[i*kolumny+j]);
		printf ("\n");
	}
}

int main () {
	
	//wczyta kolejno znaki i zapisze indeksy ich koncow
	//policzy liczbe wierszy i kolumn wejscia
	int ile_znakow, wiersze1;
	char *znaki1 = czytaj (&ile_znakow);
	int *konce_wierszy = szukaj_koncow_wierszy (ile_znakow, znaki1, &wiersze1);
	int kolumny1 = liczba_kolumn (wiersze1, konce_wierszy);
	
	//zamieni tablice na 'prostokatna', ale jednowymiarowa i indeksowana kolejno wierszami
	//uporzadkuje zgodnie z wytycznymi, policzy nowa liczbe wierszy i kolumn
	int kolumny2, wiersze2 = wiersze1;
	char *znaki2 = porzadkuj (znaki1, konce_wierszy, kolumny1, &kolumny2, &wiersze2, wiersze1);
	free (znaki1);
	free (konce_wierszy);
	
	//przeskaluje tablice do nowej liczby wierszy i kolumn
	char *znaki3 = skaluj_tablice (znaki2, kolumny1, wiersze1, kolumny2, wiersze2);
	free (znaki2);
	//jesli wejscie bylo puste lub zawieralo wylacznie entery tutaj zakonczy dzialanie
	if (kolumny2==0 && wiersze2==0) {
		free (znaki3);
		return 0;
	}
	
	//usunie grupe znakow z lewego dolnego rogu
	int id = (wiersze2-1)*kolumny2;
	usuwaj (&znaki3, kolumny2, wiersze2, id, znaki3[id]);
	
	//uporzadkuje dane raz jeszcze i policzy nowa liczbe kolumn i wierszy
	int kolumny3, wiersze3;
	char *znaki4 = ostatnie_poprawki (znaki3, kolumny2, wiersze2, &kolumny3, &wiersze3);
	free (znaki3);
	
	//przeskalowuje tablice do nowej liczby kolumn i wierszy
	char *znaki5 = skaluj_tablice (znaki4, kolumny2, wiersze2, kolumny3, wiersze3);
	free (znaki4);
	
	//wypisze wynik pomijajac spacje na koncach wierszy
	pisz_wynik (znaki5, wiersze3, kolumny3);
	free (znaki5);
	
	return 0;
}

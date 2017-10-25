// JeszkoAGwiazdka.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
using namespace std;

int grid[22][22];		// mapa gridu - wczytywanie
int G[22][22];        	// koszt ruchu z punktu STARTOWEGO do KONCOWEGO	
float H[22][22];		// HEURYSTYKA
float F[22][22];		// F = G + H
short pole_typ[22][22];	// 0 = pole nieodwiedzone, 1 = pole na liscie otwarte, 2 = pole na lisce zamknietej, 3 = sciezka, 5 = przeszkoda
short rodzic[22][22];	// 1 = gora, 2 = dol, 3 = prawo, 4 = lewo
int kopia_grid[22][22];	// kopia mapy, finalnie wyswietlana
int i = 0;				// zmienne pomocnicze do for()
int j = 0;				//
int pomocX;				// zmienna pomocnicza by ustawic obecnyX
int pomocY;				//
bool czyRodzic = false;	// przy konfligdzie gdy bedzie dwoje rodzicow i jedno dziecko, pomoc do mniejszyF(int x, int y);
int ahaX = 0;			// przypisuje do obecnegoX w mniejszyF()
int ahaY = 0;			//
int obecnyX = 20;		// obecny punkt (ustawiony tez jako startowy)
int obecnyY = 1;		//
int malutki = 0;		// pomoc, do mniejszego F
bool potwierdzenie = false;	// pomoc do listy otwartej, gdy nie ma listy otwartej zatrzymuje program
int koniecX = 1;		// punkt do rysowania sciezki
int koniecY = 20;		//
int naPoczatekX;		// pomoc w ustawieniu punktu do rysowania sciezki
int naPoczatekY;		//
int ileG;				// liczy ile wartosc ma dane G[][]

						
void mapa() {

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			grid[i][j] = 7;
		}
	}

	ifstream plik{ "grid15.txt" };

	//zapisywanie liczb z pliku do tablicy grid[20][20], jezeli plik jest otwarty
	if (plik.is_open() == true) {
		for (int y = 0; y < 22; y++) {
			for (int x = 0; x < 22; x++) {
				if (x == 0 || x == 21) {
					continue;
				}
				else if (y == 0 || y == 21) {
					continue;
				}
				else {
					plik >> grid[y][x];
				}

			}
		}
		plik.close();

		//wyswietlanie tablicy grid[20][20]
		for (int i = 0; i < 22; i++) {
			for (int j = 0; j < 22; j++) {
				cout << grid[i][j] << " ";
			}
			cout << endl;
		}
	}
	else { //w przypadku gdy sie nie udalo wczytac pliku 
		cout << "\n" << endl;
		cout << "Nie wczytano pliku, sprawdz dane." << endl;
		getchar();
		cout << "\n" << endl;
	}
}

// tablica pomocnicza do pole_typ[][]
void typ_pola() {
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			if (i == 0 || i == 21) {
				continue;
			}
			else if (j == 0 || j == 21) {
				continue;
			}
			else {
				cout << pole_typ[i][j] << " ";
			}
		}
		cout << endl;
	}
}

// tablica pomocnicza rodzic[][]
void tablica_rodzic() {
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			if (i == 0 || i == 21) {
				continue;
			}
			else if (j == 0 || j == 21) {
				continue;
			}
			else {
				cout << rodzic[i][j] << " ";
			}
		}
		cout << endl;
	}
}

void liczG() {
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			cout << G[i][j] << " ";
		}
		cout << endl;
	}
}

void liczH() {
	float pierwiastek = 0;
	for (i = 0; i < 22; i++) {
		for (j = 0; j < 22; j++) {
			if (i == 0 || i == 21) {
				continue;
			}
			else if (j == 0 || j == 21) {
				continue;
			}
			else {
				pierwiastek = sqrt(((i - 0)*(i - 0)) + ((j - 19)*(j - 19)));
				H[i][j] = round(pierwiastek * 100) / 100;
			}
		}
	}

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			cout << H[i][j] << " ";
		}
		cout << endl;
	}
}

void liczF() {
	for (i = 0; i < 22; i++) {
		for (j = 0; j < 22; j++) {
			if (i == 0 || i == 21) {
				continue;
			}
			else if (j == 0 || j == 21) {
				continue;
			}
			else {
				F[i][j] = G[i][j] + H[i][j];
			}
		}
	}
}

void mniejszeF() {
	float saladyn = 99;
	int dodaj = 0;
	for (i = 0; i < 22; i++) {
		for (j = 0; j < 22; j++) {
			if (i == 0 || i == 21) {
				continue;
			}
			else if (j == 0 || j == 21) {
				continue;
			}
			else if (1 == pole_typ[i][j] && F[i][j] < saladyn) {
				saladyn = F[i][j];
				ahaX = i;
				ahaY = j;
				dodaj += 1;
			}
		}
	}
	if (dodaj == 0) {
		potwierdzenie = true;
	}
	else {
		obecnyX = ahaX;
		obecnyY = ahaY;
		dodaj = 0;
	}
}

void mniejszyF(int x, int y) {
	float malutki = F[x][y];
	if (rodzic[x][y] == 1 && F[x + 1][y] < malutki) {
		malutki = F[x + 1][y];
		czyRodzic = true;
	}
	if (rodzic[x][y] == 4 && F[x][y + 1] < malutki) {
		malutki = F[x][y + 1];
		czyRodzic = true;
	}
	if (rodzic[x][y] == 3 && F[x][y - 1] < malutki) {
		malutki = F[x][y - 1];
		czyRodzic = true;
	}
	if (rodzic[x][y] == 2 && F[x + 1][y] < malutki) {
		malutki = F[x + 1][y];
		czyRodzic = true;
	}
}

void ruch(int x, int y) {
	if (grid[x][y] != 7) {
		if ((grid[x][y] == 5) || (pole_typ[x][y] == 2)) {
			if (grid[x][y] == 5) pole_typ[x][y] = 5;
		}
		else if (pole_typ[x][y] == 1) {
			liczF();

			mniejszyF(x, y);
		}
		else {
			pole_typ[x][y] = 1;
			czyRodzic = true;
			G[x][y] = G[obecnyX][obecnyY] + 1;
		}
	}
}

void kierunek(int x, int y) {


	////////////////////////////////
	int pomocXc = x;
	int pomocYc = y + 1;

	ruch(pomocXc, pomocYc);
	if (czyRodzic == true) {
		rodzic[pomocXc][pomocYc] = 3;
		czyRodzic = false;
	}

	////////////////////////////////
	int pomocXb = x;
	int pomocYb = y - 1;

	ruch(pomocXb, pomocYb);
	if (czyRodzic == true) {
		rodzic[pomocXb][pomocYb] = 4;
		czyRodzic = false;
	}

	////////////////////////////////
	int pomocXg = x - 1;
	int pomocYg = y;

	ruch(pomocXg, pomocYg);
	if (czyRodzic == true) {
		rodzic[pomocXg][pomocYg] = 1;
		czyRodzic = false;
	}

	////////////////////////////////
	int pomocXd = x + 1;
	int pomocYd = y;

	ruch(pomocXd, pomocYd);
	if (czyRodzic == true) {
		rodzic[pomocXd][pomocYd] = 2;
		czyRodzic = false;
	}

}

void rysuj(int x, int y) {
	if (G[x - 1][y] == ileG - 1) {
		kopia_grid[x - 1][y] = 3;
		naPoczatekX = x - 1;
		naPoczatekY = y;
	}
	else if (G[x + 1][y] == ileG - 1) {
		kopia_grid[x + 1][y] = 3;
		naPoczatekY = y;
		naPoczatekX = x + 1;
	}
	else if (G[x][y - 1] == ileG - 1) {
		kopia_grid[x][y - 1] = 3;
		naPoczatekY = y - 1;
		naPoczatekX = x;
	}
	else if (G[x][y + 1] == ileG - 1) {
		kopia_grid[x][y + 1] = 3;
		naPoczatekX = x;
		naPoczatekY = y + 1;
	}

	ileG = ileG - 1;
	koniecX = naPoczatekX;
	koniecY = naPoczatekY;
}

void kopia_mapa() {
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			kopia_grid[i][j] = grid[i][j];
		}
		cout << endl;
	}

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			cout << kopia_grid[i][j] << " ";
		}
		cout << endl;
	}
}

void the_end() {
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			cout << kopia_grid[i][j] << " ";
		}
		cout << endl;
	}
}

int main()
{
	cout << " MAPA GRID \n" << endl;
	mapa();
	cout << "\n KOSZT RUCHU G \n \n" << endl;
	liczG();
	cout << "\n HEURYSTYKA H \n \n" << endl;
	liczH();
	cout << "\n F \n \n" << endl;
	liczF();
	do {
		pole_typ[obecnyX][obecnyY] = 2; // zamknięte pole
		kierunek(obecnyX, obecnyY);
		typ_pola();
		mniejszeF();
		cout << "\n" << endl;
		cout << obecnyX << " " << obecnyY << endl;
		if (potwierdzenie == true) {
			cout << "\n Brak sciezki, sciany" << endl;
			getchar();
		}
		system("cls");
	} while (G[1][20] == 0);

	kopia_mapa();
	kopia_grid[20][1] = 3;
	kopia_grid[1][20] = 3;
	do {
		ileG = G[koniecX][koniecY];
		rysuj(koniecX, koniecY);
		system("cls");
		the_end();
	} while (G[koniecX][koniecY] != 1);

	cout << "ZWYCIESTWO" << endl;

	cout << "\n Wielkosc trasy: " << G[1][20] << endl;
	getchar();
	return 0;
}
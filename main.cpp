//Grupo: Flaviu E. Hongu y David Palacios
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

const int numFichas = 55;

typedef short int tArray[numFichas];

typedef string tFichasJugador[numFichas];

string muestraFicha(short int numero1, short int numero2);

void mostrarTablero(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas, int numFichasJugador);

int mostrarMenu();

bool ejecutarOpcion(short int opcionMenu, tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int& numRobadas, short int& limite, int& numFichasJugador, tArray pozo1, tArray pozo2);

void ponerFichaDer(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite, int& numFichasJugador);

bool puedePonerDer(tFichasJugador fichasJugador, string tablero, int  numFichasJugador);

void ponerFichaIzq(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite, int& numFichasJugador);

bool puedePonerIzq(tFichasJugador fichasJugador, string tablero, int  numFichasJugador);

void robarFicha(tFichasJugador& fichasJugador, string tablero, short int& numRobadas, short int limite, int& numFichasJugador, tArray pozo1, tArray pozo2);

void reordenarFichas(tFichasJugador& fichasJugador, int posicion, int numFichasJugador);

void cambiarLimite(short int& limite); //Funcion peligrosa, el limite puede ser cambiado en mitad de la partida sin que esto
//haga reiniciar la partida, lo cual cambia un poco la experiencia de juego. Esto es a proposito, no obligo a reiniciar la partida
//por cambiar este limite maximo para los numeros de las fichas

string girarFicha(string ficha);

void guardarPartida(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas, int numFichasJugador);

void cargarPartida(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int& numRobadas, int& numFichasJugador);

void generarPozo(tArray& pozo1, tArray& pozo2, short int limiteMaximo);

void desordenarPozo(tArray& pozo1, tArray& pozo2, int numFichasPartida);

int calcularNumFichas(short int limiteMaximo);

void calcularPuntos(tFichasJugador fichasJugador, int numFichasJugador);


int main() {
	//Declaración de variables
	//Variables ajustables
	short int limiteMaximo = 6;
	cambiarLimite(limiteMaximo);
	tArray pozo1, pozo2;
	generarPozo(pozo1, pozo2, limiteMaximo);
	desordenarPozo(pozo1, pozo2, calcularNumFichas(limiteMaximo));
	short int numUsadas = 0;

	//Valores estáticos
	bool salir = false;
	tFichasJugador fichasJugador;
	for (int i = 0; i < 7; i++) {
		fichasJugador[i] = muestraFicha(pozo1[i], pozo2[i]);
	}
	string tablero = muestraFicha(pozo1[7], pozo2[7]);
	numUsadas = 7;
	int numFichasJugador = 7;
	short int numColocadas = 0;
	short int numRobadas = 0;
	int opcion = 0;
	//Codigo fuera del bucle de juego
	//Bucle de juego
	while (!salir) {
		mostrarTablero(fichasJugador, tablero, numColocadas, numRobadas, numFichasJugador);
		opcion = mostrarMenu();
		salir = ejecutarOpcion(opcion, fichasJugador, tablero, numColocadas, numRobadas, limiteMaximo, numFichasJugador, pozo1, pozo2);
		if (numFichasJugador == 0 && numUsadas != 54) robarFicha(fichasJugador, tablero, numRobadas, limiteMaximo, numFichasJugador, pozo1, pozo2);
		else if (numFichasJugador == 0 && numUsadas == 54) {
			salir = true;
			cout << "Has terminado la partida sin fichas que robar, enhorabuena, has ganado!" << endl;
		}
		else if (numUsadas == calcularNumFichas(limiteMaximo) && !puedePonerDer(fichasJugador, tablero, numFichasJugador) && !puedePonerIzq(fichasJugador, tablero, numFichasJugador)) {
			salir = true;
			calcularPuntos(fichasJugador, numFichasJugador);
		}
	}

	return 0;
}

string muestraFicha(short int numero1, short int numero2) {
	string fichaMostrada = "";
	fichaMostrada = '|';
	fichaMostrada += '0' + numero1;
	fichaMostrada += '-';
	fichaMostrada += '0' + numero2;
	fichaMostrada += '|';
	return fichaMostrada;
}

void mostrarTablero(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas, int numFichasJugador) {
	cout << "--------------------" << endl;
	cout << "|     TABLERO     |" << endl;
	cout << "--------------------" << endl;
	cout << tablero << endl;
	cout << "Fichas colocadas: " << numColocadas << " - ";
	cout << "Fichas robadas: " << numRobadas << endl;
	cout << "Fichas del jugador: " << endl;
	for (int i = 0; i < numFichasJugador; i++) {
		if ((i + 1) % 4 == 0) {
			cout << endl;
		}
		cout << fichasJugador[i] << " ";
	}
	cout << endl;
}

int mostrarMenu() {
	int opcion;

	cout << "--------------------" << endl;
	cout << "| MENU DE OPCIONES |" << endl;
	cout << "--------------------" << endl;
	cout << "[1] Colocar ficha a la derecha" << endl;
	cout << "[2] Colocar ficha a la izquierda" << endl;
	cout << "[3] Robar ficha nueva" << endl;
	cout << "[4] Guardar partida" << endl;
	cout << "[5] Cargar partida" << endl;
	cout << "[0] Salir" << endl << endl;
	cout << "Elija opcion: ";

	cin >> opcion;
	cout << endl;

	return opcion;
}

bool ejecutarOpcion(short int opcionMenu, tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int& numRobadas, short int& limite, int& numFichasJugador, tArray pozo1, tArray pozo2) {
	bool salir = false;

	switch (opcionMenu) {
	case 1: ponerFichaDer(fichasJugador, tablero, numColocadas, limite, numFichasJugador);
		break;
	case 2: ponerFichaIzq(fichasJugador, tablero, numColocadas, limite, numFichasJugador);
		break;
	case 3: robarFicha(fichasJugador, tablero, numRobadas, limite, numFichasJugador, pozo1, pozo2);
		break;
	case 4: guardarPartida(fichasJugador, tablero, numColocadas, numRobadas, numFichasJugador);
		break;
	case 5: cargarPartida(fichasJugador, tablero, numColocadas, numRobadas, numFichasJugador);
		break;
	case 0: salir = true;
	}

	return salir;
}

void ponerFichaDer(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite, int& numFichasJugador) {
	if (puedePonerDer(fichasJugador, tablero, numFichasJugador)) {
		int i;
		cout << "Elige que ficha colocar: ";
		cin >> i;
		cout << endl;
		if (tablero[tablero.size() - 2] == fichasJugador[i - 1][1]) {
			tablero += fichasJugador[i - 1];
			numColocadas++;
			reordenarFichas(fichasJugador, i, numFichasJugador);
			numFichasJugador--;
		}
		else if (tablero[tablero.size() - 2] == fichasJugador[i - 1][3]) {
			tablero += girarFicha(fichasJugador[i - 1]);
			numColocadas++;
			reordenarFichas(fichasJugador, i, numFichasJugador);
			numFichasJugador--;
		}
		else {
			cout << "No puedes colocar esa ficha a la derecha, elige otra." << endl;
		}
	}
	else {
		cout << "ERROR! No puedes colocar ninguna ficha a la derecha!" << endl;
		cout << "Escoge otra opcion." << endl;
	}
}

bool puedePonerDer(tFichasJugador fichasJugador, string tablero, int numFichasJugador) {
	bool puede = false;
	int i = 0;
	while (!puede && i < numFichasJugador) {
		if (tablero[tablero.size() - 2] == fichasJugador[i][1] || tablero[tablero.size() - 2] == fichasJugador[i][3]) puede = true;
		i++;
	}
	return puede;
}

void ponerFichaIzq(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite, int& numFichasJugador) {
	if (puedePonerIzq(fichasJugador, tablero, numFichasJugador)) {
		int i;
		cout << "Elige que ficha colocar: ";
		cin >> i;
		cout << endl;
		if (tablero[1] == fichasJugador[i - 1][3]) {
			tablero = fichasJugador[i - 1] + tablero;
			numColocadas++;
			reordenarFichas(fichasJugador, i, numFichasJugador);
			numFichasJugador--;
		}
		else if (tablero[1] == fichasJugador[i - 1][1]) {
			tablero = girarFicha(fichasJugador[i - 1]) + tablero;
			numColocadas++;
			reordenarFichas(fichasJugador, i, numFichasJugador);
			numFichasJugador--;
		}
		else {
			cout << "No puedes colocar esa ficha a la izquierda, elige otra." << endl;
		}
	}
	else {
		cout << "ERROR! No puedes colocar ninguna ficha a la izquierda!" << endl;
		cout << "Escoge otra opcion." << endl;
	}
}

bool puedePonerIzq(tFichasJugador fichasJugador, string tablero, int numFichasJugador) {
	bool puede = false;
	int i = 0;
	while (!puede && i < numFichasJugador) {
		if (tablero[1] == fichasJugador[i][1] || tablero[1] == fichasJugador[i][3]) puede = true;
		i++;
	}
	return puede;
}

void robarFicha(tFichasJugador& fichasJugador, string tablero, short int& numRobadas, short int limite, int& numFichasJugador, tArray pozo1, tArray pozo2) {
	if (numRobadas + 8 == calcularNumFichas(limite)) {
		cout << "Ya no quedan fichas que robar!" << endl;
	}
	else if (!puedePonerDer(fichasJugador, tablero, numFichasJugador) && !puedePonerIzq(fichasJugador, tablero, numFichasJugador)) {
		fichasJugador[numFichasJugador] = muestraFicha(pozo1[8 + numRobadas], pozo2[8 + numRobadas]);
		numRobadas++;
		numFichasJugador++;
	}
	else {
		cout << "No puedes robar. Mira mejor, que puedes colocar." << endl;
	}
}

void reordenarFichas(tFichasJugador& fichasJugador, int posicion, int numFichasJugador) {
	for (int i = posicion; i < numFichasJugador; i++) {
		fichasJugador[i - 1] = fichasJugador[i];
	}
}

void cambiarLimite(short int& limite) {
	cout << "Escoge un limite para los numeros de las fichas entre 6 y 9: ";

	int i;
	cin >> i;

	if (i >= 6 && i <= 9) {
		limite = i;
	}
	else {
		cout << "Valor incorrecto para el limite! Vuelve a intentarlo." << endl;
	}
}

string girarFicha(string ficha) {
	string fichaGirada = "|x-x|";
	fichaGirada[1] = ficha[3];
	fichaGirada[3] = ficha[1];

	return fichaGirada;
}

void guardarPartida(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas, int numFichasJugador) {
	string nombrePartida;

	cout << "Nombre de la partida: ";
	cin >> nombrePartida;
	cout << endl;

	ofstream partida;
	partida.open(nombrePartida + ".txt");
	partida << numFichasJugador << endl;
	for (int i = 0; i < numFichasJugador; i++) {
		partida << fichasJugador[i] << endl;
	}
	partida << tablero << endl;
	partida << numColocadas << endl;
	partida << numRobadas << endl;
	partida.close();
}

void cargarPartida(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int& numRobadas, int& numFichasJugador) {
	string nombrePartida;
	cout << "Nombre de la partida a cargar: ";
	cin >> nombrePartida;
	cout << endl;
	nombrePartida += ".txt";

	int i = 0;
	string linea;
	ifstream partida(nombrePartida);
	if (partida.is_open()) {
		while (getline(partida, linea)) {
			if (i == 0) {
				numFichasJugador = stoi(linea);
			}
			else if (i - 1 < numFichasJugador) {
				fichasJugador[i - 1] = linea;
			}
			else if (i == numFichasJugador + 1) {
				tablero = linea;
			}
			else if (i == numFichasJugador + 2) {
				numColocadas = stoi(linea);
			}
			else if (i == numFichasJugador + 3) {
				numRobadas = stoi(linea);
			}
			i++;
		}
		partida.close();
	}
	else cout << "ERROR. No se pudo cargar partida!" << endl;
}

void generarPozo(tArray& pozo1, tArray& pozo2, short int limiteMaximo) {
	int posicion = 0;
	for (int i = 0; i <= limiteMaximo; i++) {
		for (int j = 0; j <= i; j++) {
			pozo1[posicion] = i;
			pozo2[posicion] = j;
			posicion++;
		}
	}
}

void desordenarPozo(tArray& pozo1, tArray& pozo2, int numFichasPartida) {
	srand(time(NULL));
	int ind1, ind2;
	short int _tmp1, _tmp2;
	for (int i = 0; i < 1000; i++) {
		ind1 = rand() % numFichasPartida;
		ind2 = rand() % numFichasPartida;
		if (ind1 != ind2) {
			_tmp1 = pozo1[ind1];
			_tmp2 = pozo2[ind1];
			pozo1[ind1] = pozo1[ind2];
			pozo2[ind1] = pozo2[ind2];
			pozo1[ind2] = _tmp1;
			pozo2[ind2] = _tmp2;
		}
	}
}


int calcularNumFichas(short int limiteMaximo) {
	int numFichasPartida = 0;
	for (int i = 0; i <= limiteMaximo; i++) {
		numFichasPartida += i + 1;
	}
	return numFichasPartida;
}
/*De esta manera se calcula el número de fichas a modo de variable, lo cual es mas sencillo pero
al no poder usar una variable como tamaño de un array, no se puede usar este metodo.*/

void calcularPuntos(tFichasJugador fichasJugador, int numFichasJugador) {
	int puntos = 0;
	for (int i = 0; i < numFichasJugador; i++) {
		int a = fichasJugador[i][1] - '0';
		int b = fichasJugador[i][3] - '0';
		puntos += a;
		puntos += b;
	}
	cout << "Has acabado la partida con " << puntos << " puntos." << endl;
}

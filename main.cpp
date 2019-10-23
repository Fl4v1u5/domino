#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

string generarFicha(short int limite);

string muestraFicha(string ficha);

void mostrarTablero(string fichaJugador, string tablero, short int numColocadas, short int numRobadas);

int mostrarMenu();

bool ejecutarOpcion(short int opcionMenu, string &fichaJugador, string &tablero, short int &numColocadas, short int &numRobadas, short int &limite);

void ponerFichaDer(string &fichaJugador, string &tablero, short int &numColocadas, short int limite);

bool puedePonerDer(string fichaJugador, string tablero);

void ponerFichaIzq(string &fichaJugador, string &tablero, short int &numColocadas, short int limite);

bool puedePonerIzq(string fichaJugador, string tablero);

void robarFicha(string &fichaJugador, string tablero, short int &numRobadas, short int limite);

void cambiarLimite(short int &limite); //Funcion peligrosa, el limite puede ser cambiado en mitad de la partida sin que esto
//haga reiniciar la partida, lo cual cambia un poco la experiencia de juego. Esto es a proposito, no obligo a reiniciar la partida
//por cambiar este limite maximo para los numeros de las fichas

string girarFicha(string ficha);

void guardarPartida(string fichaJugador, string tablero, short int numColocadas, short int numRobadas);

void cargarPartida(string &fichaJugador, string &tablero, short int &numColocadas, short int &numRobadas);


int main() {
	//Declaración de variables
	//Variables ajustables
	short int limiteMaximo = 6;
	cambiarLimite(limiteMaximo);
	//Valores estáticos
	bool salir = false;
	srand(time(NULL));
	string fichaJugador = muestraFicha(generarFicha(limiteMaximo));
	string tablero = muestraFicha(generarFicha(limiteMaximo));
	short int numColocadas = 0;
	short int numRobadas = 0;
	int opcion = 0;

	
		
	//Codigo fuera del bucle de juego

	//Bucle de juego
	while (!salir) {
		mostrarTablero(fichaJugador, tablero, numColocadas, numRobadas);
		opcion = mostrarMenu();
		salir = ejecutarOpcion(opcion, fichaJugador, tablero, numColocadas, numRobadas, limiteMaximo);
	}
	
    return 0;
}

string generarFicha(short int limite) {
	string ficha = "x-x";
	ficha[0] = '0' + rand() % (limite + 1);
	ficha[2] = '0' + rand() % (limite + 1);

	return ficha;
}

string muestraFicha(string ficha) {
	string fichaMostrada = "|" + ficha + "|";
	return fichaMostrada;
}

void mostrarTablero(string fichaJugador, string tablero, short int numColocadas, short int numRobadas) {
	cout << "--------------------" << endl;
	cout << "|     TABLERO     |" << endl;
	cout << "--------------------" << endl;
	cout << tablero << endl;
	cout << "Fichas colocadas: " << numColocadas << " - ";
	cout << "Fichas robadas: " << numRobadas << endl;
	cout << "Ficha jugador: " << fichaJugador << endl;
}

int mostrarMenu() {
	int opcion;

	cout << "--------------------" << endl;
	cout << "| MENU DE OPCIONES |" << endl;
	cout << "--------------------" << endl;
	cout << "[1] Colocar ficha a la derecha" << endl;
	cout << "[2] Colocar ficha a la izquierda" << endl;
	cout << "[3] Robar ficha nueva" << endl;
	cout << "[4] Cambiar limite maximo de numero" << endl;
	cout << "[5] Guardar partida" << endl;
	cout << "[6] Cargar partida" << endl;
	cout << "[0] Salir" << endl << endl;
	cout << "Elija opcion: ";

	cin >> opcion;
	cout << endl;
	
	return opcion;
}

bool ejecutarOpcion(short int opcionMenu, string &fichaJugador, string &tablero, short int &numColocadas, short int &numRobadas, short int &limite) {
	bool salir = false;

	switch (opcionMenu) {
		case 1: ponerFichaDer(fichaJugador, tablero, numColocadas, limite);
			break;
		case 2: ponerFichaIzq(fichaJugador, tablero, numColocadas, limite);
			break;
		case 3: robarFicha(fichaJugador, tablero, numRobadas, limite);
			break;
		case 4: cambiarLimite(limite);
			break;
		case 5: guardarPartida(fichaJugador, tablero, numColocadas, numRobadas);
			break;
		case 6: cargarPartida(fichaJugador, tablero, numColocadas, numRobadas);
			break;
		case 0: salir = true;
	}

	return salir;
}

void ponerFichaDer(string &fichaJugador, string &tablero, short int &numColocadas, short int limite) {
	if (puedePonerDer(fichaJugador, tablero)) {
		if (tablero[tablero.size() - 2] == fichaJugador[1]) {
			tablero += fichaJugador;
		}
		else {
			tablero += girarFicha(fichaJugador);
		}
		numColocadas++;
		fichaJugador = muestraFicha(generarFicha(limite));
	}
	else {
		cout << "ERROR! No puedes colocar la ficha a la derecha!" << endl;
		cout << "Escoge otra opcion." << endl;
	}
}

bool puedePonerDer(string fichaJugador, string tablero) {
	bool puede = false;
	if (tablero[tablero.size() - 2] == fichaJugador[1] || tablero[tablero.size() - 2] == fichaJugador[3]) puede = true;
	return puede;
}

void ponerFichaIzq(string &fichaJugador, string &tablero, short int &numColocadas, short int limite) {
	if (puedePonerIzq(fichaJugador, tablero)) {
		if (tablero[1] == fichaJugador[3]) {
			tablero = fichaJugador + tablero;
		}
		else {
			tablero = girarFicha(fichaJugador) + tablero;
		}
		numColocadas++;
		fichaJugador = muestraFicha(generarFicha(limite));
	}
	else {
		cout << "ERROR! No puedes colocar la ficha a la izquierda!" << endl;
		cout << "Escoge otra opcion." << endl;
	}
}

bool puedePonerIzq(string fichaJugador, string tablero) {
	bool puede = false;
	if (tablero[1] == fichaJugador[1] || tablero[1] == fichaJugador[3]) puede = true;
	return puede;
}

void robarFicha(string &fichaJugador, string tablero, short int &numRobadas, short int limite) {
	if (!puedePonerDer(fichaJugador, tablero) && !puedePonerIzq(fichaJugador, tablero)) {
		fichaJugador = muestraFicha(generarFicha(limite));
		numRobadas++;
	}
	else {
		cout << "No puedes robar. Mira mejor, que puedes colocar." << endl;
	}
}

void cambiarLimite(short int &limite) {
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

void guardarPartida(string fichaJugador, string tablero, short int numColocadas, short int numRobadas) {
	string nombrePartida;

	cout << "Nombre de la partida: ";
	cin >> nombrePartida;
	cout << endl;

	ofstream partida;
	partida.open(nombrePartida + ".txt");
	partida << fichaJugador << endl;
	partida << tablero << endl;
	partida << numColocadas << endl;
	partida << numRobadas << endl;
	partida.close();
}

void cargarPartida(string &fichaJugador, string &tablero, short int &numColocadas, short int &numRobadas) {
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
				fichaJugador = linea;
			}
			else if (i == 1) {
				tablero = linea;
			}
			else if (i == 2) {
				numColocadas = stoi(linea);
			}
			else if (i == 3) {
				numRobadas = stoi(linea);
			}
			i++;
		}
		partida.close();
	}
	else cout << "ERROR. No se pudo cargar partida!" << endl;
}
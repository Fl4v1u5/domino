#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

string muestraFicha(short int numero1, short int numero2);

void mostrarTablero(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas);

int mostrarMenu();

bool ejecutarOpcion(short int opcionMenu, tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int& numRobadas, short int& limite);

void ponerFichaDer(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite);

bool puedePonerDer(tFichasJugador fichasJugador, string tablero);

void ponerFichaIzq(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite);

bool puedePonerIzq(tFichasJugador fichasJugador, string tablero);

void robarFicha(tFichasJugador& fichasJugador, string tablero, short int& numRobadas, short int limite);

void cambiarLimite(short int& limite); //Funcion peligrosa, el limite puede ser cambiado en mitad de la partida sin que esto
//haga reiniciar la partida, lo cual cambia un poco la experiencia de juego. Esto es a proposito, no obligo a reiniciar la partida
//por cambiar este limite maximo para los numeros de las fichas

string girarFicha(string ficha);

void guardarPartida(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas);

void cargarPartida(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int& numRobadas);

/*Al tener como numero maximo en una ficha una cantidad variable, el numero de fichas es también
variable y por lo tanto no puede ser una const int numFichas = 28; Existe una manera muy sencilla de
calcular el numero total de fichas usando unicamente bucles, y de implementar los arrays usando
tambien estos bucles para explotar la naturaleza variable del numero total de fichas que habrá en el pozo.
Por eso no utilizo esas dos lineas de código
EDIT: No se puede usar una variable como tamaño de un array así que me veo obligado a crear varias constantes...*/

const int numFichas = 55;

typedef short int tArray[numFichas];

typedef string tFichasJugador[7];

void generarPozo(tArray& pozo1, tArray& pozo2, short int limiteMaximo);

void desordenarPozo(tArray& pozo1, tArray& pozo2);

//int calcularNumFichas(short int limiteMaximo);


int main() {
	//Declaración de variables
	//Variables ajustables
	short int limiteMaximo = 6;
	cambiarLimite(limiteMaximo);
	/*Uso ifs y repito funciones por que el numero total de fichas es variable y no
	se puede usar la funcion calcularNumFichas() para darle valor al tamaño ya que no es constante.
	Por esto hay todos estos ifs y for...*/
	tArray pozo1, pozo2;
	generarPozo(pozo1, pozo2, limiteMaximo);
	desordenarPozo(pozo1, pozo2);
	short int numUsadas = 0;

	//Valores estáticos
	bool salir = false;
	srand(time(NULL));
	tFichasJugador fichaJugador;
	for (int i = 0; i < 7; i++) {
		fichaJugador[i] = muestraFicha(pozo1[i], pozo2[i]);
	}
	string tablero = muestraFicha(pozo1[numUsadas], pozo2[numUsadas]);
	numUsadas = 7;
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

string muestraFicha(short int numero1, short int numero2) {
	string fichaMostrada = "|";
	fichaMostrada += '0' + numero1;
	fichaMostrada += '-';
	fichaMostrada += '0' + numero2;
	fichaMostrada = "|";
	return fichaMostrada;
}

void mostrarTablero(tFichasJugador fichasJugador, string tablero, short int numColocadas, short int numRobadas) {
	cout << "--------------------" << endl;
	cout << "|     TABLERO     |" << endl;
	cout << "--------------------" << endl;
	cout << tablero << endl;
	cout << "Fichas colocadas: " << numColocadas << " - ";
	cout << "Fichas robadas: " << numRobadas << endl;
	cout << "Ficha jugador: " << endl;
	for (int i = 0; i < fichasJugador->size(); i++) {
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

bool ejecutarOpcion(short int opcionMenu, tFichasJugador& fichaJugador, string& tablero, short int& numColocadas, short int& numRobadas, short int& limite) {
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

void ponerFichaDer(tFichasJugador& fichasJugador, string& tablero, short int& numColocadas, short int limite) {
	if (puedePonerDer(fichasJugador, tablero)) {
		if (tablero[tablero.size() - 2] == fichasJugador[1]) {
			tablero += fichasJugador;
		}
		else {
			tablero += girarFicha(fichasJugador);
		}
		numColocadas++;
		fichasJugador = muestraFicha(generarFicha(limite));
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

void ponerFichaIzq(string& fichaJugador, string& tablero, short int& numColocadas, short int limite) {
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

void robarFicha(string& fichaJugador, string tablero, short int& numRobadas, short int limite) {
	if (!puedePonerDer(fichaJugador, tablero) && !puedePonerIzq(fichaJugador, tablero)) {
		fichaJugador = muestraFicha(generarFicha(limite));
		numRobadas++;
	}
	else {
		cout << "No puedes robar. Mira mejor, que puedes colocar." << endl;
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

void cargarPartida(string& fichaJugador, string& tablero, short int& numColocadas, short int& numRobadas) {
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

void desordenarPozo(tArray& pozo1, tArray& pozo2) {
	int ind1, ind2;
	short int _tmp1, _tmp2;
	for (int i = 0; i < 1000; i++) {
		ind1 = rand() % numFichas;
		ind2 = rand() % numFichas;
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


/*int calcularNumFichas(short int limiteMaximo) {
	int numFichas = 0;
	for (int i = 0; i <= limiteMaximo; i++) {
		numFichas += i + 1;
	}
	return numFichas;
}
De esta manera se calcula el número de fichas a modo de variable, lo cual es mas sencillo pero
al no poder usar una variable como tamaño de un array, no se puede usar este metodo.*/

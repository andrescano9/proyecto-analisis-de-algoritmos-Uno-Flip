#include <iostream>
#include <vector>
#include <string>
#include "Mazo.h"
#include "Carta.h"

using namespace std;

// Clase Jugador
class Jugador {
public:
    string nombre;
    vector<Carta> mano;

    Jugador(string n) : nombre(n) {}
};

// Función para repartir cartas a los jugadores
void repartirCartas(vector<Jugador>& jugadores, Mazo& mazo, int cartasPorJugador) {
    for (int i = 0; i < jugadores.size(); ++i) {
        for (int j = 0; j < cartasPorJugador; ++j) {
            jugadores[i].mano.push_back(mazo.sacarCarta());
        }
    }
}

// Función para iniciar el juego
void iniciarJuego(int numJugadores) {
    Mazo mazo;
    mazo.barajar();

    vector<Jugador> jugadores;
    for (int i = 0; i < numJugadores; ++i) {
        string nombre;
        cout << "Ingrese el nombre del jugador " << (i + 1) << ": "<<endl;
        cin >> nombre;
        jugadores.push_back(Jugador(nombre));
    }

    // Repartir 7 cartas a cada jugador (puedes ajustar este número si es necesario)
    repartirCartas(jugadores, mazo, 7);

    // Mostrar manos iniciales de los jugadores
    for (const Jugador& jugador : jugadores) {
        cout <<endl<< "Mano de " << jugador.nombre << ":" << endl;
        for (const Carta& carta : jugador.mano) {
            cout << carta.color << " " << carta.valor << endl;
        }
        cout << endl;
    }

    // van demas cosas 
}

int main() {
    int numJugadores = 0;

    cout << "Bienvenido al juego UNO-Flip!" << endl;

    while (true) {
        cout << "\nMenu principal:" << endl;
        cout << "1. Seleccionar cantidad de jugadores (2 a 10)" << endl;
        cout << "2. Iniciar juego" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";

        int opcion;
        cin >> opcion;

        // Verifica la opción ingresada
        if (opcion == 1) {
            // Selección del número de jugadores
            do {
                cout <<endl<< "Ingrese la cantidad de jugadores (2 a 10): ";
                cin >> numJugadores;
                if (numJugadores < 2 || numJugadores > 10) {
                    cout << "Numero de jugadores invalido. Intente nuevamente.\n";
                }
            } while (numJugadores < 2 || numJugadores > 10);

            cout <<endl<< "Has seleccionado " << numJugadores << " jugadores.\n";
        }
        else if (opcion == 2) {
            if (numJugadores < 2 || numJugadores > 10) {
                cout << "Debes seleccionar un numero valido de jugadores antes de iniciar el juego." << endl;
            } else {
                iniciarJuego(numJugadores);
            }
        }
        else if (opcion == 3) {
            // Opción para salir del juego
            cout <<endl<< "Saliendo del juego..." << endl;
            break;
        }
        else {
            // opcion invalida del menu
            cout <<endl<< "Opcion no valida. Intente nuevamente.\n";
        }
    }

    return 0;
}

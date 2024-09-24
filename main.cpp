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
    
    // Método para agregar carta a la mano
    void agregarCarta(Carta carta) {
        mano.push_back(carta);
    }
};

// Función para repartir cartas a los jugadores
void repartirCartas(vector<Jugador>& jugadores, Mazo& mazo, int cartasPorJugador) {
    for (int i = 0; i < jugadores.size(); ++i) {
        for (int j = 0; j < cartasPorJugador; ++j) {
            jugadores[i].agregarCarta(mazo.sacarCarta());
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
        cout << "Ingrese el nombre del jugador " << (i + 1) << ": " << endl;
        cin >> nombre;
        jugadores.push_back(Jugador(nombre));
    }

    // Repartir 7 cartas a cada jugador
    repartirCartas(jugadores, mazo, 7);

    // Mostrar manos iniciales de los jugadores
    for (const Jugador& jugador : jugadores) {
        cout << "\n===== Mano de " << jugador.nombre << " =====" << endl;
        for (const Carta& carta : jugador.mano) {
            cout << carta.getColorActual() << " " << carta.getValorActual() << endl;
        }
        cout << "==============================" << endl << endl;
    }

    int jugadorActual = 0; // Indice del jugador actual

    // Bucle principal del juego
    while (true) {
        jugarTurno(jugadores[jugadorActual], mazo);
        
        // Aquí puedes añadir condiciones para determinar si un jugador ha ganado

        // Pasar al siguiente jugador
        jugadorActual = (jugadorActual + 1) % numJugadores; // Ciclar entre jugadores
    }
}

// Función para jugar el turno de un jugador
void jugarTurno(Jugador& jugador, Mazo& mazo) {
    cout << "\nTurno de " << jugador.nombre << endl;

    // Mostrar mano del jugador
    cout << "Tu mano:" << endl;
    for (int i = 0; i < jugador.mano.size(); ++i) {
        cout << i + 1 << ": " << jugador.mano[i].getColorActual() << " " << jugador.mano[i].getValorActual() << endl;
    }

    // Solicitar al jugador que elija una carta
    int cartaElegida;
    cout << "Elige una carta para jugar (0 para robar): ";
    cin >> cartaElegida;

    if (cartaElegida == 0) {
        // Robar carta del mazo
        jugador.agregarCarta(mazo.sacarCarta());
        cout << "Has robado una carta." << endl;
    } else if (cartaElegida > 0 && cartaElegida <= jugador.mano.size()) {
        // Jugar carta seleccionada
        Carta cartaJugando = jugador.mano[cartaElegida - 1];
        cout << "Has jugado: " << cartaJugando.getColorActual() << " " << cartaJugando.getValorActual() << endl;

        // Aquí puedes añadir lógica para los efectos de las cartas
        jugador.mano.erase(jugador.mano.begin() + (cartaElegida - 1)); // Eliminar carta de la mano
    } else {
        cout << "Opción inválida. Intenta nuevamente." << endl;
    }
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
                cout << endl << "Ingrese la cantidad de jugadores (2 a 10): ";
                cin >> numJugadores;
                if (numJugadores < 2 || numJugadores > 10) {
                    cout << "Numero de jugadores invalido. Intente nuevamente.\n";
                }
            } while (numJugadores < 2 || numJugadores > 10);

            cout << endl << "Has seleccionado " << numJugadores << " jugadores.\n";
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
            cout << endl << "Saliendo del juego..." << endl;
            break;
        }
        else {
            // opción inválida del menú
            cout << endl << "Opcion no valida. Intente nuevamente.\n";
        }
    }

    return 0;
}

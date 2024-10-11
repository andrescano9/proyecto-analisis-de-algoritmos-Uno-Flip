#include <iostream>
#include <vector>
#include <string>
#include "Mazo.h"
#include "Carta.h"
#include "Jugador.h"

using namespace std;

// Declaración anticipada de funciones
void mostrarModoActual(bool isLight);
void jugarTurno(Jugador& jugador, Mazo& mazo, bool& isLight);
void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo);

void repartirCartas(vector<Jugador>& jugadores, Mazo& mazo, int cartasPorJugador) {
    for (int i = 0; i < jugadores.size(); ++i) {
        for (int j = 0; j < cartasPorJugador; ++j) {
            jugadores[i].agregarCarta(mazo.sacarCarta());
        }
    }
}

void iniciarJuego(int numJugadores) {
    Mazo mazo;
    mazo.barajar();

    vector<Jugador> jugadores;
    for (int i = 0; i < numJugadores; ++i) {
        string nombre;
        cout << endl << Ingrese el nombre del jugador << (i + 1) << : << endl;
        cin >> nombre;
        jugadores.push_back(Jugador(nombre));
    }

    // Repartir 7 cartas a cada jugador
    repartirCartas(jugadores, mazo, 7);

    // Mostrar manos de los jugadores
    for (const Jugador& jugador : jugadores) {
        cout << \n===== Mano de << jugador.nombre << ===== << endl;
        jugador.mostrarMano(true); // Muestra el lado light por defecto
        cout << ============================== << endl << endl;
    }

    mazo.cartaActiva = mazo.sacarCarta(); // Carta activa inicial
    cout << La carta activa en la mesa es:
         << mazo.cartaActiva.getColorActual(true) <<
         mazo.cartaActiva.getNumeroActual(true) << endl;

    bool isLight = true; // Modo inicial
    int jugadorActual = 0; // Índice del jugador actual

    while (true) {
        cout << Cartas restantes en el mazo: << mazo.getNumCartas() << endl; // Mostrar cartas restantes
        mostrarModoActual(isLight); // Mostrar el modo actual
        jugarTurno(jugadores[jugadorActual], mazo, isLight);

        if (jugadores[jugadorActual].mano.empty()) {
            cout << \n¡Felicidades << jugadores[jugadorActual].nombre << ! Has ganado el juego! << endl;
            break; // Termina el juego
        }

        // Pasar al siguiente jugador
        jugadorActual = (jugadorActual + 1) % numJugadores; // Ciclar entre jugadores
    }
}

void jugarTurno(Jugador& jugador, Mazo& mazo, bool& isLight) {
    cout << \nTurno de << jugador.nombre << endl;

    // Mostrar mano del jugador correctamente
    jugador.mostrarMano(true); // Siempre muestra en light

    // Solicitar al jugador que elija una carta
    int cartaElegida;
    cout << endl << Elige una carta para jugar (0 para robar): ;
    cin >> cartaElegida;
    cout << endl;

    if (cartaElegida == 0) {
        // Robar carta del mazo
        Carta cartaRobada = mazo.sacarCarta();
        jugador.agregarCarta(cartaRobada);
        cout << Has robado una carta:
             << cartaRobada.getColorActual(true) <<
             cartaRobada.getNumeroActual(true) << endl;

        // Mostrar la carta activa después de robar
        cout << La carta activa en la mesa es:
             << mazo.cartaActiva.getColorActual(true) <<
             mazo.cartaActiva.getNumeroActual(true) << endl;

    } else if (cartaElegida > 0 && cartaElegida <= jugador.mano.size()) {
        // Jugar carta seleccionada
        Carta cartaJugando = jugador.mano[cartaElegida - 1];
        cout << Has jugado:
             << cartaJugando.getColorActual(true) <<
             cartaJugando.getNumeroActual(true) << endl;

        // Verificar si la carta jugada es válida
        if ((cartaJugando.getColorActual(true) == mazo.cartaActiva.getColorActual(true) ||
             cartaJugando.getNumeroActual(true) == mazo.cartaActiva.getNumeroActual(true)) ||
             cartaJugando.esAccion) {

            // Mostrar el efecto de la carta si es una carta de acción
            mostrarEfectoCarta(cartaJugando, jugador, mazo);

            // Actualizar la carta activa
            mazo.setCartaActiva(cartaJugando);
            jugador.mano.erase(jugador.mano.begin() + (cartaElegida - 1)); // Eliminar carta de la mano

            // Cambiar el modo si la carta jugada es una carta con número flip
            if (cartaJugando.getNumeroActual(true) == flip) {
                cout << ¡El modo ha cambiado! << endl;
                isLight = !isLight; // Cambia el modo (light a dark o dark a light)
            }

            // Mostrar la nueva carta activa
            cout << La nueva carta activa es:
                 << mazo.cartaActiva.getColorActual(true) <<
                 mazo.cartaActiva.getNumeroActual(true) << endl;

        } else {
            cout << Carta no válida. Intenta nuevamente. << endl;
            // Mostrar la carta activa si la jugada fue inválida
            cout << La carta activa en la mesa es:
                 << mazo.cartaActiva.getColorActual(true) <<
                 mazo.cartaActiva.getNumeroActual(true) << endl;
        }
    } else {
        cout << Opción inválida. Intenta nuevamente. << endl;
    }
}

void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo) {
    if (carta.getNumeroActual(true) == +2) {
        // El siguiente jugador roba dos cartas
        cout << Efecto: El siguiente jugador roba 2 cartas. << endl;
        // Lógica para el siguiente jugador
    } else if (carta.getNumeroActual(true) == BloquearTodos) {
        cout << Efecto: El siguiente jugador pierde su turno. << endl;
        // Lógica para el siguiente jugador
    } else if (carta.esAccion) {
        // Efectos de otras cartas de acción
        cout << Efecto: << carta.getNumeroActual(true) << endl;
    }
}

void mostrarModoActual(bool isLight) {
    cout << Modo actual: << (isLight ? Light : Dark) << endl;
}

int main() {
    int numJugadores = 0;

    cout << Bienvenido al juego UNO-Flip! << endl;

    while (true) {
        cout << \nMenu principal: << endl;
        cout << 1. Seleccionar cantidad de jugadores (2 a 10) << endl;
        cout << 2. Iniciar juego << endl;
        cout << 3. Salir << endl;
        cout << Seleccione una opcion: ;

        int opcion;
        cin >> opcion;

        if (opcion == 1) {
            // Selección del número de jugadores
            do {
                cout << endl << Ingrese la cantidad de jugadores (2 a 10): ;
                cin >> numJugadores;
                if (numJugadores < 2 || numJugadores > 10) {
                    cout << Número de jugadores inválido. Intente nuevamente.\n;
                }
            } while (numJugadores < 2 || numJugadores > 10);

            cout << endl << Has seleccionado << numJugadores << jugadores.\n;
        } else if (opcion == 2) {
            if (numJugadores < 2 || numJugadores > 10) {
                cout << endl << Debes seleccionar un número válido de jugadores antes de iniciar el juego. << endl;
            } else {
                iniciarJuego(numJugadores);
            }
        } else if (opcion == 3) {
            // Opción para salir del juego
            cout << endl << Gracias por jugar! << endl;
            break;
        } else {
            cout << Opción no válida. Intente nuevamente. << endl;
        }
    }

    return 0;
}

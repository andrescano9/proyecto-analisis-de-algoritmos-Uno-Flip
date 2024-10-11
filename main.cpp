#include <iostream>
#include <vector>
#include <string>
#include "Mazo.h"
#include "Carta.h"
#include "Jugador.h"
#include <limits>

using namespace std;

// Declaracion anticipada de funciones
void mostrarModoActual(bool isLight);
void jugarTurno(Jugador& jugador, Mazo& mazo, bool& isLight);
void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo);
void mostrarInformacionTurno(const Jugador& jugador, const Mazo& mazo, bool isLight);

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
        cout << endl << "Ingrese el nombre del jugador " << (i + 1) << ": " << endl;
        cin >> nombre;
        jugadores.push_back(Jugador(nombre));
    }

    // Repartir 7 cartas a cada jugador
    repartirCartas(jugadores, mazo, 7);

    // Mostrar manos de los jugadores
    for (const Jugador& jugador : jugadores) {
        cout << "\n===== Mano de " << jugador.nombre << " =====" << endl;
        jugador.mostrarMano(true); // Muestra el lado light por defecto
        cout << "==============================" << endl << endl;
    }

    mazo.cartaActiva = mazo.sacarCarta(); // Carta activa inicial
    cout << "La carta activa en la mesa es: "
         << mazo.cartaActiva.getColorActual(true) << " "
         << mazo.cartaActiva.getNumeroActual(true) << endl;

    bool isLight = true; // Modo inicial
    int jugadorActual = 0; // Indice del jugador actual

    while (true) {
        cout << "Cartas restantes en el mazo: " << mazo.getNumCartas() << endl; // Mostrar cartas restantes
        mostrarModoActual(isLight); // Mostrar el modo actual
        jugarTurno(jugadores[jugadorActual], mazo, isLight);

        if (jugadores[jugadorActual].mano.empty()) {
            cout << "\n¡Felicidades " << jugadores[jugadorActual].nombre << "! Has ganado el juego!" << endl;
            break; // Termina el juego
        }

        // Pasar al siguiente jugador
        jugadorActual = (jugadorActual + 1) % numJugadores; // Ciclar entre jugadores
    }
}

void mostrarInformacionTurno(const Jugador& jugador, const Mazo& mazo, bool isLight) {
    cout << "\nTurno de " << jugador.nombre << endl;
    jugador.mostrarMano(true); // Siempre muestra en light
    cout << "La carta activa en la mesa es: "
         << mazo.cartaActiva.getColorActual(true) << " "
         << mazo.cartaActiva.getNumeroActual(true) << endl;
}

void jugarTurno(Jugador& jugador, Mazo& mazo, bool& isLight) {
    // Mostrar la información del turno
    mostrarInformacionTurno(jugador, mazo, isLight);

    // Solicitar al jugador que elija una carta
    int cartaElegida = -1;
    while (true) {
        cout << endl << "Elige una carta para jugar (0 para robar): ";
        cin >> cartaElegida;

        if (cin.fail()) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada incorrecta
            cout << endl << "Entrada no valida. Por favor, ingrese un numero." << endl;
            // Vuelve a mostrar la información del turno
            mostrarInformacionTurno(jugador, mazo, isLight);
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar cualquier carácter extra
            break; // Salir del bucle si la entrada es válida
        }
    }

    cout << endl;

    if (cartaElegida == 0) {
        // Robar carta del mazo
        Carta cartaRobada = mazo.sacarCarta();
        jugador.agregarCarta(cartaRobada);
        cout << "Has robado una carta: "
             << cartaRobada.getColorActual(true) << " "
             << cartaRobada.getNumeroActual(true) << endl;

        // Mostrar la carta activa después de robar
        cout << "La carta activa en la mesa es: "
             << mazo.cartaActiva.getColorActual(true) << " "
             << mazo.cartaActiva.getNumeroActual(true) << endl;

    } else if (cartaElegida > 0 && cartaElegida <= jugador.mano.size()) {
        // Jugar carta seleccionada
        Carta cartaJugando = jugador.mano[cartaElegida - 1];
        cout << "Has jugado: "
             << cartaJugando.getColorActual(true) << " "
             << cartaJugando.getNumeroActual(true) << endl;

        // Verificar si la carta jugada es valida
        if ((cartaJugando.getColorActual(true) == mazo.cartaActiva.getColorActual(true) ||
             cartaJugando.getNumeroActual(true) == mazo.cartaActiva.getNumeroActual(true)) ||
             cartaJugando.esAccion) {

            // Mostrar el efecto de la carta si es una carta de accion
            mostrarEfectoCarta(cartaJugando, jugador, mazo);

            // Actualizar la carta activa
            mazo.setCartaActiva(cartaJugando);
            jugador.mano.erase(jugador.mano.begin() + (cartaElegida - 1)); // Eliminar carta de la mano

            // Cambiar el modo si la carta jugada es una carta con numero "flip"
            if (cartaJugando.getNumeroActual(true) == "flip") {
                cout << "¡El modo ha cambiado!" << endl;
                isLight = !isLight; // Cambia el modo (light a dark o dark a light)
            }

            // Mostrar la nueva carta activa
            cout << "La nueva carta activa es: "
                 << mazo.cartaActiva.getColorActual(true) << " "
                 << mazo.cartaActiva.getNumeroActual(true) << endl;

        } else {
            cout << "Carta no valida. Intenta nuevamente." << endl;
            // Mostrar la carta activa si la jugada fue invalida
            cout << "La carta activa en la mesa es: "
                 << mazo.cartaActiva.getColorActual(true) << " "
                 << mazo.cartaActiva.getNumeroActual(true) << endl;
        }
    } else {
        cout << "Opcion invalida. Intenta nuevamente." << endl;
    }
}

void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo) {
    if (carta.getNumeroActual(true) == "+2") {
        // El siguiente jugador roba dos cartas
        cout << "Efecto: El siguiente jugador roba 2 cartas." << endl;
        // Logica para el siguiente jugador
    } else if (carta.getNumeroActual(true) == "BloquearTodos") {
        cout << "Efecto: El siguiente jugador pierde su turno." << endl;
        // Logica para el siguiente jugador
    } else if (carta.esAccion) {
        // Efectos de otras cartas de accion
        cout << "Efecto: " << carta.getNumeroActual(true) << endl;
    }
}

void mostrarModoActual(bool isLight) {
    cout << "Modo actual: " << (isLight ? "Light" : "Dark") << endl;
}

int main() {
    int numJugadores = 0;

    cout << endl << "Bienvenido al juego UNO-Flip!" << endl;

    while (true) {
        cout << "\nMenu principal:" << endl;
        cout << "1. Seleccionar cantidad de jugadores (2 a 10)" << endl;
        cout << "2. Iniciar juego" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";

        int opcion;
        cin >> opcion;

        if (cin.fail()) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada incorrecta
            cout << endl << "Entrada no valida. Por favor, ingrese un numero." << endl;
            continue; // Volver a mostrar el menú
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar cualquier carácter extra

        if (opcion == 1) {
            do {
                cout <<endl <<"Ingrese la cantidad de jugadores (2-10): ";
                cin >> numJugadores;

                if (cin.fail() || numJugadores < 2 || numJugadores > 10) {
                    cin.clear(); // Limpiar el estado de error
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada incorrecta
                    cout << endl << "Cantidad no valida. Por favor, ingrese un numero entre 2 y 10." << endl;
                } else {
                    cout <<endl<< "Cantidad de jugadores seleccionada: " << numJugadores << endl;
                }
            } while (numJugadores < 2 || numJugadores > 10);

        } else if (opcion == 2) {
            // Iniciar el juego solo si se ha seleccionado una cantidad de jugadores
            if (numJugadores > 0) {
                iniciarJuego(numJugadores);
                numJugadores = 0; // Reiniciar la selección de jugadores después de terminar el juego
            } else {
                cout << endl << "Primero debe seleccionar la cantidad de jugadores." << endl;
            }

        } else if (opcion == 3) {
            cout << "Saliendo del juego..." << endl;
            break;

        } else {
            cout << endl << "Opción no válida. Por favor, intente de nuevo." << endl;
        }
    }

    return 0;
}

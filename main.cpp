#include <iostream>
#include <vector>
#include <string>
#include "Mazo.h"
#include "Carta.h"
#include "Jugador.h"
#include <limits>

using namespace std;

// Declaración anticipada de funciones
void mostrarModoActual(bool isLight);
void jugarTurno(Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, bool& isLight, int& jugadorActual, int& contadorReversas);
void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, int& jugadorActual, bool isLight);
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

    // Asignar la carta activa inicial
    mazo.cartaActiva = mazo.sacarCarta(); // Carta activa inicial

    bool isLight = true; // Modo inicial
    int jugadorActual = 0; // Índice del jugador actual
    int contadorReversas = 0; // Contador de cartas reversas

    while (true) {
        // Mostrar el estado del turno
        mostrarModoActual(isLight); // Mostrar el modo actual
        jugarTurno(jugadores[jugadorActual], mazo, jugadores, isLight, jugadorActual, contadorReversas);

        if (jugadores[jugadorActual].mano.empty()) {
            cout << "\n¡Felicidades " << jugadores[jugadorActual].nombre << "! Has ganado el juego!" << endl;
            break; // Termina el juego
        }

        // Pasar al siguiente jugador
        jugadorActual = (contadorReversas % 2 == 0) ? (jugadorActual + 1) % numJugadores : (jugadorActual - 1 + numJugadores) % numJugadores;
    }
}

void mostrarInformacionTurno(const Jugador& jugador, const Mazo& mazo, bool isLight) {
    cout << "\nTurno de " << jugador.nombre << endl;
    cout << "Cartas restantes en el mazo: " << mazo.getNumCartas() << endl; // Mostrar cartas restantes
    jugador.mostrarMano(isLight); // Muestra la mano en el modo actual (Light o Dark)
    cout << endl << "La carta activa en la mesa es: "
         << mazo.cartaActiva.getColorActual(isLight) << " "
         << mazo.cartaActiva.getNumeroActual(isLight) << endl; // Mostrar carta activa en modo actual
}

void jugarTurno(Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, bool& isLight, int& jugadorActual, int& contadorReversas) {
    while (true) {
        // Mostrar la información del turno
        mostrarInformacionTurno(jugador, mazo, isLight);

        // Si no hay cartas en la mano, terminar el turno
        if (jugador.mano.empty()) {
            cout << "No tienes cartas para jugar. Roba una carta." << endl;
            Carta cartaRobada = mazo.sacarCarta();
            jugador.agregarCarta(cartaRobada);
            cout << "Has robado una carta: "
                 << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
            return; // Termina el turno, pasa al siguiente jugador
        }

        // Solicitar al jugador que elija una carta
        int cartaElegida = -1;
        while (true) {
            cout << endl << "Elige una carta para jugar (0 para robar): ";
            cin >> cartaElegida;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada no valida. Por favor, ingrese un numero." << endl;
            } else if (cartaElegida < 0 || cartaElegida > jugador.mano.size()) {
                cout << "Entrada no valida. Por favor, ingrese un numero valido." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }

        cout << endl;

        if (cartaElegida == 0) {
            // Robar carta del mazo
            Carta cartaRobada = mazo.sacarCarta();
            jugador.agregarCarta(cartaRobada);
            cout << "Has robado una carta: "
                 << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
            return; // Termina el turno, pasa al siguiente jugador

        } else if (cartaElegida > 0 && cartaElegida <= jugador.mano.size()) {
            // Jugar carta seleccionada
            Carta cartaJugando = jugador.mano[cartaElegida - 1];
            cout << "Has jugado: "
                 << cartaJugando.getColorActual(isLight) << " "
                 << cartaJugando.getNumeroActual(isLight) << endl;

            // Verificar si la carta jugada es válida
            bool cartaValida = (cartaJugando.getColorActual(isLight) == mazo.cartaActiva.getColorActual(isLight) ||
                                cartaJugando.getNumeroActual(isLight) == mazo.cartaActiva.getNumeroActual(isLight) ||
                                cartaJugando.esAccion);

            if (cartaValida) {
                mostrarEfectoCarta(cartaJugando, jugador, mazo, jugadores, jugadorActual, isLight);
                mazo.setCartaActiva(cartaJugando);
                jugador.mano.erase(jugador.mano.begin() + (cartaElegida - 1));

                if (cartaJugando.getNumeroActual(isLight) == "flip") {
                    cout << "¡El modo ha cambiado!" << endl;
                    isLight = !isLight;
                }

                // Manejar el turno basado en la carta jugada
                if (cartaJugando.getNumeroActual(isLight) == "reversa") {
                    // Alternar sentido de turno
                    contadorReversas++;
                    cout << "Se ha jugado una carta de reversa."<< endl;
                } else if (cartaJugando.getNumeroActual(isLight) == "bloquearTodos" && !isLight) {
                    // En modo Dark, decrementar jugadorActual para que el mismo jugador vuelva a jugar
                    cout << endl << jugador.nombre << " vuelve a jugar." << endl << endl;
                } else {
                    // En otros casos, actualizar jugadorActual normalmente
                    jugadorActual = (jugadorActual + (contadorReversas % 2 == 0 ? 1 : -1) + jugadores.size()) % jugadores.size();
                }

                cout << "La nueva carta activa es: "
                     << mazo.cartaActiva.getColorActual(isLight) << " "
                     << mazo.cartaActiva.getNumeroActual(isLight) << endl;

                return; // Termina el turno y pasa al siguiente jugador

            } else {
                cout << endl << "Carta no valida. Intenta nuevamente." << endl;
            }
        } else {
            cout << "Opcion invalida. Intente nuevamente." << endl;
        }
    }
}


void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, int& jugadorActual, bool isLight) {
    int siguienteJugador = (jugadorActual + 1) % jugadores.size(); // Obtener el siguiente jugador

    if (carta.getNumeroActual(isLight) == "mas1") {
        // El siguiente jugador roba 1 carta
        cout << "Efecto: El siguiente jugador roba 1 carta." << endl;
        if (mazo.getNumCartas() > 0) {
            Carta cartaRobada = mazo.sacarCarta(); // Robar carta del mazo
            jugadores[siguienteJugador].agregarCarta(cartaRobada); // Agregarla a la mano del siguiente jugador
            cout << jugadores[siguienteJugador].nombre << " ha robado una carta: "
                 << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
        } else {
            cout << "No hay cartas en el mazo para robar." << endl;
        }
    } else if (carta.getNumeroActual(isLight) == "mas5") {
        // El siguiente jugador roba 5 cartas
        cout << "Efecto: El siguiente jugador roba 5 cartas." << endl;
        for (int i = 0; i < 5; ++i) {
            if (mazo.getNumCartas() > 0) {
                Carta cartaRobada = mazo.sacarCarta(); // Robar carta del mazo
                jugadores[siguienteJugador].agregarCarta(cartaRobada); // Agregarla a la mano del siguiente jugador
                cout << jugadores[siguienteJugador].nombre << " ha robado una carta: "
                     << cartaRobada.getColorActual(isLight) << " "
                     << cartaRobada.getNumeroActual(isLight) << endl;
            } else {
                cout << "No hay más cartas en el mazo para robar." << endl;
                break; // Salir del bucle si no hay más cartas
            }
        }
    } else if (carta.getNumeroActual(isLight) == "bloquearTodos") {
        if (isLight) {
            cout << "Efecto: El siguiente jugador pierde su turno." << endl;
            // Siguiente jugador salta su turno
            jugadorActual = siguienteJugador; // Actualizar jugadorActual para saltar el turno
        } else {
            cout << endl << "Efecto: Todos los jugadores excepto " << jugador.nombre << " pierden su turno." << endl;
            // El jugador actual vuelve a jugar
            // No se actualiza jugadorActual
        }
    } else if (carta.esAccion) {
        // Efectos de otras cartas de acción
        cout << "Efecto: " << carta.getNumeroActual(isLight) << endl;
    }
}

void mostrarModoActual(bool isLight) {
    cout << "Modo actual: " << (isLight ? "Light" : "Dark") << endl;
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

        if (cin.fail()) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada incorrecta
            cout << endl << "Entrada no valida. Por favor, ingrese un numero." << endl;
            continue; // Volver a mostrar el menú
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar cualquier carácter extra

        if (opcion == 1) {
            cout << "Ingrese la cantidad de jugadores (2 a 10): ";
            cin >> numJugadores;
            if (numJugadores < 2 || numJugadores > 10) {
                cout << "Numero de jugadores no valido. Debe ser entre 2 y 10." << endl;
                numJugadores = 0; // Resetear a 0 para que el jugador sea informado de la cantidad
            }
        } else if (opcion == 2) {
            if (numJugadores < 2) {
                cout << "Primero debe seleccionar la cantidad de jugadores." << endl;
            } else {
                iniciarJuego(numJugadores);
                numJugadores = 0; // Reiniciar jugadores después de que el juego termina
            }
        } else if (opcion == 3) {
            cout << "Saliendo del juego." << endl;
            break; // Terminar el programa
        } else {
            cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    }

    return 0;
}

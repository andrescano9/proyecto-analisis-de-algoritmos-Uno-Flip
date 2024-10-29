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

bool esColorValido(const string& color, bool isLight) {
    if (isLight) {
        return (color == "rojo" || color == "amarillo" || color == "verde" || color == "azul");
    } else {
        return (color == "naranja" || color == "agua marina" || color == "rosa" || color == "morado");
    }
}

void manejarCambioColor(Mazo& mazo, bool isLight) {
    std::string nuevoColor;
    cout << "Elige un nuevo color (rojo, amarillo, verde, azul para Light; naranja, agua marina, rosa, morado para Dark): ";
    cin >> nuevoColor;

    // Validar el color elegido
    while (!esColorValido(nuevoColor, isLight)) {
        cout << "Color no válido. Por favor, elige nuevamente: ";
        cin >> nuevoColor;
    }

    cout << "El nuevo color es: " << nuevoColor << endl;

    // Establecer el nuevo color en el modo correspondiente
    if (isLight) {
        mazo.cartaActiva.setColorLight(nuevoColor);
    } else {
        mazo.cartaActiva.setColorDark(nuevoColor);
    }
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
                                cartaJugando.esAccion ||
                                cartaJugando.getNumeroActual(isLight) == "CambioColor"); // Permite jugar CambioColor siempre

            // Si la carta activa es "CambioColor", permitir jugar cualquier carta del nuevo color elegido
            if (mazo.cartaActiva.getNumeroActual(isLight) == "CambioColor") {
                cartaValida = (cartaJugando.getColorActual(isLight) == mazo.cartaActiva.getColorActual(isLight));
            }

            // Validar si la carta jugada es una reversa
            if (cartaJugando.getNumeroActual(isLight) == "reversa") {
                // Asegurarse que el color coincida con la carta activa
                if (cartaJugando.getColorActual(isLight) != mazo.cartaActiva.getColorActual(isLight)) {
                    cartaValida = false;
                    cout << "No puedes jugar la carta reversa. Debe coincidir con el color de la carta activa." << endl;
                }
            }

            // Validar si la carta jugada es un bloqueo
            if (cartaJugando.getNumeroActual(isLight) == "bloquear") {
                // Asegurarse que el color coincida con la carta activa
                if (cartaJugando.getColorActual(isLight) != mazo.cartaActiva.getColorActual(isLight)) {
                    cartaValida = false; // Si no coincide, la carta no es válida
                    cout << "No puedes jugar la carta de bloqueo. Debe coincidir con el color de la carta activa." << endl;
                } else {
                    cout << "Se ha jugado una carta de bloqueo." << endl; // Mensaje para carta de bloqueo válida
                }
            }

if (cartaValida) {
    // Manejar el efecto de la carta jugada
    mostrarEfectoCarta(cartaJugando, jugador, mazo, jugadores, jugadorActual, isLight);

    // Establecer la nueva carta activa
    mazo.setCartaActiva(cartaJugando);
    jugador.mano.erase(jugador.mano.begin() + (cartaElegida - 1));

    // Cambiar modo si se jugó "flip"
    if (cartaJugando.getNumeroActual(isLight) == "flip") {
        cout << "¡El modo ha cambiado!" << endl;
        isLight = !isLight;
        return;
    }

    // Si se jugó un "CambioColor", manejar el nuevo color
    if (cartaJugando.getNumeroActual(isLight) == "CambioColor") {
        manejarCambioColor(mazo, isLight);
        // No cambiar de jugador, ya que el turno sigue con el mismo jugador tras elegir el color
        return;
    }

    // Manejar el turno basado en la carta jugada
    if (cartaJugando.getNumeroActual(isLight) == "reversa") {
        contadorReversas++;
        cout << "Se ha jugado una carta de reversa." << endl;
    } else if (cartaJugando.getNumeroActual(isLight) == "bloquear") {
        cout << "Se ha jugado una carta de bloqueo. El siguiente jugador pierde su turno." << endl;
        // No cambiar al siguiente jugador porque el turno se salta
        return;
    } else {
        // Cambiar al siguiente jugador solo si no se jugó un CambioColor o Bloquear
        jugadorActual = (jugadorActual + (contadorReversas % 2 == 0 ? 1 : -1) + jugadores.size()) % jugadores.size();
    }

    return; // Termina el turno, pasa al siguiente jugador
} else {
    cout << "No puedes jugar esa carta. Debe coincidir el color, número o ser una carta de acción." << endl;
}

        } else {
            cout << "Entrada no válida." << endl;
        }
    }
}






void mostrarModoActual(bool isLight) {
    cout << "Modo actual: " << (isLight ? "Light" : "Dark") << endl;
}

void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, int& jugadorActual, bool isLight) {
    if (carta.getNumeroActual(isLight) == "mas1") {
        // Lógica para carta +1...
        jugadorActual = (jugadorActual + 1) % jugadores.size();
        Carta cartaRobada = mazo.sacarCarta();
        cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado una carta adicional: "
             << cartaRobada.getColorActual(isLight) << " "
             << cartaRobada.getNumeroActual(isLight) << "!" << endl;
        jugadores[jugadorActual].agregarCarta(cartaRobada);
        jugadorActual = (jugadorActual + 1) % jugadores.size();
    } else if (carta.getNumeroActual(isLight) == "mas5") {
        // Cambiar el turno al siguiente jugador para que robe cinco cartas
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Actualiza al siguiente jugador

        // El siguiente jugador roba cinco cartas
        cout <<endl<< "El jugador " << jugadores[jugadorActual].nombre << " ha robado cinco cartas adicionales!" << endl;
        for (int i = 0; i < 5; ++i) {
            Carta cartaRobada = mazo.sacarCarta();
            jugadores[jugadorActual].agregarCarta(cartaRobada);
            cout << "Carta robada: " << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
        }

        // Actualizar para que el turno pase al siguiente después del jugador que robó
        jugadorActual = (jugadorActual + 1) % jugadores.size();
    }


     else if (carta.getNumeroActual(isLight) == "bloquear") {
        cout << "El siguiente jugador se ha bloqueado." << endl;
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Salta al siguiente jugador
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
                cout <<endl<< "Numero de jugadores no valido. Debe ser entre 2 y 10." << endl;
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

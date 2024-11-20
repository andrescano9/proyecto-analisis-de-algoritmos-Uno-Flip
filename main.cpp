
#include <iostream>
#include <vector>
#include <string>
#include "Mazo.h"
#include "Carta.h"
#include "Jugador.h"
#include <algorithm>
#include <random>
#include <limits>

using namespace std;

// Declaración anticipada de funciones
void mostrarModoActual(bool isLight);
void jugarTurno(Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, bool& isLight, int& jugadorActual, int& contadorReversas);
void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, int& jugadorActual, bool isLight);
void mostrarInformacionTurno(const Jugador& jugador, const Mazo& mazo, bool isLight);

void repartirCartas(vector<Jugador>& jugadores, Mazo& mazo, int cartasPorJugador)
{
    // Primero, barajamos el mazo
    mazo.barajar();  // Esto ya lo hace la función barajar dentro de Mazo

    // Repartir las cartas aleatoriamente entre los jugadores
    for (int i = 0; i < jugadores.size(); ++i)
    {
        for (int j = 0; j < cartasPorJugador; ++j)
        {
            // Sacamos la carta del mazo y la agregamos a la mano del jugador
            jugadores[i].agregarCarta(mazo.sacarCarta());
        }
    }
}




void iniciarJuego(int numJugadores, int numBots)
{
    Mazo mazo;
    mazo.barajar();

    vector<Jugador> jugadores;

    // Agregar jugadores humanos
    for (int i = 0; i < numJugadores; ++i)
    {
        string nombre;
        cout << endl << "Ingrese el nombre del jugador " << (i + 1) << ": " << endl;
        cin >> nombre;
        jugadores.push_back(Jugador(nombre));
    }

    // Agregar bots
    for (int i = 0; i < numBots; ++i)
    {
        string nombre = "Bot" + to_string(i + 1);  // Nombre automático para los bots
        jugadores.push_back(Jugador(nombre, true)); // Bot con el flag 'true'
    }

    // Repartir 7 cartas a cada jugador
    repartirCartas(jugadores, mazo, 7);

    // Asignar la carta activa inicial
    mazo.cartaActiva = mazo.sacarCarta(); // Carta activa inicial

    bool isLight = true; // Modo inicial
    int jugadorActual = 0; // Índice del jugador actual (considerando todos)
    int contadorReversas = 0; // Contador de cartas reversas

    while (true)
    {
        // Mostrar el estado del turno
        mostrarModoActual(isLight); // Mostrar el modo actual
        jugarTurno(jugadores[jugadorActual], mazo, jugadores, isLight, jugadorActual, contadorReversas);

        if (jugadores[jugadorActual].mano.empty())
        {
            cout << "\n¡Felicidades " << jugadores[jugadorActual].nombre << "! Has ganado el juego!" << endl;
            break; // Termina el juego
        }

        // Pasar al siguiente jugador considerando todos los jugadores y bots
        jugadorActual = (contadorReversas % 2 == 0) ? (jugadorActual + 1) % jugadores.size() : (jugadorActual - 1 + jugadores.size()) % jugadores.size();
    }
}



void mostrarInformacionTurno(const Jugador& jugador, const Mazo& mazo, bool isLight)
{
    cout << "\nTurno de " << jugador.nombre;
    if (jugador.esBot()) {
        cout << " (Bot)";
    }
    cout << endl;
    cout << "Cartas restantes en el mazo: " << mazo.getNumCartas() << endl;
    jugador.mostrarMano(isLight); // Mostrar la mano, ya sea del jugador humano o bot
    cout << endl << "La carta activa en la mesa es: "
         << mazo.cartaActiva.getColorActual(isLight) << " "
         << mazo.cartaActiva.getNumeroActual(isLight) << endl;
}


bool esColorValido(const string& color, bool isLight)
{
    if (isLight)
    {
        return (color == "rojo" || color == "amarillo" || color == "verde" || color == "azul");
    }
    else
    {
        return (color == "naranja" || color == "agua" || color == "rosa" || color == "morado");
    }
}

void manejarCambioColor(Mazo& mazo, bool isLight)
{
    std::string nuevoColor;
    cout << "Elige un nuevo color (LIGHT: (rojo, amarillo, verde, azul) ;DARK: (rosa, agua, morado, naranja )): ";
    cin >> nuevoColor;

    // Validar el color elegido
    while (!esColorValido(nuevoColor, isLight))
    {
        cout << "Color no válido. Por favor, elige nuevamente: ";
        cin >> nuevoColor;
    }

    cout << "El nuevo color es: " <<nuevoColor<< endl;

    // Establecer el nuevo color en el modo correspondiente
    if (isLight)
    {
        mazo.cartaActiva.setColorLight(nuevoColor);
    }
    else
    {
        mazo.cartaActiva.setColorDark(nuevoColor);
    }
}



void jugarTurno(Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, bool& isLight, int& jugadorActual, int& contadorReversas)
{
    while (true)
    {
        // Mostrar la información del turno
        mostrarInformacionTurno(jugador, mazo, isLight);

        // Si no hay cartas en la mano, terminar el turno
        if (jugador.mano.empty())
        {
            cout << "¡Felicidades! El jugador " << jugador.nombre << " ha ganado al quedarse sin cartas." << endl;
            exit(0);
        }

        // Si el jugador es un bot, manejar su lógica
        if (jugador.esBot())
        {
            bool jugoCarta = false;
            // Buscar una carta del mismo color que la carta activa
            for (int i = 0; i < jugador.mano.size(); ++i)
            {
                Carta cartaBot = jugador.mano[i];

                // Si el color de la carta coincide con el color de la carta activa, juega la carta
                if (cartaBot.getColorActual(isLight) == mazo.cartaActiva.getColorActual(isLight) ||
                    cartaBot.getNumeroActual(isLight) == mazo.cartaActiva.getNumeroActual(isLight) ||
                    cartaBot.esAccion)  // También puede jugar cartas de acción
                {
                    cout << "El bot " << jugador.nombre << " ha jugado una carta: "
                         << cartaBot.getColorActual(isLight) << " "
                         << cartaBot.getNumeroActual(isLight) << endl;
                    mazo.setCartaActiva(cartaBot);  // Establecer la nueva carta activa
                    jugador.mano.erase(jugador.mano.begin() + i);  // Eliminar la carta jugada
                    mostrarEfectoCarta(cartaBot, jugador, mazo, jugadores, jugadorActual, isLight);  // Mostrar el efecto de la carta
                    jugoCarta = true;
                    break;
                }
            }

            // Si no jugó ninguna carta, el bot roba una carta
            if (!jugoCarta)
            {
                cout << "El bot " << jugador.nombre << " no tiene cartas del mismo color. Ha robado una carta." << endl;
                Carta cartaRobada = mazo.sacarCarta();
                jugador.agregarCarta(cartaRobada);
                cout << "La carta robada es: "
                     << cartaRobada.getColorActual(isLight) << " "
                     << cartaRobada.getNumeroActual(isLight) << endl;
            }

            return; // Termina el turno, pasa al siguiente jugador
        }

        // Turno de un jugador humano
        int cartaElegida = -1;
        while (true)
        {
            cout << endl << "Elige una carta para jugar (0 para robar): ";
            cin >> cartaElegida;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada no válida. Por favor, ingrese un número." << endl;
            }
            else if (cartaElegida < 0 || cartaElegida > jugador.mano.size())
            {
                cout << "Entrada no válida. Por favor, ingrese un número válido." << endl;
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }

        cout << endl;

        if (cartaElegida == 0)
        {
            // Robar carta del mazo
            Carta cartaRobada = mazo.sacarCarta();
            jugador.agregarCarta(cartaRobada);
            cout << "La carta robada es: "
                 << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
            return; // Termina el turno después de robar
        }
        else if (cartaElegida > 0 && cartaElegida <= jugador.mano.size())
        {
            // Jugar carta seleccionada
            Carta cartaJugando = jugador.mano[cartaElegida - 1];
            cout << "Has jugado: "
                 << cartaJugando.getColorActual(isLight) << " "
                 << cartaJugando.getNumeroActual(isLight) << endl;

            // Verificar si la carta jugada es válida
            bool cartaValida = false;

            // Validar si la carta es un comodín (CambioColor o CambioColorMás2)
            if (cartaJugando.getNumeroActual(isLight) == "CambioColor" ||
                cartaJugando.getNumeroActual(isLight) == "CambioColorMas2")
            {
                cartaValida = true; // Comodines siempre son válidos
            }
            else
            {
                // Validar por color o número
                if (cartaJugando.getColorActual(isLight) == mazo.cartaActiva.getColorActual(isLight) ||
                    cartaJugando.getNumeroActual(isLight) == mazo.cartaActiva.getNumeroActual(isLight))
                {
                    cartaValida = true;
                }
            }

            // Si es una carta "reversa", aumentar el contador de reversas
            if (cartaJugando.getNumeroActual(isLight) == "reversa")
            {
                contadorReversas++;
            }

            // Si la carta es válida, jugarla
            if (cartaValida)
            {
                // Manejar el efecto de la carta jugada
                mostrarEfectoCarta(cartaJugando, jugador, mazo, jugadores, jugadorActual, isLight);

                // Establecer la nueva carta activa
                mazo.setCartaActiva(cartaJugando);
                jugador.mano.erase(jugador.mano.begin() + (cartaElegida - 1));

                // Cambiar el modo si se jugó "flip"
                if (cartaJugando.getNumeroActual(isLight) == "flip")
                {
                    cout << "¡El modo ha cambiado!" << endl;
                    isLight = !isLight;
                    return;
                }

                // Si se jugó un "CambioColor", cambiar el color
                if (cartaJugando.getNumeroActual(isLight) == "CambioColor")
                {
                    manejarCambioColor(mazo, isLight);
                }

                break; // Termina el turno si la carta fue válida
            }
            else
            {
                cout << "Carta no válida, por favor elige otra." << endl;
                // El jugador sigue eligiendo cartas, sin pasar al siguiente jugador
            }
        }
    }
}












void mostrarModoActual(bool isLight)
{
    cout << endl << "Modo: " << (isLight ? "Light" : "Dark") << endl;
}





void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, int& jugadorActual, bool isLight)
{
    if (carta.getNumeroActual(isLight) == "bloqueo")
    {
        // Se juega una carta de bloquear, el siguiente jugador se salta su turno
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Salta al siguiente jugador
        cout << "¡Se ha jugado una carta de Bloquear! El siguiente jugador se salta su turno." << endl;
    }


    if (carta.getNumeroActual(isLight) == "bloquearTodos")
    {
        // Se juega una carta de bloquear, el siguiente jugador se salta su turno
        jugadorActual = (jugadorActual - 1) % jugadores.size(); // Salta al siguiente jugador
        cout << "Todos fueron bloqueados" << endl;
    }




    else if (carta.getNumeroActual(isLight) == "mas1")
    {
        jugadorActual = (jugadorActual + 1) % jugadores.size();
        Carta cartaRobada = mazo.sacarCarta();
        cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado una carta adicional: "
             << cartaRobada.getColorActual(isLight) << " "
             << cartaRobada.getNumeroActual(isLight) << "!" << endl;
        jugadores[jugadorActual].agregarCarta(cartaRobada);
    }
    else if (carta.getNumeroActual(isLight) == "mas5")
    {
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Cambiar al siguiente jugador
        cout << endl << "El jugador " << jugadores[jugadorActual].nombre << " ha robado cinco cartas adicionales!" << endl;
        for (int i = 0; i < 5; ++i)
        {
            Carta cartaRobada = mazo.sacarCarta();
            jugadores[jugadorActual].agregarCarta(cartaRobada);
            cout << "Carta robada: " << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
        }
    }
    else if (carta.getNumeroActual(isLight) == "CambioColorWild")
    {
        cout << "¡Se ha jugado una carta de CambioColorWild!" << endl;
        cout << "El jugador " << jugadores[jugadorActual].nombre << " debe elegir un nuevo color." << endl;

        // Solicitar el nuevo color
        string nuevoColor;
        cout << "Elige un nuevo color (naranja,agua, rosa, morado): ";
        cin >> nuevoColor;

        // Validar el color elegido
        while (!esColorValido(nuevoColor, isLight))
        {
            cout << "Color no válido. Por favor, elige nuevamente: ";
            cin >> nuevoColor;
        }

        cout << "El nuevo color es: " << nuevoColor << endl;

        // Establecer el nuevo color en el modo correspondiente
        if (isLight)
        {
            mazo.cartaActiva.setColorLight(nuevoColor);
        }
        else
        {
            mazo.cartaActiva.setColorDark(nuevoColor);
        }

        // Pasar al siguiente jugador
        jugadorActual = (jugadorActual + 1) % jugadores.size();
    }

    else if (carta.getNumeroActual(isLight) == "CambioColorMas2")
    {
        cout << "¡Se ha jugado una carta de CambioColorMas2!" << endl;

        // Paso 1: Llamamos a la función manejarCambioColor para cambiar el color
        manejarCambioColor(mazo, isLight);

        // Paso 2: El siguiente jugador (B) roba dos cartas
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Siguiente jugador
        for (int i = 0; i < 2; ++i)
        {
            Carta cartaRobada = mazo.sacarCarta();
            jugadores[jugadorActual].agregarCarta(cartaRobada);
            cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado una carta: "
                 << cartaRobada.getColorActual(isLight) << " "
                 << cartaRobada.getNumeroActual(isLight) << endl;
        }
        cout << endl;

        // Ahora, el siguiente jugador puede jugar cualquier carta que coincida con el nuevo color elegido
        // Continuamos con el turno del siguiente jugador



    }



}


int main()
{
    int numJugadores = 0;
    int numBots = 0;

    cout << "Bienvenido al juego UNO-Flip!" << endl;

    while (true)
    {
        cout << "\nMenu principal:" << endl;
        cout << "1. Seleccionar cantidad de jugadores (2 a 10)" << endl;
        cout << "2. Iniciar juego" << endl;
        cout << "3. Salir" << endl;

        int opcion = 0;
        bool opcionValida = false;

        while (!opcionValida)
        {
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            // Validación de la entrada
            if (cin.fail())
            {
                cin.clear(); // Limpiar el estado de error
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada incorrecta
                cout << "Entrada no valida. Por favor, ingrese un numero entre 1 y 3." << endl;
            }
            else if (opcion < 1 || opcion > 3)
            {
                cout << "Opción no válida. Por favor, ingrese un número entre 1 y 3." << endl;
            }
            else
            {
                opcionValida = true; // Entrada válida
            }
        }

        // Procesar la opción válida
        if (opcion == 1)
        {
            cout << "Ingrese la cantidad de jugadores (2 a 10): ";
            while (true)
            {
                cin >> numJugadores;

                if (cin.fail() || numJugadores < 2 || numJugadores > 10)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Número de jugadores no válido. Debe ser entre 2 y 10. Intente nuevamente: ";
                }
                else
                {
                    break;
                }
            }

            cout << "Ingrese la cantidad de bots (0 a " << 10 - numJugadores << "): ";
            while (true)
            {
                cin >> numBots;

                if (cin.fail() || numBots < 0 || numBots > 10 - numJugadores)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Número de bots no válido. Debe ser entre 0 y " << 10 - numJugadores << ". Intente nuevamente: ";
                }
                else
                {
                    break;
                }
            }
        }
        else if (opcion == 2)
        {
            if (numJugadores < 2)
            {
                cout << "Primero debe seleccionar la cantidad de jugadores." << endl;
            }
            else
            {
                // Llamar a la función iniciarJuego pasando numJugadores y numBots
                iniciarJuego(numJugadores, numBots);
                numJugadores = 0; // Reiniciar jugadores después de que el juego termina
                numBots = 0; // Reiniciar bots
            }
        }
        else if (opcion == 3)
        {
            cout << "Saliendo del juego." << endl;
            break; // Terminar el programa
        }
    }

    return 0;
}

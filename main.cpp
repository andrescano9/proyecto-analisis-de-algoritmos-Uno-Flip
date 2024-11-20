
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
void mostrarModoActual(bool isLight);  // Muestra el modo actual (luz o oscuro)
void jugarTurno(Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, bool& isLight, int& jugadorActual, int& contadorReversas);  // Lógica del turno de un jugador (incluyendo bots)
void mostrarEfectoCarta(const Carta& carta, Jugador& jugador, Mazo& mazo, vector<Jugador>& jugadores, int& jugadorActual, bool isLight);  // Muestra los efectos de una carta jugada
void mostrarInformacionTurno(const Jugador& jugador, const Mazo& mazo, bool isLight);  // Muestra la información del turno (jugador, mazo y carta activa)

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
    if (jugador.esBot())
    {
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
            exit(0);  // Termina el juego
        }

if (jugador.esBot())
{
    bool jugoCarta = false;

    // Buscar una carta del mismo color o número que la carta activa, o si es una carta de acción
    for (int i = 0; i < jugador.mano.size(); ++i)
    {
        Carta cartaBot = jugador.mano[i];

        // Validar si la carta puede ser jugada
        bool cartaValida = false;

        // Si la carta es una carta de acción (bloqueo, salto, reversa, flip, etc.)
        if (cartaBot.esAccion)
        {
            // La carta de acción se puede jugar si el color coincide con la carta activa o si la carta activa también es de acción
            if (cartaBot.getColorActual(isLight) == mazo.cartaActiva.getColorActual(isLight) ||
                mazo.cartaActiva.esAccion)  // Se puede jugar cualquier carta de acción si la carta activa es también una acción
            {
                cartaValida = true;
            }
        }
        // Si la carta es una carta numérica
        else if (cartaBot.getColorActual(isLight) == mazo.cartaActiva.getColorActual(isLight) ||
                 cartaBot.getNumeroActual(isLight) == mazo.cartaActiva.getNumeroActual(isLight))
        {
            cartaValida = true;
        }

        // Si la carta es válida, jugarla
        if (cartaValida)
        {
            cout << "El " << jugador.nombre << " ha jugado una carta: "
                 << cartaBot.getColorActual(isLight) << " "
                 << cartaBot.getNumeroActual(isLight) << endl;



        mostrarEfectoCarta(cartaBot, jugador, mazo, jugadores, jugadorActual, isLight);  // Mostrar el efecto de la carta jugada

            // Si la carta es un "CambioColor", "CambioColorMas2", "CambioColorWild"
            if (cartaBot.getNumeroActual(isLight) == "CambioColor" ||
                cartaBot.getNumeroActual(isLight) == "CambioColorMas2" ||
                cartaBot.getNumeroActual(isLight) == "CambioColorWild" )
            {
                // Si es una carta "reversa", aumentar el contador de reversas
                if (cartaBot.getNumeroActual(isLight) == "reversa")
                {
                    contadorReversas++;
                }

// Si se jugó un "CambioColor", cambiar el color
if (cartaBot.getNumeroActual(isLight) == "CambioColor")
{
    string colorSeleccionado;

    // El bot selecciona un color dependiendo del modo
    if (isLight)
    {
        colorSeleccionado = "azul";  // Para modo light
        jugadorActual = (jugadorActual + 1) % jugadores.size();


    }
    else
    {
        colorSeleccionado = "agua";  // Para modo dark
        jugadorActual = (jugadorActual + 1) % jugadores.size();

    }

    // Mostrar el color seleccionado por el bot
    cout << "El " << jugador.nombre << " ha elegido el color: " << colorSeleccionado << endl;

    // Establecer el nuevo color en el modo correspondiente
    if (isLight)
    {
        mazo.cartaActiva.setColorLight(colorSeleccionado);
    }
    else
    {
        mazo.cartaActiva.setColorDark(colorSeleccionado);
    }
}


// Si se jugó "CambioColorWild", manejar el cambio de color con entrada automática para el bot
if (cartaBot.getNumeroActual(isLight) == "CambioColorWild")
{
    // Seleccionar un color automáticamente dependiendo del modo (light o dark)
    string colorSeleccionado;
    if (isLight)
    {
        // Elegir un color para modo Light
        colorSeleccionado = "rojo";
    }
    else
    {
        // Elegir un color para modo Dark
        colorSeleccionado = "rosa";
    }

    // Establecer el nuevo color en el modo correspondiente
    cout << "El bot " << jugador.nombre << " ha elegido el color: " << colorSeleccionado << endl;

    if (isLight)
    {
        mazo.cartaActiva.setColorLight(colorSeleccionado);
    }
    else
    {
        mazo.cartaActiva.setColorDark(colorSeleccionado);
    }

    // El siguiente jugador roba hasta que obtenga una carta del color seleccionado
    bool cartaValida = false;
    while (!cartaValida)
    {
        // El siguiente jugador roba una carta
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Cambiar al siguiente jugador
        Carta cartaRobada = mazo.sacarCarta();
        jugadores[jugadorActual].agregarCarta(cartaRobada);
        cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado una carta: "
             << cartaRobada.getColorActual(isLight) << " "
             << cartaRobada.getNumeroActual(isLight) << endl;

        // Verificar si la carta robada es del color elegido
        if (cartaRobada.getColorActual(isLight) == colorSeleccionado)
        {
            cartaValida = true;
        }
    }
}


                // Si se jugó "CambioColorMas2", manejar el cambio de color y el robo de cartas
                if (cartaBot.getNumeroActual(isLight) == "CambioColorMas2")
                {
                    // Seleccionar un color automáticamente dependiendo del modo (light o dark)
                    string colorSeleccionado;
                    if (isLight)
                    {
                        colorSeleccionado = "rojo";
                    }
                    else
                    {
                        colorSeleccionado = "rosa";
                    }

                    cout << "El " << jugador.nombre << " ha elegido el color: " << colorSeleccionado << endl;

                    if (isLight)
                    {
                        mazo.cartaActiva.setColorLight(colorSeleccionado);
                    }
                    else
                    {
                        mazo.cartaActiva.setColorDark(colorSeleccionado);
                    }

                    // El siguiente jugador roba dos cartas
                    jugadorActual = (jugadorActual + 1) % jugadores.size();
                    cout << endl;
                    for (int i = 0; i < 2; ++i)
                    {
                        Carta cartaRobada = mazo.sacarCarta();
                        jugadores[jugadorActual].agregarCarta(cartaRobada);
                        cout << "El " << jugadores[jugadorActual].nombre << " ha robado una carta: "
                             << cartaRobada.getColorActual(isLight) << " "
                             << cartaRobada.getNumeroActual(isLight) << endl;
                    }
                }

                break; // Termina el turno si la carta fue válida
            }

            // Establecer la nueva carta activa
            mazo.setCartaActiva(cartaBot);
            jugador.mano.erase(jugador.mano.begin() + i);  // Eliminar la carta jugada

            // Aquí es donde debes asegurarte de que se ejecute el efecto de la carta jugada
            mostrarEfectoCarta(cartaBot, jugador, mazo, jugadores, jugadorActual, isLight);  // Mostrar el efecto de la carta jugada

            jugoCarta = true;
            break;  // Termina el turno si el bot ha jugado una carta
        }
    }

    // Si no jugó ninguna carta, el bot roba una carta
    if (!jugoCarta)
    {
        cout << "El " << jugador.nombre << " no tiene cartas del mismo color. Ha robado una carta." << endl;
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

            // Validar si la carta es un comodín (CambioColor o CambioColorMas2)
            if (cartaJugando.getNumeroActual(isLight) == "CambioColor" ||
                    cartaJugando.getNumeroActual(isLight) == "CambioColorMas2"||
                    cartaJugando.getNumeroActual(isLight) == "CambioColorWild")
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
                if (cartaJugando.getNumeroActual(isLight) == "CambioColorWild")
                {
                    cout << "El jugador " << jugadores[jugadorActual].nombre << " debe elegir un nuevo color." << endl<<endl;

                    // Solicitar el nuevo color
                    string nuevoColor;
                    cout << "Elige un nuevo color (naranja, agua, rosa, morado): ";
                    cin >> nuevoColor;

                    // Validar el color elegido
                    while (!esColorValido(nuevoColor, isLight))
                    {
                        cout <<endl<< "Color no válido. Por favor, elige nuevamente: ";
                        cin >> nuevoColor;
                    }

                    cout <<endl <<"El nuevo color es: " << nuevoColor << endl;

                    // Establecer el nuevo color en el modo correspondiente
                    if (isLight)
                    {
                        mazo.cartaActiva.setColorLight(nuevoColor);
                    }
                    else
                    {
                        mazo.cartaActiva.setColorDark(nuevoColor);
                    }

                    // El jugador actual sigue robando cartas hasta que tenga una del color elegido
                    bool cartaValida = false;
                    cout<<endl;
                    while (!cartaValida)
                    {
                        // El jugador roba una carta
                        Carta cartaRobada = mazo.sacarCarta();
                        jugadores[jugadorActual].agregarCarta(cartaRobada);
                        cout << "La carta robada es: "
                             << cartaRobada.getColorActual(isLight) << " "
                             << cartaRobada.getNumeroActual(isLight) << endl;

                        // Verificar si la carta robada es del color elegido
                        if (cartaRobada.getColorActual(isLight) == nuevoColor)
                        {
                            cartaValida = true;
                        }
                    }

                    // Pasar al siguiente jugador
                    jugadorActual = (jugadorActual + 1) % jugadores.size();
                }

                // Si se jugó "CambioColorMas2", manejar el cambio de color y el robo de cartas
                if (cartaJugando.getNumeroActual(isLight) == "CambioColorMas2")
                {
                    cout <<endl<< "El jugador " << jugadores[jugadorActual].nombre << " debe elegir un nuevo color." << endl;

                    // Solicitar el nuevo color
                    string nuevoColor;
                    cout << "Elige un nuevo color (rojo, amarillo, azul, verde): ";
                    cin >> nuevoColor;

                    // Validar el color elegido
                    while (!esColorValido(nuevoColor, isLight))
                    {
                        cout << "Color no válido. Por favor, elige nuevamente: ";
                        cin >> nuevoColor;
                    }

                    cout << "El nuevo color es: " << nuevoColor << endl<<endl;

                    // Establecer el nuevo color en el modo correspondiente
                    if (isLight)
                    {
                        mazo.cartaActiva.setColorLight(nuevoColor);
                    }
                    else
                    {
                        mazo.cartaActiva.setColorDark(nuevoColor);
                    }

                    // El siguiente jugador roba dos cartas
                    jugadorActual = (jugadorActual + 1) % jugadores.size();
                    cout<<endl;
                    for (int i = 0; i < 2; ++i)
                    {
                        Carta cartaRobada = mazo.sacarCarta();
                        jugadores[jugadorActual].agregarCarta(cartaRobada);
                        cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado una carta: "
                             << cartaRobada.getColorActual(isLight) << " "
                             << cartaRobada.getNumeroActual(isLight) << endl;
                    }
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
        jugadorActual = (jugadorActual) % jugadores.size(); // Salta al siguiente jugador
        cout << "¡Se ha jugado una carta de Bloquear! El siguiente jugador se salta su turno." << endl;
    }
    else if (carta.getNumeroActual(isLight) == "bloquearTodos")
    {
        // Se juega una carta de bloquear todos, todos los jugadores se saltan su turno
        jugadorActual = (jugadorActual - 1) % jugadores.size(); // El siguiente jugador se salta su turno
        cout << "¡Se ha jugado una carta de Bloquear Todos! Todos los jugadores se saltan su turno." << endl;
    }
    else if (carta.getNumeroActual(isLight) == "mas1")
    {
        // Se juega una carta de +1, el siguiente jugador roba una carta
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Cambiar al siguiente jugador
        Carta cartaRobada = mazo.sacarCarta();
        cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado una carta adicional: "
             << cartaRobada.getColorActual(isLight) << " "
             << cartaRobada.getNumeroActual(isLight) << "!" << endl;
        jugadores[jugadorActual].agregarCarta(cartaRobada);
    }
    else if (carta.getNumeroActual(isLight) == "mas5")
    {
        // Se juega una carta de +5, el siguiente jugador roba cinco cartas
        jugadorActual = (jugadorActual + 1) % jugadores.size(); // Cambiar al siguiente jugador
        cout << "El jugador " << jugadores[jugadorActual].nombre << " ha robado cinco cartas adicionales!" << endl;
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
        // Se juega una carta de Cambio de Color +2, el color será manejado por la función jugarTurno
        cout << "¡Se ha jugado una carta de CambioColorMas2!" << endl;
    }

    else if (carta.getNumeroActual(isLight) == "CambioColorMas2")
    {
        // Se juega una carta de Cambio de Color +2, el color será manejado por la función jugarTurno
        cout << "¡Se ha jugado una carta de CambioColorMas2!" << endl;
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

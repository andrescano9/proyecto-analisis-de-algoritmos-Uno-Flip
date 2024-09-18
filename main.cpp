#include <iostream>
using namespace std;

// Función principal
int main() {
    int numJugadores = 0;

    cout << "Bienvenido al juego UNO-Flip!" << endl;

    while (true) {
        cout << "\nMenu principal:" << endl;
        cout << "1. Seleccionar cantidad de jugadores (2 a 10)" << endl;
        cout << "2. Salir" << endl;
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

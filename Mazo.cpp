#include "Mazo.h"
#include <cstdlib> // rand
#include <ctime>   //  time

// Cartas Posibles
Mazo::Mazo() {
    string colores[] = {"rojo", "azul", "verde", "amarillo"};
    string numeros[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    string especiales[] = {"salto", "reversa", "+2"};
    
    // Crear cartas numericas
    for (string color : colores) {
        for (string num : numeros) {
            cartas.push_back(Carta(color, num));
        }
        // Crear cartas especiales (2 por color)
        for (string esp : especiales) {
            cartas.push_back(Carta(color, esp));
            cartas.push_back(Carta(color, esp));
        }
    }
    // 
    for (int i = 0; i < 4; ++i) {
        cartas.push_back(Carta("comodin", "cambioColor"));
        cartas.push_back(Carta("comodin", "cambioColor+4"));
    }

    // Semilla numeros random
    srand(time(0));
}

// de forma random se baraja el mazo
void Mazo::barajar() {
    random_shuffle(cartas.begin(), cartas.end());
}

// saca una carta al azar del mazo
Carta Mazo::sacarCarta() {
    Carta carta = cartas.back();
    cartas.pop_back();
    return carta;
}

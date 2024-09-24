#include "Mazo.h"
#include <cstdlib> // rand
#include <ctime>   // time
#include <algorithm> 

// Constructor que crea las cartas posibles para el mazo
Mazo::Mazo() {
    string coloresLight[] = {"rojo", "azul", "verde", "amarillo"};
    string numeros[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    string especialesLight[] = {"salto", "reversa", "draw1"}; // Cambia +2 por draw1
    string coloresDark[] = {"rosa", "teal", "morado", "naranja"};
    string especialesDark[] = {"draw5", "skipEveryone", "wild", "wildDrawColor"};

    // Crear cartas numéricas del lado Light
    for (string color : coloresLight) {
        for (string num : numeros) {
            cartas.push_back(Carta(color, num, "", ""));
        }
        // Crear cartas especiales del lado Light (2 por color)
        for (string esp : especialesLight) {
            cartas.push_back(Carta(color, esp, "", ""));
            cartas.push_back(Carta(color, esp, "", ""));
        }
    }

    // Crear cartas numéricas del lado Dark
    for (string color : coloresDark) {
        for (string num : numeros) {
            cartas.push_back(Carta("", "", color, num));
        }
        // Crear cartas especiales del lado Dark (2 por color)
        for (string esp : especialesDark) {
            cartas.push_back(Carta("", "", color, esp));
            cartas.push_back(Carta("", "", color, esp));
        }
    }

    // Agregar comodines
    for (int i = 0; i < 4; ++i) {
        cartas.push_back(Carta("comodin", "cambioColor", "comodin", "wildDrawColor"));
        cartas.push_back(Carta("comodin", "cambioColor+4", "comodin", "wildDrawColor"));
    }

    // Semilla para generar números aleatorios
    srand(time(0));
}

// Barajar el mazo de cartas de forma aleatoria
void Mazo::barajar() {
    random_shuffle(cartas.begin(), cartas.end());
}

// Sacar una carta al azar del mazo
Carta Mazo::sacarCarta() {
    Carta carta = cartas.back();
    cartas.pop_back();
    return carta;
}

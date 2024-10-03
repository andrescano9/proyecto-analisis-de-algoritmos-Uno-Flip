// Jugador.cpp

#include "Jugador.h"
#include <iostream>

// Método que muestra las cartas en la mano del jugador
void Jugador::mostrarMano(bool isLight) const {
    std::cout << "Mano de " << nombre << ":" << std::endl;
    for (size_t i = 0; i < mano.size(); ++i) {
        // Mostrar solo el lado actual de la carta
        std::cout << i + 1 << ": "
                  << mano[i].getColorActual(isLight) << " " << mano[i].getNumeroActual(isLight) << std::endl;
    }
}

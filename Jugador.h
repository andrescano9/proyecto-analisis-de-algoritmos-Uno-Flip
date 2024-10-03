// Jugador.h

#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <string>
#include "Carta.h"

class Jugador {
public:
    std::string nombre;
    std::vector<Carta> mano;

    // Constructor que inicializa el nombre del jugador
    Jugador(std::string nombreJugador) : nombre(nombreJugador) {}

    // Método que agrega una carta a la mano del jugador
    void agregarCarta(const Carta& carta) {
        mano.push_back(carta);
    }

    // Método que muestra las cartas en la mano del jugador con un parámetro
    void mostrarMano(bool isLight) const; // Modificar aquí para aceptar un argumento booleano
};

#endif // JUGADOR_H

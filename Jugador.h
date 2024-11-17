#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include <string>
#include "Carta.h"

class Jugador {
public:
    std::string nombre;
    bool isBot;  // Atributo para identificar si es un bot
    std::vector<Carta> mano;

    // Constructor para jugador humano
    Jugador(std::string nombreJugador) : nombre(nombreJugador), isBot(false) {}

    // Constructor para jugador bot
    Jugador(std::string nombreJugador, bool esBot) : nombre(nombreJugador), isBot(esBot) {}

    // Método que agrega una carta a la mano del jugador
    void agregarCarta(const Carta& carta) {
        mano.push_back(carta);
    }

    // Método que muestra las cartas en la mano del jugador
    void mostrarMano(bool isLight) const;

    // Método para verificar si el jugador es un bot
    bool esBot() const {
        return isBot;
    }
};

#endif // JUGADOR_H

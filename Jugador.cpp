#include "Jugador.h"

Jugador::Jugador(string n) : nombre(n) {}

// Metodo que agrega una carta a la mano del jugador
void Jugador::agregarCarta(Carta carta) {
    mano.push_back(carta);
}

#ifndef MAZO_H
#define MAZO_H

#include <vector>
#include <algorithm> // para barajar
#include "Carta.h"

// Mazo.h

class Mazo {
private:
    vector<Carta> cartas; // Cartas del mazo
    Carta cartaActiva;    // Carta activa en la mesa

public:
    Mazo(); // Constructor
    void barajar();
    Carta sacarCarta();
    void setCartaActiva(const Carta& carta); // Establece la carta activa
    Carta getCartaActiva() const;           // Devuelve la carta activa
    int getNumCartas() const;                // Devuelve el número de cartas en el mazo
};

// Mazo.cpp

void Mazo::setCartaActiva(const Carta& carta) {
    cartaActiva = carta;
}

Carta Mazo::getCartaActiva() const {
    return cartaActiva;
}

int Mazo::getNumCartas() const {
    return cartas.size();
}


#endif

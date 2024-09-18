#ifndef JUGADOR_H
#define JUGADOR_H

#include <vector>
#include "Carta.h"

class Jugador {
public:
    string nombre;
    vector<Carta> mano;

    Jugador(string n);
    void agregarCarta(Carta carta);
};

#endif

#ifndef MAZO_H
#define MAZO_H

#include <vector>
#include <algorithm> // para barajar
#include "Carta.h"

class Mazo {
private:
    vector<Carta> cartas;

public:
    Mazo();
    void barajar();
    Carta sacarCarta();
};

#endif

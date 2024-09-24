#ifndef MAZO_H
#define MAZO_H

#include <vector>
#include <algorithm> // para barajar
#include "Carta.h"

class Mazo {
private:
    std::vector<Carta> cartas;  // Asegúrate de usar std::vector para que funcione correctamente

public:
    Mazo();
    void barajar();
    Carta sacarCarta();
};

#endif

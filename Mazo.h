#ifndef MAZO_H
#define MAZO_H

#include <vector>
#include <algorithm> // para barajar
#include "Carta.h"

// Clase Mazo
class Mazo {
private:
    vector<Carta> cartas;        // Cartas del mazo
    vector<Carta> mazoDescarte;  // Cartas descartadas
    Carta cartaActiva;           // Carta activa en la mesa

public:
    Mazo(); // Constructor
    void barajar();
    Carta sacarCarta();
    void devolverCartaDescarte(const Carta& carta); // Devolver carta al mazo de descarte
    void setCartaActiva(const Carta& carta); // Establece la carta activa
    Carta getCartaActiva() const;           // Devuelve la carta activa
    int getNumCartas() const;                // Devuelve el número de cartas en el mazo
};

#endif 

#ifndef MAZO_H
#define MAZO_H

#include <vector>
#include <string>
#include <algorithm> // Para barajar
#include "Carta.h"

// Clase Mazo
class Mazo {
private:
    std::vector<Carta> cartas;        // Cartas en el mazo
    std::vector<Carta> mazoDescarte;  // Cartas en el mazo de descarte
    Carta cartaActiva;                 // Carta activa en la mesa

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

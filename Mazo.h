#ifndef MAZO_H
#define MAZO_H

#include <vector>
#include "Carta.h"

class Mazo {
public:
    std::vector<Carta> cartas;        // Vector para almacenar las cartas
    std::vector<Carta> mazoDescarte;  // Vector para almacenar las cartas descartadas
    Carta cartaActiva;                 // Carta activa en juego

    // Constructor para inicializar el mazo
    Mazo();

    // Método para sacar una carta del mazo
    Carta sacarCarta();

    // Método para devolver carta al mazo de descarte
    void devolverCartaDescarte(const Carta& carta);

    // Establece la carta activa
    void setCartaActiva(const Carta& carta);

    // Devuelve la carta activa
    Carta getCartaActiva() const;

    // Devuelve el número de cartas en el mazo
    int getNumCartas() const;

    // Método para barajar el mazo
    void barajar();

    // Mostrar el mazo de cartas
    void mostrarMazo() const;

    // Obtener el tamaño del mazo
    int size() const;

    // Obtener el tamaño del mazo de descarte
    int sizeDescarte() const;

    // Obtener el mazo de descarte
    std::vector<Carta> getMazoDescarte() const;
};

#endif // MAZO_H

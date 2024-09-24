#include "Carta.h"

// Constructor que inicializa la carta con colores y valores para ambos lados
Carta::Carta(string colorL, string valorL, string colorD, string valorD)
    : colorLight(colorL), valorLight(valorL), colorDark(colorD), valorDark(valorD), ladoLight(true) {}

// Retorna el color de la carta según el lado activo
string Carta::getColorActual() const {
    return ladoLight ? colorLight : colorDark;
}

// Retorna el valor de la carta según el lado activo
string Carta::getValorActual() const {
    return ladoLight ? valorLight : valorDark;
}

// Cambia el lado activo de la carta (Light <-> Dark)
void Carta::cambiarLado() {
    ladoLight = !ladoLight;
}

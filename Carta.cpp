#include "Carta.h"
#include <iostream>

Carta::Carta() : colorLight(""), numeroLight(""), colorDark(""), numeroDark("") {}

Carta::Carta(const std::string& colorLight, const std::string& numeroLight, const std::string& colorDark, const std::string& numeroDark)
    : colorLight(colorLight), numeroLight(numeroLight), colorDark(colorDark), numeroDark(numeroDark) {}

// Retorna el color actual dependiendo del lado (light o dark)
std::string Carta::getColorActual(bool isLight) const {
    return isLight ? colorLight : colorDark;
}

// Retorna el número actual dependiendo del lado (light o dark)
std::string Carta::getNumeroActual(bool isLight) const {
    return isLight ? numeroLight : numeroDark;
}

// Muestra los valores de la carta en ambos lados
void Carta::mostrarCarta() const {
    std::cout << "Color Light: " << colorLight << ", Numero Light: " << numeroLight << " || "
              << "Color Dark: " << colorDark << ", Numero Dark: " << numeroDark << std::endl;
}

// Métodos de modificación para cada componente de la carta
void Carta::setColorLight(const std::string& color) {
    colorLight = color;
}

void Carta::setNumeroLight(const std::string& numero) {
    numeroLight = numero;
}

void Carta::setColorDark(const std::string& color) {
    colorDark = color;
}

void Carta::setNumeroDark(const std::string& numero) {
    numeroDark = numero;
}

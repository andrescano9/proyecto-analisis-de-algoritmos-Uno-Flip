#ifndef CARTA_H
#define CARTA_H

#include <string>

class Carta {
public:
    std::string colorLight;   // Color del lado 'light'
    std::string numeroLight;  // Número del lado 'light'
    std::string colorDark;    // Color del lado 'dark'
    std::string numeroDark;   // Número del lado 'dark'
    bool esAccion;            // Indica si la carta es una carta de acción

public:
    // Constructor predeterminado
    Carta();

    // Constructor que acepta valores para ambos lados
    Carta(const std::string& colorLight, const std::string& numeroLight, const std::string& colorDark, const std::string& numeroDark);

    // Métodos para obtener el color y número según el lado actual
    std::string getColorActual(bool isLight) const; // Devuelve el color según el lado
    std::string getNumeroActual(bool isLight) const; // Devuelve el número según el lado
    void mostrarCarta() const; // Muestra los detalles de la carta

    // Métodos de configuración
    void setColorLight(const std::string& color);
    void setNumeroLight(const std::string& numero);
    void setColorDark(const std::string& color);
    void setNumeroDark(const std::string& numero);

    // Método para establecer si la carta es de acción
    void setEsAccion(bool esAccion);
};

#endif // CARTA_H

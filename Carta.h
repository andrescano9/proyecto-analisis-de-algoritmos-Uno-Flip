#ifndef CARTA_H
#define CARTA_H

#include <string>
using namespace std;

class Carta {
public:
    string colorLight, colorDark;  // Colores para los lados Light y Dark
    string valorLight, valorDark;  // Valores para los lados Light y Dark
    bool ladoLight;  // True si estamos en el lado Light, false si estamos en el lado Dark

    // Constructor que inicializa los valores para ambos lados
    Carta(string colorL, string valorL, string colorD, string valorD);

    // Obtener el color actual según el lado activo
    string getColorActual() const;

    // Obtener el valor actual según el lado activo
    string getValorActual() const;

    // Cambiar de lado (Light a Dark o viceversa)
    void cambiarLado();
};

#endif

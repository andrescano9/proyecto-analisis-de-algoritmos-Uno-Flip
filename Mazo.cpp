#include "Mazo.h"
#include <cstdlib> // rand
#include <ctime>   // time
#include <algorithm>
#include <stdexcept> // Para std::runtime_error
#include <random> // Necesario para std::shuffle

Mazo::Mazo() {
    std::string coloresLight[] = {"rojo", "azul", "verde", "amarillo"};
    std::string numeros[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::string especialesLight[] = {"salto", "reversa", "draw1"};
    std::string coloresDark[] = {"rosa", "teal", "morado", "naranja"};
    std::string especialesDark[] = {"draw5", "skipEveryone", "wild", "wildDrawColor"};

    // Crear cartas numéricas del lado Light
    for (const std::string& color : coloresLight) {
        for (const std::string& num : numeros) {
            cartas.push_back(Carta(color, num, color, num)); // Crear carta con parámetros completos
        }
        // Crear cartas especiales del lado Light (2 por color)
        for (const std::string& esp : especialesLight) {
            cartas.push_back(Carta(color, esp, color, esp)); // Carta especial Light
            cartas.push_back(Carta(color, esp, color, esp)); // Otra carta especial Light
        }
    }

    // Crear cartas numéricas del lado Dark
    for (const std::string& color : coloresDark) {
        for (const std::string& num : numeros) {
            cartas.push_back(Carta(color, num, color, num)); // Crear carta con parámetros completos
        }
        // Crear cartas especiales del lado Dark (2 por color)
        for (const std::string& esp : especialesDark) {
            cartas.push_back(Carta(color, esp, color, esp)); // Carta especial Dark
            cartas.push_back(Carta(color, esp, color, esp)); // Otra carta especial Dark
        }
    }

    // Agregar comodines
    for (int i = 0; i < 4; ++i) {
        cartas.push_back(Carta("comodin", "cambioColor", "comodin", "wildDrawColor")); // Comodín
        cartas.push_back(Carta("comodin", "cambioColor+2", "comodin", "wildDrawColor")); // Comodín
    }

    // Semilla para generar números aleatorios
    srand(static_cast<unsigned>(time(0))); // Asegura que la semilla sea adecuada
}

// Sacar una carta al azar del mazo
Carta Mazo::sacarCarta() {
    if (cartas.empty()) {
        // Si no quedan cartas en el mazo, transferir cartas del mazo de descarte
        if (!mazoDescarte.empty()) {
            cartas = mazoDescarte; // Transferir cartas del mazo de descarte al mazo
            mazoDescarte.clear();  // Limpiar el mazo de descarte
            barajar();             // Barajar el nuevo mazo
        } else {
            // Si no hay cartas en el mazo ni en el mazo de descarte, lanzar una excepción
            throw std::runtime_error("No hay cartas disponibles en el mazo.");
        }
    }

    // Asegúrate de que haya al menos una carta para devolver
    if (cartas.empty()) {
        throw std::runtime_error("No hay cartas disponibles para sacar.");
    }

    // Se devuelven cartas del mazo
    Carta carta = cartas.back();
    cartas.pop_back();
    return carta;
}

// Devolver carta al mazo de descarte
void Mazo::devolverCartaDescarte(const Carta& carta) {
    mazoDescarte.push_back(carta);
}

// Establece la carta activa
void Mazo::setCartaActiva(const Carta& carta) {
    cartaActiva = carta;
}

// Devuelve la carta activa
Carta Mazo::getCartaActiva() const {
    return cartaActiva;
}

// Devuelve el número de cartas en el mazo
int Mazo::getNumCartas() const {
    return cartas.size();
}

// Método para barajar el mazo
void Mazo::barajar() {
    std::random_device rd; // Obtener un generador de números aleatorios
    std::mt19937 g(rd()); // Usar un motor de aleatoriedad
    std::shuffle(cartas.begin(), cartas.end(), g); // Barajar el mazo
}


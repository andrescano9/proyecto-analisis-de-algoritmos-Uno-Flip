#include "Mazo.h"
#include <cstdlib> // rand
#include <ctime>   // time
#include <algorithm>
#include <stdexcept> // Para std::runtime_error
#include <random> // Necesario para std::shuffle

Mazo::Mazo() {
    // Crear cartas con un solo tipo que tenga ambos lados
    // Rojo
    cartas.push_back(Carta("rojo", "0", "naranja", "0"));
    cartas.push_back(Carta("rojo", "1", "naranja", "1"));
    cartas.push_back(Carta("rojo", "2", "naranja", "2"));
    cartas.push_back(Carta("rojo", "3", "naranja", "3"));
    cartas.push_back(Carta("rojo", "4", "naranja", "4"));
    cartas.push_back(Carta("rojo", "5", "naranja", "5"));
    cartas.push_back(Carta("rojo", "6", "naranja", "6"));
    cartas.push_back(Carta("rojo", "7", "naranja", "7"));
    cartas.push_back(Carta("rojo", "8", "naranja", "8"));
    cartas.push_back(Carta("rojo", "9", "naranja", "9"));

    // Amarillo
    cartas.push_back(Carta("amarillo", "0", "agua marina", "0"));
    cartas.push_back(Carta("amarillo", "1", "agua marina", "1"));
    cartas.push_back(Carta("amarillo", "2", "agua marina", "2"));
    cartas.push_back(Carta("amarillo", "3", "agua marina", "3"));
    cartas.push_back(Carta("amarillo", "4", "agua marina", "4"));
    cartas.push_back(Carta("amarillo", "5", "agua marina", "5"));
    cartas.push_back(Carta("amarillo", "6", "agua marina", "6"));
    cartas.push_back(Carta("amarillo", "7", "agua marina", "7"));
    cartas.push_back(Carta("amarillo", "8", "agua marina", "8"));
    cartas.push_back(Carta("amarillo", "9", "agua marina", "9"));

    // Verde
    cartas.push_back(Carta("verde", "0", "rosa", "0"));
    cartas.push_back(Carta("verde", "1", "rosa", "1"));
    cartas.push_back(Carta("verde", "2", "rosa", "2"));
    cartas.push_back(Carta("verde", "3", "rosa", "3"));
    cartas.push_back(Carta("verde", "4", "rosa", "4"));
    cartas.push_back(Carta("verde", "5", "rosa", "5"));
    cartas.push_back(Carta("verde", "6", "rosa", "6"));
    cartas.push_back(Carta("verde", "7", "rosa", "7"));
    cartas.push_back(Carta("verde", "8", "rosa", "8"));
    cartas.push_back(Carta("verde", "9", "rosa", "9"));

    // Azul
    cartas.push_back(Carta("azul", "0", "morado", "0"));
    cartas.push_back(Carta("azul", "1", "morado", "1"));
    cartas.push_back(Carta("azul", "2", "morado", "2"));
    cartas.push_back(Carta("azul", "3", "morado", "3"));
    cartas.push_back(Carta("azul", "4", "morado", "4"));
    cartas.push_back(Carta("azul", "5", "morado", "5"));
    cartas.push_back(Carta("azul", "6", "morado", "6"));
    cartas.push_back(Carta("azul", "7", "morado", "7"));
    cartas.push_back(Carta("azul", "8", "morado", "8"));
    cartas.push_back(Carta("azul", "9", "morado", "9"));

    // Comodines por color
    // Rojo
    cartas.push_back(Carta("rojo", "mas1", "naranja", "mas5")); // Rojo con Naranja
    cartas.push_back(Carta("rojo", "reversa", "naranja", "reversa")); // Rojo con Naranja
    cartas.push_back(Carta("rojo", "bloqueo", "naranja", "bloquearTodos")); // Rojo con Naranja
    cartas.push_back(Carta("rojo", "flip", "naranja", "flip")); // Rojo con Naranja

    // Amarillo
    cartas.push_back(Carta("amarillo", "mas1", "agua marina", "mas5")); // Amarillo con Agua Marina
    cartas.push_back(Carta("amarillo", "reversa", "agua marina", "reversa")); // Amarillo con Agua Marina
    cartas.push_back(Carta("amarillo", "bloqueo", "agua marina", "bloquearTodos")); // Amarillo con Agua Marina
    cartas.push_back(Carta("amarillo", "flip", "agua marina", "flip")); // Amarillo con Agua Marina

    // Verde
    cartas.push_back(Carta("verde", "mas1", "rosa", "mas5")); // Verde con Rosa
    cartas.push_back(Carta("verde", "reversa", "rosa", "reversa")); // Verde con Rosa
    cartas.push_back(Carta("verde", "bloqueo", "rosa", "bloquearTodos")); // Verde con Rosa
    cartas.push_back(Carta("verde", "flip", "rosa", "flip")); // Verde con Rosa

    // Azul
    cartas.push_back(Carta("azul", "mas1", "morado", "mas5")); // Azul con Morado
    cartas.push_back(Carta("azul", "reversa", "morado", "reversa")); // Azul con Morado
    cartas.push_back(Carta("azul", "bloqueo", "morado", "bloquearTodos")); // Azul con Morado
    cartas.push_back(Carta("azul", "flip", "morado", "flip")); // Azul con Morado

    // Comodines
    cartas.push_back(Carta("comodin", "CambioColor", "comodin", "CambioColor"));
    cartas.push_back(Carta("comodin", "CambioColorMas2", "comodin", "CambioColorWild"));
    cartas.push_back(Carta("comodin", "CambioColor", "comodin", "CambioColor"));
    cartas.push_back(Carta("comodin", "CambioColorMas2", "comodin", "CambioColorWild"));
    cartas.push_back(Carta("comodin", "CambioColor", "comodin", "CambioColor"));
    cartas.push_back(Carta("comodin", "CambioColorMas2", "comodin", "CambioColorWild"));

    // Semilla para generar números aleatorios
    srand(static_cast<unsigned>(time(0)));

    barajar(); // Barajar el mazo al inicializar
}

// Sacar una carta al azar del mazo
Carta Mazo::sacarCarta() {
    if (cartas.empty()) {
        if (!mazoDescarte.empty()) {
            cartas = mazoDescarte; // Restaurar el mazo desde el mazo de descarte
            mazoDescarte.clear(); // Limpiar el mazo de descarte
            barajar(); // Barajar las cartas restauradas
        } else {
            throw std::runtime_error("No hay cartas disponibles en el mazo."); // Lanzar excepción si no hay cartas
        }
    }

    Carta cartaSacada = cartas.back(); // Sacar la carta de arriba
    cartas.pop_back(); // Eliminarla del mazo
    return cartaSacada; // Devolver la carta
}

// Devolver carta al mazo de descarte
void Mazo::devolverCartaDescarte(const Carta& carta) {
    mazoDescarte.push_back(carta); // Añadir la carta al mazo de descarte
}

// Establecer la carta activa
void Mazo::setCartaActiva(const Carta& carta) {
    cartaActiva = carta; // Establecer la carta activa
}

// Obtener la carta activa
Carta Mazo::getCartaActiva() const {
    return cartaActiva; // Devolver la carta activa
}

// Obtener el número de cartas en el mazo
int Mazo::getNumCartas() const {
    return cartas.size(); // Devolver el tamaño del mazo
}

// Método para barajar el mazo
void Mazo::barajar() {
    std::shuffle(cartas.begin(), cartas.end(), std::default_random_engine(std::random_device()())); // Barajar las cartas
}

// Mostrar el mazo de cartas
void Mazo::mostrarMazo() const {
    for (const auto& carta : cartas) {
        carta.mostrarCarta(); // Mostrar cada carta
    }
}

// Obtener el tamaño del mazo de descarte
int Mazo::sizeDescarte() const {
    return mazoDescarte.size(); // Devolver el tamaño del mazo de descarte
}

// Obtener el mazo de descarte
std::vector<Carta> Mazo::getMazoDescarte() const {
    return mazoDescarte; // Devolver el mazo de descarte
}

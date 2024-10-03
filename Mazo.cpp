#include "Mazo.h"
#include <cstdlib> // rand
#include <ctime>   // time
#include <algorithm>
#include <stdexcept> // Para std::runtime_error
#include <random> // Necesario para std::shuffle

Mazo::Mazo() {
    // Crear cartas con un solo tipo que tenga ambos lados
    // Rojo
    cartas.push_back(Carta("rojo", "0", "naranja", "5"));
    cartas.push_back(Carta("rojo", "1", "naranja", "6"));
    cartas.push_back(Carta("rojo", "2", "naranja", "7"));
    cartas.push_back(Carta("rojo", "3", "naranja", "8"));
    cartas.push_back(Carta("rojo", "4", "naranja", "9"));
    cartas.push_back(Carta("rojo", "5", "naranja", "draw1"));
    cartas.push_back(Carta("rojo", "6", "naranja", "wildDraw2"));
    cartas.push_back(Carta("rojo", "7", "naranja", "flip"));
    cartas.push_back(Carta("rojo", "8", "naranja", "reversa"));
    cartas.push_back(Carta("rojo", "9", "naranja", "skip"));

    // Azul
    cartas.push_back(Carta("azul", "0", "agua marina", "5"));
    cartas.push_back(Carta("azul", "1", "agua marina", "6"));
    cartas.push_back(Carta("azul", "2", "agua marina", "7"));
    cartas.push_back(Carta("azul", "3", "agua marina", "8"));
    cartas.push_back(Carta("azul", "4", "agua marina", "9"));
    cartas.push_back(Carta("azul", "5", "agua marina", "draw1"));
    cartas.push_back(Carta("azul", "6", "agua marina", "wildDraw2"));
    cartas.push_back(Carta("azul", "7", "agua marina", "flip"));
    cartas.push_back(Carta("azul", "8", "agua marina", "reversa"));
    cartas.push_back(Carta("azul", "9", "agua marina", "skip"));

    // Verde
    cartas.push_back(Carta("verde", "0", "rosa", "5"));
    cartas.push_back(Carta("verde", "1", "rosa", "6"));
    cartas.push_back(Carta("verde", "2", "rosa", "7"));
    cartas.push_back(Carta("verde", "3", "rosa", "8"));
    cartas.push_back(Carta("verde", "4", "rosa", "9"));
    cartas.push_back(Carta("verde", "5", "rosa", "draw1"));
    cartas.push_back(Carta("verde", "6", "rosa", "wildDraw2"));
    cartas.push_back(Carta("verde", "7", "rosa", "flip"));
    cartas.push_back(Carta("verde", "8", "rosa", "reversa"));
    cartas.push_back(Carta("verde", "9", "rosa", "skip"));

    // Azul morado
    cartas.push_back(Carta("azul", "0", "morado", "5"));
    cartas.push_back(Carta("azul", "1", "morado", "6"));
    cartas.push_back(Carta("azul", "2", "morado", "7"));
    cartas.push_back(Carta("azul", "3", "morado", "8"));
    cartas.push_back(Carta("azul", "4", "morado", "9"));
    cartas.push_back(Carta("azul", "5", "morado", "draw1"));
    cartas.push_back(Carta("azul", "6", "morado", "wildDraw2"));
    cartas.push_back(Carta("azul", "7", "morado", "flip"));
    cartas.push_back(Carta("azul", "8", "morado", "reversa"));
    cartas.push_back(Carta("azul", "9", "morado", "skip"));

    // Comodines
    for (int i = 0; i < 4; ++i) {
        cartas.push_back(Carta("comodin", "cambioColor", "comodin", "wildDrawColor")); // Comodín
        cartas.push_back(Carta("comodin", "cambioColor+2", "comodin", "wildDrawColor")); // Comodín
    }

    // Semilla para generar números aleatorios
    srand(static_cast<unsigned>(time(0)));

    barajar(); // Barajar el mazo al inicializar
}

// Sacar una carta al azar del mazo
Carta Mazo::sacarCarta() {
    if (cartas.empty()) {
        if (!mazoDescarte.empty()) {
            cartas = mazoDescarte;
            mazoDescarte.clear();
            barajar();
        } else {
            throw std::runtime_error("No hay cartas disponibles en el mazo.");
        }
    }

    Carta cartaSacada = cartas.back();
    cartas.pop_back();
    return cartaSacada;
}

// Devolver carta al mazo de descarte
void Mazo::devolverCartaDescarte(const Carta& carta) {
    mazoDescarte.push_back(carta);
}

// Establecer la carta activa
void Mazo::setCartaActiva(const Carta& carta) {
    cartaActiva = carta;
}

// Obtener la carta activa
Carta Mazo::getCartaActiva() const {
    return cartaActiva;
}

// Obtener el número de cartas en el mazo
int Mazo::getNumCartas() const {
    return cartas.size();
}

// Método para barajar el mazo
void Mazo::barajar() {
    std::shuffle(cartas.begin(), cartas.end(), std::default_random_engine(std::random_device()()));
}

// Mostrar el mazo de cartas
void Mazo::mostrarMazo() const {
    for (const auto& carta : cartas) {
        carta.mostrarCarta(); // Suponiendo que hay un método mostrarCarta en la clase Carta
    }
}

// Obtener el tamaño del mazo
int Mazo::size() const {
    return cartas.size();
}

// Obtener el tamaño del mazo de descarte
int Mazo::sizeDescarte() const {
    return mazoDescarte.size();
}

// Obtener el mazo de descarte
std::vector<Carta> Mazo::getMazoDescarte() const {
    return mazoDescarte;
}

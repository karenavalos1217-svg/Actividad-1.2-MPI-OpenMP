#ifndef OPERACIONES_ARREGLOS_H
#define OPERACIONES_ARREGLOS_H

#include <string>

class OperacionesArreglos {
public:

    // Llena una sección del arreglo.
    // Si detalle es true, muestra qué proceso e hilo creó cada elemento.
    static void crearArregloMPI(
        int* arreglo,
        int cantidad,
        int posicionInicial,
        int proceso,
        const std::string& equipo,
        bool aleatorio,
        bool detalle
    );

    // Suma dos arreglos elemento por elemento.
    static void sumar(
        int* A,
        int* B,
        int* resultado,
        int cantidad,
        int posicionInicial,
        int proceso,
        const std::string& equipo,
        bool detalle
    );

    // Resta B de A elemento por elemento.
    static void restar(
        int* A,
        int* B,
        int* resultado,
        int cantidad,
        int posicionInicial,
        int proceso,
        const std::string& equipo,
        bool detalle
    );

    // Multiplica A y B elemento por elemento.
    static void multiplicar(
        int* A,
        int* B,
        long long* resultado,
        int cantidad,
        int posicionInicial,
        int proceso,
        const std::string& equipo,
        bool detalle
    );

    // Calcula el cuadrado de cada elemento del arreglo A.
    static void cuadrado(
        int* A,
        long long* resultado,
        int cantidad,
        int posicionInicial,
        int proceso,
        const std::string& equipo,
        bool detalle
    );
};

#endif

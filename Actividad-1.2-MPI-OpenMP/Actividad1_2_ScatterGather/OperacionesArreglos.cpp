#include "OperacionesArreglos.h"

#include <iostream>
#include <cstdlib>
#include <omp.h>

using namespace std;

// Crea los valores de la sección local del arreglo.
// OpenMP reparte las posiciones entre los hilos disponibles.
void OperacionesArreglos::crearArregloMPI(
    int* arreglo,
    int cantidad,
    int posicionInicial,
    int proceso,
    const string& equipo,
    bool aleatorio,
    bool detalle
) {
    #pragma omp parallel for
    for (int i = 0; i < cantidad; i++) {

        int hilo = omp_get_thread_num();

        if (aleatorio) {
            // Se genera un valor entre 1 y 1,000,000.
            // rand_r utiliza una semilla independiente para evitar
            // que varios hilos modifiquen la misma semilla.
            unsigned int semilla =
                1234u + (proceso * 100000u) +
                (i * 97u) + (hilo * 31u);

semilla = semilla * 1103515245u + 12345u;
arreglo[i] = 1 + (semilla % 1000000u);
        } else {
            // Para la prueba de 40 elementos usamos valores consecutivos.
            arreglo[i] = posicionInicial + i + 1;
        }

        if (detalle) {
            #pragma omp critical
            {
                cout << "Equipo: " << equipo
                     << " | Proceso MPI: " << proceso
                     << " | Hilo OpenMP: " << hilo
                     << " | Posicion: " << posicionInicial + i
                     << " | Valor generado: " << arreglo[i]
                     << endl;
            }
        }
    }
}


// Suma A + B elemento por elemento.
void OperacionesArreglos::sumar(
    int* A,
    int* B,
    int* resultado,
    int cantidad,
    int posicionInicial,
    int proceso,
    const string& equipo,
    bool detalle
) {
    #pragma omp parallel for
    for (int i = 0; i < cantidad; i++) {

        resultado[i] = A[i] + B[i];

        if (detalle) {
            int hilo = omp_get_thread_num();

            #pragma omp critical
            {
                cout << "Equipo: " << equipo
                     << " | Proceso MPI: " << proceso
                     << " | Hilo OpenMP: " << hilo
                     << " | Posicion: " << posicionInicial + i
                     << " | Suma: " << A[i]
                     << " + " << B[i]
                     << " = " << resultado[i]
                     << endl;
            }
        }
    }
}


// Resta A - B elemento por elemento.
void OperacionesArreglos::restar(
    int* A,
    int* B,
    int* resultado,
    int cantidad,
    int posicionInicial,
    int proceso,
    const string& equipo,
    bool detalle
) {
    #pragma omp parallel for
    for (int i = 0; i < cantidad; i++) {

        resultado[i] = A[i] - B[i];

        if (detalle) {
            int hilo = omp_get_thread_num();

            #pragma omp critical
            {
                cout << "Equipo: " << equipo
                     << " | Proceso MPI: " << proceso
                     << " | Hilo OpenMP: " << hilo
                     << " | Posicion: " << posicionInicial + i
                     << " | Resta: " << A[i]
                     << " - " << B[i]
                     << " = " << resultado[i]
                     << endl;
            }
        }
    }
}


// Multiplica A * B elemento por elemento.
// Se usa long long porque con valores de hasta 1,000,000
// el resultado puede ser mayor al limite de un int.
void OperacionesArreglos::multiplicar(
    int* A,
    int* B,
    long long* resultado,
    int cantidad,
    int posicionInicial,
    int proceso,
    const string& equipo,
    bool detalle
) {
    #pragma omp parallel for
    for (int i = 0; i < cantidad; i++) {

        resultado[i] =
            static_cast<long long>(A[i]) * B[i];

        if (detalle) {
            int hilo = omp_get_thread_num();

            #pragma omp critical
            {
                cout << "Equipo: " << equipo
                     << " | Proceso MPI: " << proceso
                     << " | Hilo OpenMP: " << hilo
                     << " | Posicion: " << posicionInicial + i
                     << " | Multiplicacion: " << A[i]
                     << " * " << B[i]
                     << " = " << resultado[i]
                     << endl;
            }
        }
    }
}


// Calcula A[i] al cuadrado.
void OperacionesArreglos::cuadrado(
    int* A,
    long long* resultado,
    int cantidad,
    int posicionInicial,
    int proceso,
    const string& equipo,
    bool detalle
) {
    #pragma omp parallel for
    for (int i = 0; i < cantidad; i++) {

        resultado[i] =
            static_cast<long long>(A[i]) * A[i];

        if (detalle) {
            int hilo = omp_get_thread_num();

            #pragma omp critical
            {
                cout << "Equipo: " << equipo
                     << " | Proceso MPI: " << proceso
                     << " | Hilo OpenMP: " << hilo
                     << " | Posicion: " << posicionInicial + i
                     << " | Cuadrado: " << A[i]
                     << "^2 = " << resultado[i]
                     << endl;
            }
        }
    }
}

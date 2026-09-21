/*
    Integrantes:
    Avalos Oliva Karen Lizeth
    Chavez Torres Oliver Daniel
    Ramirez Arvizu Fernando David
*/

#include "OperacionesArreglos.h"
#include <omp.h>
#include <stdexcept>

// Suma de arreglos
void OperacionesArreglos::sumarArreglosOpenMP(
    int* A, int* B, int* R, int n)
{
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        R[i] = A[i] + B[i];
    }
}

// Resta de arreglos
void OperacionesArreglos::restarArreglosOpenMP(
    int* A, int* B, int* R, int n)
{
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        R[i] = A[i] - B[i];
    }
}

// Multiplicacion de arreglos
void OperacionesArreglos::multiplicarArreglosOpenMP(
    int* A, int* B, int* R, int n)
{
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        R[i] = A[i] * B[i];
    }
}

// Cuadrado de un arreglo
void OperacionesArreglos::cuadradoArregloOpenMP(
    int* A, int* R, int n)
{
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        R[i] = A[i] * A[i];
    }
}

// Sumatoria de un arreglo
void OperacionesArreglos::SumatoriaOpenMP(
    int* A, long long* R, int n)
{
    if (A == nullptr || R == nullptr)
    {
        throw std::invalid_argument(
            "Error: el arreglo o resultado es nulo.");
    }

    if (n <= 0)
    {
        throw std::invalid_argument(
            "Error: el tamano del arreglo debe ser mayor a cero.");
    }

    long long suma = 0;

    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < n; i++)
    {
        suma += A[i];
    }

    R[0] = suma;
}

// Maximo de un arreglo
void OperacionesArreglos::MaximoOpenMP(
    int* A, int* R, int n)
{
    if (A == nullptr || R == nullptr)
    {
        throw std::invalid_argument(
            "Error: el arreglo o resultado es nulo.");
    }

    if (n <= 0)
    {
        throw std::invalid_argument(
            "Error: el tamano del arreglo debe ser mayor a cero.");
    }

    int maximo = A[0];

    #pragma omp parallel for reduction(max:maximo)
    for (int i = 0; i < n; i++)
    {
        if (A[i] > maximo)
        {
            maximo = A[i];
        }
    }

    R[0] = maximo;
}

// Minimo de un arreglo
void OperacionesArreglos::MinimoOpenMP(
    int* A, int* R, int n)
{
    if (A == nullptr || R == nullptr)
    {
        throw std::invalid_argument(
            "Error: el arreglo o resultado es nulo.");
    }

    if (n <= 0)
    {
        throw std::invalid_argument(
            "Error: el tamano del arreglo debe ser mayor a cero.");
    }

    int minimo = A[0];

    #pragma omp parallel for reduction(min:minimo)
    for (int i = 0; i < n; i++)
    {
        if (A[i] < minimo)
        {
            minimo = A[i];
        }
    }

    R[0] = minimo;
}

// Promedio de un arreglo
void OperacionesArreglos::PromedioOpenMP(
    int* A, double* R, int n)
{
    if (A == nullptr || R == nullptr)
    {
        throw std::invalid_argument(
            "Error: el arreglo o resultado es nulo.");
    }

    if (n <= 0)
    {
        throw std::invalid_argument(
            "Error: el tamano del arreglo debe ser mayor a cero.");
    }

    long long suma = 0;

    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < n; i++)
    {
        suma += A[i];
    }

    R[0] = static_cast<double>(suma) / n;
}

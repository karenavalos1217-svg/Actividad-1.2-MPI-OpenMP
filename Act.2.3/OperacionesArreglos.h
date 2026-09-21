/*
    Integrantes:
    Avalos Oliva Karen Lizeth
    Chavez Torres Oliver Daniel
    Ramirez Arvizu Fernando David
*/

#ifndef OPERACIONESARREGLOS_H
#define OPERACIONESARREGLOS_H

class OperacionesArreglos
{
public:

    void sumarArreglosOpenMP(
        int* A, int* B, int* R, int n);

    void restarArreglosOpenMP(
        int* A, int* B, int* R, int n);

    void multiplicarArreglosOpenMP(
        int* A, int* B, int* R, int n);

    void cuadradoArregloOpenMP(
        int* A, int* R, int n);

    void SumatoriaOpenMP(
        int* A, long long* R, int n);

    void MaximoOpenMP(
        int* A, int* R, int n);

    void MinimoOpenMP(
        int* A, int* R, int n);

    void PromedioOpenMP(
        int* A, double* R, int n);
};

#endif

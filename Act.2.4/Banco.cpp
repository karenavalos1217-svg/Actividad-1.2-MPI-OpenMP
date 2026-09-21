/*
 * 1. Avalos Oliva, Karen Lizeth
 * 2. Chavez Torres, Oliver Daniel
 * 3. Ramirez Arvizu, Fernando David
 */

#include "Banco.h"
#include <iostream>

using namespace std;

// CONSTRUCTOR

Banco::Banco(int saldoInicial) {
    saldo = saldoInicial;
    omp_init_lock(&candado);
}

// DESTRUCTOR

Banco::~Banco() {
    omp_destroy_lock(&candado);
}

// METODOS BASE

void Banco::depositar(int cantidad) {
    saldo += cantidad;
}

void Banco::retirar(int cantidad) {
    if (saldo >= cantidad) {
        saldo -= cantidad;
    }
}

int Banco::consultarSaldo() {
    return saldo;
}

// MECANISMO 1: CRITICAL
//
// #pragma omp critical garantiza que solamente un hilo
// ejecute este bloque a la vez.

void Banco::depositarCritical(int cantidad, int idHilo) {

    #pragma omp critical
    {
        saldo += cantidad;

        // Protege solamente la salida de consola
        #pragma omp critical(salida)
        {
            cout << "  [Hilo " << idHilo << "] "
                 << "DEPOSITO | Critical | +$" << cantidad
                 << " | Saldo: $" << saldo << endl;
        }
    }
}

void Banco::retirarCritical(int cantidad, int idHilo) {

    #pragma omp critical
    {
        if (saldo >= cantidad) {

            saldo -= cantidad;

            #pragma omp critical(salida)
            {
                cout << "  [Hilo " << idHilo << "] "
                     << "RETIRO   | Critical | -$" << cantidad
                     << " | Saldo: $" << saldo << endl;
            }

        } else {

            #pragma omp critical(salida)
            {
                cout << "  [Hilo " << idHilo << "] "
                     << "RETIRO   | Critical | RECHAZADO"
                     << " | Fondos insuficientes"
                     << " | Saldo: $" << saldo << endl;
            }
        }
    }
}

int Banco::consultarSaldoCritical(int idHilo) {

    int saldoTemp;

    #pragma omp critical
    {
        saldoTemp = saldo;

        #pragma omp critical(salida)
        {
            cout << "  [Hilo " << idHilo << "] "
                 << "CONSULTA | Critical"
                 << " | Saldo: $" << saldoTemp << endl;
        }
    }

    return saldoTemp;
}

// MECANISMO 2: ATOMIC
// Atomic protege operaciones simples sobre una variable.
// Es más específico que critical y normalmente tiene menor
// sobrecarga para actualizaciones sencillas.
//
// Para el retiro se utiliza critical porque la comprobacion
// de fondos y la resta deben realizarse como una sola
// operacion para evitar condiciones de carrera.

void Banco::depositarAtomic(int cantidad, int idHilo) {

    #pragma omp atomic update
    saldo += cantidad;

    int saldoActual;

    #pragma omp atomic read
    saldoActual = saldo;

    #pragma omp critical(salida)
    {
        cout << "  [Hilo " << idHilo << "] "
             << "DEPOSITO | Atomic  | +$" << cantidad
             << " | Saldo: $" << saldoActual << endl;
    }
}

void Banco::retirarAtomic(int cantidad, int idHilo) {

    bool retiroRealizado = false;
    int saldoActual;

    /*
     * La comprobacion y la resta deben realizarse juntas.
     * Atomic por si solo no permite expresar de forma segura
     * la condicion "si hay fondos, entonces restar".
     */

    #pragma omp critical
    {
        if (saldo >= cantidad) {
            saldo -= cantidad;
            retiroRealizado = true;
        }

        saldoActual = saldo;
    }

    #pragma omp critical(salida)
    {
        if (retiroRealizado) {

            cout << "  [Hilo " << idHilo << "] "
                 << "RETIRO   | Atomic  | -$" << cantidad
                 << " | Saldo: $" << saldoActual << endl;

        } else {

            cout << "  [Hilo " << idHilo << "] "
                 << "RETIRO   | Atomic  | RECHAZADO"
                 << " | Fondos insuficientes"
                 << " | Saldo: $" << saldoActual << endl;
        }
    }
}

int Banco::consultarSaldoAtomic(int idHilo) {

    int saldoTemp;

    #pragma omp atomic read
    saldoTemp = saldo;

    #pragma omp critical(salida)
    {
        cout << "  [Hilo " << idHilo << "] "
             << "CONSULTA | Atomic"
             << " | Saldo: $" << saldoTemp << endl;
    }

    return saldoTemp;
}

// MECANISMO 3: LOCK
// omp_set_lock bloquea el acceso.
// omp_unset_lock libera el acceso.
// Solamente un hilo puede trabajar con el saldo mientras
// el candado se encuentra ocupado.

void Banco::depositarLock(int cantidad, int idHilo) {

    omp_set_lock(&candado);

    saldo += cantidad;

    #pragma omp critical(salida)
    {
        cout << "  [Hilo " << idHilo << "] "
             << "DEPOSITO | Lock    | +$" << cantidad
             << " | Saldo: $" << saldo << endl;
    }

    omp_unset_lock(&candado);
}

void Banco::retirarLock(int cantidad, int idHilo) {

    omp_set_lock(&candado);

    if (saldo >= cantidad) {

        saldo -= cantidad;

        #pragma omp critical(salida)
        {
            cout << "  [Hilo " << idHilo << "] "
                 << "RETIRO   | Lock    | -$" << cantidad
                 << " | Saldo: $" << saldo << endl;
        }

    } else {

        #pragma omp critical(salida)
        {
            cout << "  [Hilo " << idHilo << "] "
                 << "RETIRO   | Lock    | RECHAZADO"
                 << " | Fondos insuficientes"
                 << " | Saldo: $" << saldo << endl;
        }
    }

    omp_unset_lock(&candado);
}

int Banco::consultarSaldoLock(int idHilo) {

    omp_set_lock(&candado);

    int saldoTemp = saldo;

    #pragma omp critical(salida)
    {
        cout << "  [Hilo " << idHilo << "] "
             << "CONSULTA | Lock"
             << " | Saldo: $" << saldoTemp << endl;
    }

    omp_unset_lock(&candado);

    return saldoTemp;
}


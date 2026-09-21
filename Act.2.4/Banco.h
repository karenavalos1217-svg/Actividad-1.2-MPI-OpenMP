/*
 * 1. Avalos Oliva, Karen Lizeth
 * 2. Chavez Torres, Oliver Daniel
 * 3. Ramirez Arvizu, Fernando David
 */

#ifndef BANCO_H
#define BANCO_H

#include <omp.h>

class Banco {
private:
    int saldo;              // Saldo actual de la cuenta bancaria
    omp_lock_t candado;     // Lock de OpenMP

public:
    // Constructor y destructor
    Banco(int saldoInicial = 0);
    ~Banco();

    // Métodos base solicitados
    void depositar(int cantidad);
    void retirar(int cantidad);
    int consultarSaldo();

    // VERSION 1: SECCION CRITICA
    void depositarCritical(int cantidad, int idHilo);
    void retirarCritical(int cantidad, int idHilo);
    int consultarSaldoCritical(int idHilo);

    // VERSION 2: ATOMIC
    void depositarAtomic(int cantidad, int idHilo);
    void retirarAtomic(int cantidad, int idHilo);
    int consultarSaldoAtomic(int idHilo);

    // VERSION 3: LOCK
    void depositarLock(int cantidad, int idHilo);
    void retirarLock(int cantidad, int idHilo);
    int consultarSaldoLock(int idHilo);
};

#endif // BANCO_H

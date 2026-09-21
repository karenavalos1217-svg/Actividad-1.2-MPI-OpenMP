/*
    Avalos Oliva Karen Lizeth
    Chavez Torres Oliver Daniel
    Ramirez Arvizu Fernando David
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <omp.h>
#include "Banco.h"

using namespace std;

// Funcion para ejecutar la simulacion
void ejecutarSimulacion(int opcionSincronizacion, int saldoInicial)
{
    Banco miBanco(saldoInicial);

    const int NUM_HILOS = 4;
    const int OPERACIONES_POR_HILO = 5;

    omp_set_num_threads(NUM_HILOS);

    cout << "\n--- EJECUTANDO CON ";

    switch (opcionSincronizacion)
    {
        case 1:
            cout << "CRITICAL";
            break;
        case 2:
            cout << "ATOMIC";
            break;
        case 3:
            cout << "LOCKS";
            break;
    }

    cout << " ---\n";

    cout << "     MECANISMO: ";

    switch (opcionSincronizacion)
    {
        case 1:
            cout << "CRITICAL";
            break;
        case 2:
            cout << "ATOMIC";
            break;
        case 3:
            cout << "LOCKS";
            break;
    }

    cout << endl;
    cout << "     Saldo inicial: $" << saldoInicial << endl;
    cout << "     Hilos configurados: " << NUM_HILOS << endl;
    cout << "     Operaciones por hilo: " << OPERACIONES_POR_HILO << endl;

    double inicioTiempo = omp_get_wtime();

    #pragma omp parallel
    {
        int idHilo = omp_get_thread_num();

        // Generador de numeros aleatorios independiente para cada hilo
        random_device rd;
        mt19937 generador(rd() + idHilo);

        uniform_int_distribution<int> distribucionOperacion(0, 2);
        uniform_int_distribution<int> distribucionCantidad(1, 100);

        for (int i = 0; i < OPERACIONES_POR_HILO; i++)
        {
            int op = distribucionOperacion(generador);
            int cantidad = distribucionCantidad(generador) * 10;

            // Mostrar que operacion va a realizar el hilo
            #pragma omp critical(salida)
            {
                cout << "  [Hilo " << idHilo << "] Operacion "
                     << i + 1 << "/" << OPERACIONES_POR_HILO << " -> ";

                if (op == 0)
                    cout << "Deposito" << endl;
                else if (op == 1)
                    cout << "Retiro" << endl;
                else
                    cout << "Consulta" << endl;
            }

            // Ejecutar la operacion seleccionada
            switch (opcionSincronizacion)
            {
                case 1: // CRITICAL
                    if (op == 0)
                        miBanco.depositarCritical(cantidad, idHilo);
                    else if (op == 1)
                        miBanco.retirarCritical(cantidad, idHilo);
                    else
                        miBanco.consultarSaldoCritical(idHilo);
                    break;

                case 2: // ATOMIC
                    if (op == 0)
                        miBanco.depositarAtomic(cantidad, idHilo);
                    else if (op == 1)
                        miBanco.retirarAtomic(cantidad, idHilo);
                    else
                        miBanco.consultarSaldoAtomic(idHilo);
                    break;

                case 3: // LOCKS
                    if (op == 0)
                        miBanco.depositarLock(cantidad, idHilo);
                    else if (op == 1)
                        miBanco.retirarLock(cantidad, idHilo);
                    else
                        miBanco.consultarSaldoLock(idHilo);
                    break;
            }
        }
    }

    double finTiempo = omp_get_wtime();

    cout << "              RESULTADOS FINALES" << endl;

    cout << "Saldo inicial:          $" << saldoInicial << endl;
    cout << "Saldo final:            $" << miBanco.consultarSaldo() << endl;
    cout << "Numero de hilos:        " << NUM_HILOS << endl;
    cout << "Operaciones por hilo:   " << OPERACIONES_POR_HILO << endl;
    cout << "Total de operaciones:   "
         << NUM_HILOS * OPERACIONES_POR_HILO << endl;
    cout << "Tiempo de ejecucion:    "
         << finTiempo - inicioTiempo << " segundos" << endl;
}

int main()
{

    cout << "INTEGRANTES DEL EQUIPO:" << endl;
    cout << "1. Avalos Oliva, Karen Lizeth" << endl;
    cout << "2. Chavez Torres, Oliver Daniel" << endl;
    cout << "3. Ramirez Arvizu, Fernando David" << endl;

    omp_set_num_threads(4);

    int opcion = 0;
    int saldoInicial = 1000;

    do
    {
        cout << "       SIMULADOR BANCARIO CON OPENMP" << endl;
        cout << "             ACTIVIDAD 2.4" << endl;

        cout << "Saldo inicial de cada prueba: $" << saldoInicial << endl;

        cout << "\nSeleccione el mecanismo de sincronizacion:" << endl;
        cout << "1. CRITICAL" << endl;
        cout << "   #pragma omp critical" << endl;
        cout << "2. ATOMIC" << endl;
        cout << "   #pragma omp atomic" << endl;
        cout << "3. LOCKS" << endl;
        cout << "   omp_lock_t" << endl;
        cout << "4. SALIR" << endl;

        cout << "\nSeleccione una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
            case 1:
                ejecutarSimulacion(1, saldoInicial);
                break;

            case 2:
                ejecutarSimulacion(2, saldoInicial);
                break;

            case 3:
                ejecutarSimulacion(3, saldoInicial);
                break;

            case 4:
                cout << "\nSaliendo del programa..." << endl;
                break;

            default:
                cout << "\nOpcion no valida. Intente nuevamente." << endl;
                break;
        }

    } while (opcion != 4);


    cout << "INTEGRANTES DEL EQUIPO:" << endl;
    cout << "1. Avalos Oliva, Karen Lizeth" << endl;
    cout << "2. Chavez Torres, Oliver Daniel" << endl;
    cout << "3. Ramirez Arvizu, Fernando David" << endl;

    return 0;
}

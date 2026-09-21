/*
    Integrantes:
    Avalos Oliva Karen Lizeth
    Chavez Torres Oliver Daniel
    Ramirez Arvizu Fernando David
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <omp.h>

#include "OperacionesArreglos.h"

using namespace std;

// Llena los arreglos con valores ascendentes
void llenarArreglosAscendentes(
    int* A, int* B, int n)
{
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        A[i] = i + 1;
        B[i] = n - i;
    }
}

// Llena los arreglos con valores aleatorios
void llenarArreglosAleatorios(
    int* A, int* B, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = 1 + rand() % 2000000;
        B[i] = 1 + rand() % 2000000;
    }
}

// Muestra un arreglo
void mostrarArreglo(
    int* arreglo, int n, char nombre)
{
    cout << "\nArreglo " << nombre << ":\n";

    for (int i = 0; i < n; i++)
    {
        cout << arreglo[i] << " ";

        if ((i + 1) % 10 == 0)
        {
            cout << endl;
        }
    }

    cout << endl;
}

// Ejecuta el menu
void ejecutarMenu(
    OperacionesArreglos& operaciones,
    int* A,
    int* B,
    int* R,
    int n,
    bool mostrar)
{
    int opcion;
    bool arreglosLlenos = false;

    do
    {
        cout << "\nMENU\n";
        cout << "1. Llenar arreglos\n";
        cout << "2. Sumar arreglos\n";
        cout << "3. Restar arreglos\n";
        cout << "4. Multiplicar arreglos\n";
        cout << "5. Cuadrado de un arreglo\n";
        cout << "6. Sumatoria de un Arreglo\n";
        cout << "7. Maximo de un arreglo\n";
        cout << "8. Minimo de un arreglo\n";
        cout << "9. Promedio de un arreglo\n";
        cout << "10. Salir\n";

        cout << "Seleccione una opcion: ";
        cin >> opcion;

        // Llenar arreglos
        if (opcion == 1)
        {
            if (mostrar)
            {
                llenarArreglosAscendentes(A, B, n);
            }
            else
            {
                llenarArreglosAleatorios(A, B, n);
            }

            arreglosLlenos = true;

            cout << "\nArreglos llenados correctamente.\n";

            if (mostrar)
            {
                mostrarArreglo(A, n, 'A');
                mostrarArreglo(B, n, 'B');
            }
        }

        // Sumar arreglos
        else if (opcion == 2)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                cout << "\nSUMA DE ARREGLOS\n";

                if (mostrar)
                {
                    mostrarArreglo(A, n, 'A');
                    mostrarArreglo(B, n, 'B');
                }

                double inicio = omp_get_wtime();

                operaciones.sumarArreglosOpenMP(
                    A, B, R, n);

                double fin = omp_get_wtime();

                if (mostrar)
                {
                    mostrarArreglo(R, n, 'R');
                }

                cout << "Tiempo de calculo en paralelo: "
                     << (fin - inicio)
                     << " segundos\n";
            }
        }

        // Restar arreglos
        else if (opcion == 3)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                cout << "\nRESTA DE ARREGLOS\n";

                if (mostrar)
                {
                    mostrarArreglo(A, n, 'A');
                    mostrarArreglo(B, n, 'B');
                }

                double inicio = omp_get_wtime();

                operaciones.restarArreglosOpenMP(
                    A, B, R, n);

                double fin = omp_get_wtime();

                if (mostrar)
                {
                    mostrarArreglo(R, n, 'R');
                }

                cout << "Tiempo de calculo en paralelo: "
                     << (fin - inicio)
                     << " segundos\n";
            }
        }

        // Multiplicar arreglos
        else if (opcion == 4)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                cout << "\nMULTIPLICACION DE ARREGLOS\n";

                if (mostrar)
                {
                    mostrarArreglo(A, n, 'A');
                    mostrarArreglo(B, n, 'B');
                }

                double inicio = omp_get_wtime();

                operaciones.multiplicarArreglosOpenMP(
                    A, B, R, n);

                double fin = omp_get_wtime();

                if (mostrar)
                {
                    mostrarArreglo(R, n, 'R');
                }

                cout << "Tiempo de calculo en paralelo: "
                     << (fin - inicio)
                     << " segundos\n";
            }
        }

        // Cuadrado de un arreglo
        else if (opcion == 5)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                cout << "\nCUADRADO DE UN ARREGLO\n";

                if (mostrar)
                {
                    mostrarArreglo(A, n, 'A');
                }

                double inicio = omp_get_wtime();

                operaciones.cuadradoArregloOpenMP(
                    A, R, n);

                double fin = omp_get_wtime();

                if (mostrar)
                {
                    mostrarArreglo(R, n, 'R');
                }

                cout << "Tiempo de calculo en paralelo: "
                     << (fin - inicio)
                     << " segundos\n";
            }
        }

        // Sumatoria
        else if (opcion == 6)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                try
                {
                    cout << "\nSUMATORIA DE UN ARREGLO\n";

                    if (mostrar)
                    {
                        mostrarArreglo(A, n, 'A');
                    }

                    long long resultado[1];

                    double inicio = omp_get_wtime();

                    operaciones.SumatoriaOpenMP(
                        A, resultado, n);

                    double fin = omp_get_wtime();

                    cout << "\nResultado de la sumatoria: "
                         << resultado[0] << endl;

                    cout << "Tiempo de calculo en paralelo: "
                         << (fin - inicio)
                         << " segundos\n";
                }
                catch (const exception& e)
                {
                    cout << "\nExcepcion: "
                         << e.what() << endl;
                }
            }
        }

        // Maximo
        else if (opcion == 7)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                try
                {
                    cout << "\nMAXIMO DE UN ARREGLO\n";

                    if (mostrar)
                    {
                        mostrarArreglo(A, n, 'A');
                    }

                    int resultado[1];

                    double inicio = omp_get_wtime();

                    operaciones.MaximoOpenMP(
                        A, resultado, n);

                    double fin = omp_get_wtime();

                    cout << "\nResultado maximo: "
                         << resultado[0] << endl;

                    cout << "Tiempo de calculo en paralelo: "
                         << (fin - inicio)
                         << " segundos\n";
                }
                catch (const exception& e)
                {
                    cout << "\nExcepcion: "
                         << e.what() << endl;
                }
            }
        }

        // Minimo
        else if (opcion == 8)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                try
                {
                    cout << "\nMINIMO DE UN ARREGLO\n";

                    if (mostrar)
                    {
                        mostrarArreglo(A, n, 'A');
                    }

                    int resultado[1];

                    double inicio = omp_get_wtime();

                    operaciones.MinimoOpenMP(
                        A, resultado, n);

                    double fin = omp_get_wtime();

                    cout << "\nResultado minimo: "
                         << resultado[0] << endl;

                    cout << "Tiempo de calculo en paralelo: "
                         << (fin - inicio)
                         << " segundos\n";
                }
                catch (const exception& e)
                {
                    cout << "\nExcepcion: "
                         << e.what() << endl;
                }
            }
        }

        // Promedio
        else if (opcion == 9)
        {
            if (!arreglosLlenos)
            {
                cout << "\nPrimero debe llenar los arreglos.\n";
            }
            else
            {
                try
                {
                    cout << "\nPROMEDIO DE UN ARREGLO\n";

                    if (mostrar)
                    {
                        mostrarArreglo(A, n, 'A');
                    }

                    double resultado[1];

                    double inicio = omp_get_wtime();

                    operaciones.PromedioOpenMP(
                        A, resultado, n);

                    double fin = omp_get_wtime();

                    cout << "\nResultado del promedio: "
                         << resultado[0] << endl;

                    cout << "Tiempo de calculo en paralelo: "
                         << (fin - inicio)
                         << " segundos\n";
                }
                catch (const exception& e)
                {
                    cout << "\nExcepcion: "
                         << e.what() << endl;
                }
            }
        }

        // Salir
        else if (opcion == 10)
        {
            cout << "\nSaliendo de esta ejecucion...\n";
        }

        // Opcion incorrecta
        else
        {
            cout << "\nOpcion no valida.\n";
        }

    } while (opcion != 10);
}

int main()
{
    cout << "Avalos Oliva Karen Lizeth\n"
         << "Chavez Torres Oliver Daniel\n"
         << "Ramirez Arvizu Fernando David\n"
         << endl;

    srand(static_cast<unsigned int>(time(NULL)));

    OperacionesArreglos operaciones;

    // Primera ejecucion
    cout << "Tamano del arreglo: 100 elementos\n";

    int n = 100;

    try
    {
        int* A = new int[n];
        int* B = new int[n];
        int* R = new int[n];

        ejecutarMenu(
            operaciones,
            A,
            B,
            R,
            n,
            true);

        delete[] A;
        delete[] B;
        delete[] R;
    }
    catch (const bad_alloc&)
    {
        cout << "Error al reservar memoria.\n";
        return 1;
    }

    // Segunda ejecucion
    cout << "Tamano del arreglo: 10000000 elementos\n";
    cout << "Valores aleatorios de 1 a 2000000\n";

    n = 10000000;

    try
    {
        int* A = new int[n];
        int* B = new int[n];
        int* R = new int[n];

        ejecutarMenu(
            operaciones,
            A,
            B,
            R,
            n,
            false);

        delete[] A;
        delete[] B;
        delete[] R;
    }
    catch (const bad_alloc&)
    {
        cout << "Error al reservar memoria.\n";
        return 1;
    }

    cout << "\nAvalos Oliva Karen Lizeth\n"
         << "Chavez Torres Oliver Daniel\n"
         << "Ramirez Arvizu Fernando David\n"
         << endl;

    return 0;
}

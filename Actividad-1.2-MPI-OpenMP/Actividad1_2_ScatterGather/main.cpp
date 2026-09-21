#include <mpi.h>
#include <omp.h>

#include <iostream>
#include <string>
#include <iomanip>

#include "OperacionesArreglos.h"

using namespace std;


void mostrarIntegrantes() {
    cout << "Integrantes:" << endl;
    cout << "Avalos Oliva Karen Lizeth" << endl;
    cout << "Chavez Torres Oliver Daniel" << endl;
    cout << "Ramirez Arvizu Fernando David" << endl;
}


void mostrarArreglo(
    const string& nombre,
    int* arreglo,
    int cantidad
) {
    cout << nombre << ": [";

    for (int i = 0; i < cantidad; i++) {
        cout << arreglo[i];

        if (i < cantidad - 1) {
            cout << ", ";
        }
    }

    cout << "]" << endl;
}


void mostrarArregloLong(
    const string& nombre,
    long long* arreglo,
    int cantidad
) {
    cout << nombre << ": [";

    for (int i = 0; i < cantidad; i++) {
        cout << arreglo[i];

        if (i < cantidad - 1) {
            cout << ", ";
        }
    }

    cout << "]" << endl;
}


int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int proceso;
    int totalProcesos;

    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesos);

    char nombreEquipo[MPI_MAX_PROCESSOR_NAME];
    int longitudNombre;

    MPI_Get_processor_name(
        nombreEquipo,
        &longitudNombre
    );

    string equipo(nombreEquipo);

    if (proceso == 0) {
        mostrarIntegrantes();

        cout << endl;
        cout << "Actividad 1.2 - MPI y OpenMP" << endl;
        cout << "Version: MPI_Scatter y MPI_Gather" << endl;
    }

    if (totalProcesos != 5) {

        if (proceso == 0) {
            cout << "Ejecuta el programa con 5 procesos MPI." << endl;
        }

        MPI_Finalize();
        return 0;
    }

    // Muestra donde esta ejecutandose cada proceso.
    for (int p = 0; p < totalProcesos; p++) {

        MPI_Barrier(MPI_COMM_WORLD);

        if (proceso == p) {
            cout << "Equipo: " << equipo
                 << " | Proceso MPI: " << proceso;

            if (proceso == 0) {
                cout << " | Maestro";
            } else {
                cout << " | Trabajador";
            }

            cout << endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int opcion = -1;
    int tamano = 0;

    int* A = nullptr;
    int* B = nullptr;

    bool arreglosCreados = false;

    do {

        if (proceso == 0) {
            cout << endl;
            cout << "1. Crear arreglos" << endl;
            cout << "2. Sumar arreglos" << endl;
            cout << "3. Restar arreglos" << endl;
            cout << "4. Multiplicar arreglos" << endl;
            cout << "5. Calcular cuadrado de A" << endl;
            cout << "0. Salir" << endl;
            cout << "Opcion: ";

            cin >> opcion;
        }

        // Comparte la opcion del menu.
        MPI_Bcast(
            &opcion,
            1,
            MPI_INT,
            0,
            MPI_COMM_WORLD
        );


        if (opcion == 1) {

            int tipoPrueba = 0;

            if (proceso == 0) {

                cout << endl;
                cout << "1. Prueba detallada de 40 elementos" << endl;
                cout << "2. Prueba de 4,000,000 de elementos" << endl;
                cout << "Selecciona la prueba: ";

                cin >> tipoPrueba;

                if (tipoPrueba == 1) {
                    tamano = 40;
                } else {
                    tamano = 4000000;
                }

                delete[] A;
                delete[] B;

                A = new int[tamano];
                B = new int[tamano];

                // El maestro prepara los datos globales.
                // Despues Scatter se encargara de repartirlos.
                if (tamano == 40) {

                    for (int i = 0; i < tamano; i++) {
                        A[i] = i + 1;
                        B[i] = tamano - i;
                    }

                } else {

                    // Para la prueba grande se generan valores
                    // pseudoaleatorios entre 1 y 1,000,000.
                    #pragma omp parallel for
                    for (int i = 0; i < tamano; i++) {

// Se crean semillas diferentes para cada posición.
// Así podemos generar valores pseudoaleatorios sin usar rand_r,
// ya que rand_r no está disponible en Windows.
unsigned int semillaA =
    1000u + (i * 37u);

unsigned int semillaB =
    5000u + (i * 53u);

// Se aplica una operación matemática sencilla para modificar
// las semillas y obtener valores diferentes.
semillaA = semillaA * 1103515245u + 12345u;
semillaB = semillaB * 1103515245u + 12345u;

// Los valores quedan dentro del rango de 1 a 1,000,000.
A[i] = 1 + (semillaA % 1000000u);
B[i] = 1 + (semillaB % 1000000u);
                    }
                }
            }

            MPI_Bcast(
                &tipoPrueba,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            MPI_Bcast(
                &tamano,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            bool detalle = (tamano == 40);

            // En esta implementacion incluimos al maestro en Scatter/Gather.
            // De esta forma los 5 procesos reciben una seccion.
            int cantidadLocal = tamano / totalProcesos;

            int* localA = new int[cantidadLocal];
            int* localB = new int[cantidadLocal];

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            // Scatter divide A entre todos los procesos.
            MPI_Scatter(
                A,
                cantidadLocal,
                MPI_INT,
                localA,
                cantidadLocal,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            // Scatter divide B entre todos los procesos.
            MPI_Scatter(
                B,
                cantidadLocal,
                MPI_INT,
                localB,
                cantidadLocal,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            // Mostramos qué proceso e hilo recibio/proceso cada posicion
            // durante la prueba pequena.
            if (detalle) {

                int posicionInicial =
                    proceso * cantidadLocal;

                #pragma omp parallel for
                for (int i = 0; i < cantidadLocal; i++) {

                    int hilo = omp_get_thread_num();

                    #pragma omp critical
                    {
                        cout << "Equipo: " << equipo
                             << " | Proceso MPI: " << proceso
                             << " | Hilo OpenMP: " << hilo
                             << " | Posicion: "
                             << posicionInicial + i
                             << " | A: " << localA[i]
                             << " | B: " << localB[i]
                             << endl;
                    }
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double fin = MPI_Wtime();

            if (proceso == 0) {

                arreglosCreados = true;

                if (detalle) {
                    cout << endl;
                    mostrarArreglo("A", A, tamano);
                    mostrarArreglo("B", B, tamano);
                }

                cout << fixed << setprecision(6);
                cout << "Tiempo de distribucion: "
                     << fin - inicio
                     << " segundos" << endl;
            }

            delete[] localA;
            delete[] localB;
        }


        else if (opcion >= 2 && opcion <= 5) {

            int existen =
                arreglosCreados ? 1 : 0;

            MPI_Bcast(
                &existen,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            if (!existen) {

                if (proceso == 0) {
                    cout << "Primero debes crear los arreglos." << endl;
                }

                continue;
            }

            MPI_Bcast(
                &tamano,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            int cantidadLocal =
                tamano / totalProcesos;

            bool detalle =
                (tamano == 40);

            int* localA =
                new int[cantidadLocal];

            int* localB =
                new int[cantidadLocal];

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            // Cada proceso recibe automaticamente una seccion de A.
            MPI_Scatter(
                A,
                cantidadLocal,
                MPI_INT,
                localA,
                cantidadLocal,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            // B solamente es necesario para suma, resta y multiplicacion.
            if (opcion != 5) {
                MPI_Scatter(
                    B,
                    cantidadLocal,
                    MPI_INT,
                    localB,
                    cantidadLocal,
                    MPI_INT,
                    0,
                    MPI_COMM_WORLD
                );
            }

            int posicionInicial =
                proceso * cantidadLocal;


            // SUMA O RESTA
            if (opcion == 2 || opcion == 3) {

                int* localResultado =
                    new int[cantidadLocal];

                if (opcion == 2) {

                    OperacionesArreglos::sumar(
                        localA,
                        localB,
                        localResultado,
                        cantidadLocal,
                        posicionInicial,
                        proceso,
                        equipo,
                        detalle
                    );

                } else {

                    OperacionesArreglos::restar(
                        localA,
                        localB,
                        localResultado,
                        cantidadLocal,
                        posicionInicial,
                        proceso,
                        equipo,
                        detalle
                    );
                }

                int* resultadoGlobal = nullptr;

                if (proceso == 0) {
                    resultadoGlobal =
                        new int[tamano];
                }

                // Gather toma el resultado local de cada proceso
                // y reconstruye el arreglo completo en el maestro.
                MPI_Gather(
                    localResultado,
                    cantidadLocal,
                    MPI_INT,
                    resultadoGlobal,
                    cantidadLocal,
                    MPI_INT,
                    0,
                    MPI_COMM_WORLD
                );

                MPI_Barrier(MPI_COMM_WORLD);

                double fin = MPI_Wtime();

                if (proceso == 0) {

                    cout << endl;

                    if (opcion == 2) {
                        cout << "Suma completada." << endl;
                    } else {
                        cout << "Resta completada." << endl;
                    }

                    if (detalle) {
                        mostrarArreglo(
                            "Resultado",
                            resultadoGlobal,
                            tamano
                        );
                    }

                    cout << fixed << setprecision(6);
                    cout << "Tiempo de operacion: "
                         << fin - inicio
                         << " segundos" << endl;

                    delete[] resultadoGlobal;
                }

                delete[] localResultado;
            }


            // MULTIPLICACION O CUADRADO
            else {

                long long* localResultado =
                    new long long[cantidadLocal];

                if (opcion == 4) {

                    OperacionesArreglos::multiplicar(
                        localA,
                        localB,
                        localResultado,
                        cantidadLocal,
                        posicionInicial,
                        proceso,
                        equipo,
                        detalle
                    );

                } else {

                    OperacionesArreglos::cuadrado(
                        localA,
                        localResultado,
                        cantidadLocal,
                        posicionInicial,
                        proceso,
                        equipo,
                        detalle
                    );
                }

                long long* resultadoGlobal = nullptr;

                if (proceso == 0) {
                    resultadoGlobal =
                        new long long[tamano];
                }

                MPI_Gather(
                    localResultado,
                    cantidadLocal,
                    MPI_LONG_LONG,
                    resultadoGlobal,
                    cantidadLocal,
                    MPI_LONG_LONG,
                    0,
                    MPI_COMM_WORLD
                );

                MPI_Barrier(MPI_COMM_WORLD);

                double fin = MPI_Wtime();

                if (proceso == 0) {

                    cout << endl;

                    if (opcion == 4) {
                        cout << "Multiplicacion completada." << endl;
                    } else {
                        cout << "Cuadrado completado." << endl;
                    }

                    if (detalle) {
                        mostrarArregloLong(
                            "Resultado",
                            resultadoGlobal,
                            tamano
                        );
                    }

                    cout << fixed << setprecision(6);
                    cout << "Tiempo de operacion: "
                         << fin - inicio
                         << " segundos" << endl;

                    delete[] resultadoGlobal;
                }

                delete[] localResultado;
            }

            delete[] localA;
            delete[] localB;
        }


        else if (opcion != 0) {

            if (proceso == 0) {
                cout << "Opcion no valida." << endl;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

    } while (opcion != 0);


    if (proceso == 0) {

        delete[] A;
        delete[] B;

        cout << endl;
        cout << "Programa finalizado." << endl;

        mostrarIntegrantes();
    }

    MPI_Finalize();

    return 0;
}

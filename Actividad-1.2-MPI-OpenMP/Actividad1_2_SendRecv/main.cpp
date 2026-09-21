#include <mpi.h>
#include <omp.h>

#include <iostream>
#include <string>
#include <iomanip>

#include "OperacionesArreglos.h"

using namespace std;

// Etiquetas utilizadas para distinguir los mensajes enviados por MPI.
const int TAG_A = 10;
const int TAG_B = 20;
const int TAG_RESULTADO_INT = 30;
const int TAG_RESULTADO_LONG = 40;


// Imprime los integrantes al inicio y al final.
// Solo lo hace el proceso maestro para evitar repetirlos 5 veces.
void mostrarIntegrantes() {
    cout << "Integrantes:" << endl;
    cout << "Avalos Oliva Karen Lizeth" << endl;
    cout << "Chavez Torres Oliver Daniel" << endl;
    cout << "Ramirez Arvizu Fernando David" << endl;
}


// Muestra un arreglo solamente cuando estamos trabajando
// con la prueba pequeña de 40 elementos.
void mostrarArreglo(const string& nombre, int* arreglo, int cantidad) {
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

    // Inicializa MPI.
    MPI_Init(&argc, &argv);

    int proceso;
    int totalProcesos;

    // Obtiene el numero del proceso actual.
    MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

    // Obtiene cuantos procesos MPI existen.
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesos);

    // Obtiene el nombre de la computadora donde corre el proceso.
    char nombreEquipo[MPI_MAX_PROCESSOR_NAME];
    int longitudNombre;

    MPI_Get_processor_name(nombreEquipo, &longitudNombre);

    string equipo(nombreEquipo);

    if (proceso == 0) {
        mostrarIntegrantes();

        cout << endl;
        cout << "Actividad 1.2 - MPI y OpenMP" << endl;
        cout << "Version: MPI_Send y MPI_Recv" << endl;
        cout << endl;
    }

    // La ejecucion local solicitada utiliza exactamente 5 procesos:
    // proceso 0 = maestro y procesos 1-4 = trabajadores.
    if (totalProcesos != 5) {

        if (proceso == 0) {
            cout << "Esta version debe ejecutarse con 5 procesos MPI." << endl;
            cout << "Ejemplo: mpiexec -n 5 programa.exe" << endl;
        }

        MPI_Finalize();
        return 0;
    }

    // Cada proceso informa en que computadora esta ejecutandose.
    // MPI_Barrier ayuda a que esta parte aparezca antes del menu.
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

    // Los arreglos globales solamente existen en el maestro.
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

        // El proceso 0 comparte la opcion seleccionada
        // con todos los procesos trabajadores.
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
            }

            MPI_Bcast(&tipoPrueba, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&tamano, 1, MPI_INT, 0, MPI_COMM_WORLD);

            // Con 4 trabajadores, ambas pruebas se dividen exactamente.
            int cantidadLocal = tamano / 4;

            bool detalle = (tamano == 40);
            bool aleatorio = (tamano == 4000000);

            int* localA = new int[cantidadLocal];
            int* localB = new int[cantidadLocal];

            if (proceso == 0) {

                // Si ya existian arreglos anteriores, se liberan.
                delete[] A;
                delete[] B;

                A = new int[tamano];
                B = new int[tamano];

                cout << endl;
                cout << "Creando arreglos de "
                     << tamano << " elementos..." << endl;
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            if (proceso != 0) {

                // Cada trabajador calcula donde empieza su seccion
                // dentro del arreglo global.
                int posicionInicial =
                    (proceso - 1) * cantidadLocal;

                // Cada trabajador crea su propia seccion utilizando OpenMP.
                OperacionesArreglos::crearArregloMPI(
                    localA,
                    cantidadLocal,
                    posicionInicial,
                    proceso,
                    equipo,
                    aleatorio,
                    detalle
                );

                // Para B usamos otra creacion.
                // En la prueba pequena se ajustan los valores despues
                // para que A y B sean diferentes.
                OperacionesArreglos::crearArregloMPI(
                    localB,
                    cantidadLocal,
                    posicionInicial,
                    proceso,
                    equipo,
                    aleatorio,
                    false
                );

                if (!aleatorio) {
                    for (int i = 0; i < cantidadLocal; i++) {
                        localB[i] = tamano - (posicionInicial + i);
                    }
                }

                // El trabajador manda sus secciones al maestro.
                MPI_Send(
                    localA,
                    cantidadLocal,
                    MPI_INT,
                    0,
                    TAG_A,
                    MPI_COMM_WORLD
                );

                MPI_Send(
                    localB,
                    cantidadLocal,
                    MPI_INT,
                    0,
                    TAG_B,
                    MPI_COMM_WORLD
                );

            } else {

                // El maestro recibe las cuatro secciones.
                for (int trabajador = 1;
                     trabajador <= 4;
                     trabajador++) {

                    int desplazamiento =
                        (trabajador - 1) * cantidadLocal;

                    MPI_Recv(
                        A + desplazamiento,
                        cantidadLocal,
                        MPI_INT,
                        trabajador,
                        TAG_A,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                    );

                    MPI_Recv(
                        B + desplazamiento,
                        cantidadLocal,
                        MPI_INT,
                        trabajador,
                        TAG_B,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                    );
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
                cout << "Tiempo de creacion y recopilacion: "
                     << fin - inicio << " segundos" << endl;
            }

            delete[] localA;
            delete[] localB;
        }


        else if (opcion >= 2 && opcion <= 5) {

            // Todos los procesos necesitan saber si existen datos.
            int existen = arreglosCreados ? 1 : 0;

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

            // El tamano tambien se comparte con los trabajadores.
            MPI_Bcast(
                &tamano,
                1,
                MPI_INT,
                0,
                MPI_COMM_WORLD
            );

            int cantidadLocal = tamano / 4;
            bool detalle = (tamano == 40);

            int* localA = new int[cantidadLocal];
            int* localB = new int[cantidadLocal];

            // El maestro envia manualmente a cada trabajador
            // la seccion que le corresponde.
            if (proceso == 0) {

                for (int trabajador = 1;
                     trabajador <= 4;
                     trabajador++) {

                    int desplazamiento =
                        (trabajador - 1) * cantidadLocal;

                    MPI_Send(
                        A + desplazamiento,
                        cantidadLocal,
                        MPI_INT,
                        trabajador,
                        TAG_A,
                        MPI_COMM_WORLD
                    );

                    if (opcion != 5) {
                        MPI_Send(
                            B + desplazamiento,
                            cantidadLocal,
                            MPI_INT,
                            trabajador,
                            TAG_B,
                            MPI_COMM_WORLD
                        );
                    }
                }

            } else {

                MPI_Recv(
                    localA,
                    cantidadLocal,
                    MPI_INT,
                    0,
                    TAG_A,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE
                );

                if (opcion != 5) {
                    MPI_Recv(
                        localB,
                        cantidadLocal,
                        MPI_INT,
                        0,
                        TAG_B,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                    );
                }
            }

            MPI_Barrier(MPI_COMM_WORLD);

            double inicio = MPI_Wtime();

            // Suma y resta caben en int.
            if (opcion == 2 || opcion == 3) {

                int* localResultado =
                    new int[cantidadLocal];

                if (proceso != 0) {

                    int posicionInicial =
                        (proceso - 1) * cantidadLocal;

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

                    MPI_Send(
                        localResultado,
                        cantidadLocal,
                        MPI_INT,
                        0,
                        TAG_RESULTADO_INT,
                        MPI_COMM_WORLD
                    );
                }

                int* resultadoGlobal = nullptr;

                if (proceso == 0) {

                    resultadoGlobal = new int[tamano];

                    for (int trabajador = 1;
                         trabajador <= 4;
                         trabajador++) {

                        int desplazamiento =
                            (trabajador - 1) * cantidadLocal;

                        MPI_Recv(
                            resultadoGlobal + desplazamiento,
                            cantidadLocal,
                            MPI_INT,
                            trabajador,
                            TAG_RESULTADO_INT,
                            MPI_COMM_WORLD,
                            MPI_STATUS_IGNORE
                        );
                    }
                }

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


            // Multiplicacion y cuadrado usan long long.
            else {

                long long* localResultado =
                    new long long[cantidadLocal];

                if (proceso != 0) {

                    int posicionInicial =
                        (proceso - 1) * cantidadLocal;

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

                    MPI_Send(
                        localResultado,
                        cantidadLocal,
                        MPI_LONG_LONG,
                        0,
                        TAG_RESULTADO_LONG,
                        MPI_COMM_WORLD
                    );
                }

                long long* resultadoGlobal = nullptr;

                if (proceso == 0) {

                    resultadoGlobal =
                        new long long[tamano];

                    for (int trabajador = 1;
                         trabajador <= 4;
                         trabajador++) {

                        int desplazamiento =
                            (trabajador - 1) * cantidadLocal;

                        MPI_Recv(
                            resultadoGlobal + desplazamiento,
                            cantidadLocal,
                            MPI_LONG_LONG,
                            trabajador,
                            TAG_RESULTADO_LONG,
                            MPI_COMM_WORLD,
                            MPI_STATUS_IGNORE
                        );
                    }
                }

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

        // Sincroniza los procesos antes de regresar al menu.
        MPI_Barrier(MPI_COMM_WORLD);

    } while (opcion != 0);


    // Se libera la memoria reservada por el proceso maestro.
    if (proceso == 0) {
        delete[] A;
        delete[] B;

        cout << endl;
        cout << "Programa finalizado." << endl;
        mostrarIntegrantes();
    }

    // Finaliza correctamente MPI.
    MPI_Finalize();

    return 0;
}

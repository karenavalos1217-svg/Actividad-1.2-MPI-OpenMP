// Avalos Oliva Karen Lizeth
// Practica 2.0: Configuracion de ambiente de trabajo - MPI

#include <iostream>
#include <cstddef>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv); // Inicializacion de MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtener el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtener el tamano del comunicador

    // Impresion inicial del nombre (proceso principal)
    if (rank == 0) {
        std::cout << "INICIO DE EJECUCION: Avalos Oliva Karen Lizeth" << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Mensaje de cada proceso
    std::cout << "Hola Mundo desde el proceso " << rank << " de " << size << std::endl;

    MPI_Barrier(MPI_COMM_WORLD);

    // Impresion final del nombre
    if (rank == 0) {
        std::cout << "FIN DE EJECUCION: Avalos Oliva Karen Lizeth" << std::endl;
    }

    MPI_Finalize(); // Finalizacion de MPI

    return 0;
}

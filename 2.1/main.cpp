// Avalos Oliva Karen Lizeth
// Chavez Torres Oliver Daniel
// Ramirez Arvizu Fernando David

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100

int main()
{
    int *arreglo = new int[N];
    int procesados = 0;

    // Establecer el número de hilos
    omp_set_num_threads(4);

    #pragma omp parallel // indica el inicio de una region paralela
    {
        int hilo = omp_get_thread_num(); // para obtenerlo
        int total = omp_get_num_threads(); // para conocer el numero total de hilos

        printf("[Hilo %d/%d] iniciado\n", hilo, total);

        #pragma omp for //indicamos las iteraciones del cliclo for
        for (int i = 0; i < N; i++)
        {
            arreglo[i] = rand() % 1000; // para generar valores aleatorios

            int actual;

            #pragma omp atomic capture // realiza de manera segura el incremento de esta variable cuando varios hilos intentan modificarla
            actual = ++procesados;

            float avance = ((float)actual / N) * 100.0f; // se calcula el porcentaje

            printf("[Hilo %d] valor asignado: %d | posicion: %d | Avance: %.1f%%\n",
                   hilo, arreglo[i], i, avance);
        }

        #pragma omp barrier // hace qe los hilos esperen hasta que todos hayan legado a ese punto

        #pragma omp single // solo un hilo debe de ejecutar el bloque donde imprimimos el arreglo final
        {
            printf("\n ARREGLO FINAL \n");

            for (int i = 0; i < N; i++)
            {
                printf("arreglo[%d] = %d\n", i, arreglo[i]);
            }

        }

        printf("[Hilo %d/%d] finalizado\n", hilo, total);
    }

    delete[] arreglo;

    return 0;
}

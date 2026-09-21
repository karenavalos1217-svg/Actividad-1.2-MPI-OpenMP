#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        printf("Hola desde el hilo %d\n", omp_get_thread_num());
    }

    return 0;
}

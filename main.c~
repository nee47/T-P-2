#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_hash_catedra(void);
void pruebas_volumen_catedra(size_t);

int main(int argc, char *argv[])
{
  clock_t start = clock();
    if (argc > 1) {
        // Asumimos que nos están pidiendo pruebas de volumen.
        long largo = strtol(argv[1], NULL, 10);
        pruebas_volumen_catedra((size_t) largo);
        return 0;
    }

    printf("~~~ PRUEBAS CÁTEDRA ~~~\n");
    pruebas_hash_catedra();
    double tiempo = (clock() - start) * 1.00 / CLOCKS_PER_SEC ;
    printf("tiempo transcurrido : %f \n ", tiempo);
    return failure_count() > 0;
}

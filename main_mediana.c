#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_mediana_alumnos();

int main()
{
  clock_t start = clock();
  
  pruebas_mediana_alumnos();
  double tiempo = (clock() - start) * 1.00 / CLOCKS_PER_SEC ;
  printf("tiempo transcurrido : %f \n ", tiempo);
  return 0;
}

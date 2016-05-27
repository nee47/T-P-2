#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_abb_alumnos();

int main(int argc, char *argv[])
{
  clock_t start = clock();
  pruebas_abb_alumnos();
  double tiempo = (clock() - start) * 1.00 / CLOCKS_PER_SEC ;
  printf("tiempo transcurrido : %f \n ", tiempo);
  return 0;
}

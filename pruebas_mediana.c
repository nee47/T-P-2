#include <stdio.h>
#include <stdlib.h>
#include "mediana.h"

//void pruebas_bascas


int main(){
  mediana_t* mediana = calculador_mediana_crear();
  double v1 = 3 , v2 = 4, v3 = 10, v4 = 7;
  calculador_mediana_agregar_valor(mediana, v1);
  //v2 = calculador_mediana_obtener_mediana(mediana);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));
  calculador_mediana_agregar_valor(mediana, v2);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));
  
  calculador_mediana_agregar_valor(mediana, v3);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));

  calculador_mediana_agregar_valor(mediana, v4);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));

  calculador_mediana_agregar_valor(mediana, 50);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));

  calculador_mediana_agregar_valor(mediana, 13);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));

  calculador_mediana_agregar_valor(mediana, 9);
  printf("obtener media es %f\n", calculador_mediana_obtener_mediana(mediana));
  
  calculador_mediana_destruir(mediana);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "mediana.h"
#include "testing.h"

void prueba_mediana_vacia(){
  mediana_t* mediana = calculador_mediana_crear();
  print_test("calculador mediana creado", mediana);
  print_test("mediana obtener es 0", calculador_mediana_obtener_mediana(mediana) == 0);
  print_test("calculador mediana destruido", true);
  calculador_mediana_destruir(mediana);
}

void pruebas_mediana(){
  mediana_t* mediana = calculador_mediana_crear();
  print_test("mediana agregar 3", calculador_mediana_agregar_valor(mediana, 3));
  print_test("obtener mediana es 3", calculador_mediana_obtener_mediana(mediana)==3);
  print_test("mediana agregar 4", calculador_mediana_agregar_valor(mediana, 4));
  print_test("obtener mediana es 3.5", calculador_mediana_obtener_mediana(mediana) == 3.5f);
  
  print_test("mediana agregar 10", calculador_mediana_agregar_valor(mediana, 10));
  print_test("obtener mediana es 4 ", calculador_mediana_obtener_mediana(mediana) == 4);

  print_test("medaina agregar 7", calculador_mediana_agregar_valor(mediana, 7));
  print_test("obtener mediana es 5.5", calculador_mediana_obtener_mediana(mediana) == 5.5f);

  print_test("mediana agregar 50", calculador_mediana_agregar_valor(mediana, 50));
  print_test("obtener mediana es 7", calculador_mediana_obtener_mediana(mediana) == 7);

  print_test("mediana agregar 13", calculador_mediana_agregar_valor(mediana, 13));
  print_test("obtener media es 8.5", calculador_mediana_obtener_mediana(mediana) == 8.5f);

  print_test("mediana agregar 9", calculador_mediana_agregar_valor(mediana, 9));
  print_test("obtener media es 9", calculador_mediana_obtener_mediana(mediana) == 9);
  
  calculador_mediana_destruir(mediana);
}

void pruebas_mediana_alumnos(){
  prueba_mediana_vacia();
  pruebas_mediana();
}

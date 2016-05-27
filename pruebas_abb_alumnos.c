#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "testing.h"

int comparar(const char* cadena1, const char* cadena2){
  long int cad1, cad2;
  cad1 = strtol(cadena1, NULL, 10);
  cad2 = strtol(cadena2, NULL, 10);
  if(cad1 > cad2) return 1;
  if(cad1 < cad2) return -1;
  else return 0;
}

void primera_prueba(){
  abb_t* abb = abb_crear(comparar, NULL);
  int a = 1;
  abb_guardar(abb, "9", &a);
  abb_guardar(abb, "17", &a);
  abb_guardar(abb, "22", &a);
  abb_guardar(abb, "20", &a);
  abb_guardar(abb, "4", &a); 
  abb_guardar(abb, "3", &a);
  abb_guardar(abb, "6", &a);
  abb_guardar(abb, "5", &a);
  abb_guardar(abb, "7", &a);

  //abb_iter_t* iter = abb_iter_in_crear(abb);
  abb_iter_t* iter = abb_iter_crear_desde(abb, "3", "15");
  while(!abb_iter_in_al_final(iter)){
    printf("abb ver iteractual es %s\n", (char*)abb_iter_in_ver_actual(iter));
    //print_test("iter no esta al final", !abb_iter_in_al_final(iter));
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  
  iter = abb_iter_crear_desde(abb, "10", "5");
  print_test("iter al final", abb_iter_in_al_final(iter));
  print_test("iter ver actual NULL", !abb_iter_in_ver_actual(iter));
  abb_iter_in_destruir(iter);
    
  abb_destruir(abb);
}


void pruebas_abb_alumnos(){
  primera_prueba();
  //prueba();
}

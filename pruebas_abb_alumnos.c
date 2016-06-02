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

bool guardar_nuevo(abb_t* orig, abb_t* dest, const char** vector, int base, int tope, size_t cantidad){
  bool ok = true;
  int medio = (tope+base)/2;
  if (medio == 0 || medio== (cantidad/2)-1 || medio >= cantidad-1) ok = false;  
  
  abb_guardar(dest, vector[medio], &cantidad);
  if (ok){
    ok = guardar_nuevo(orig, dest, vector, medio, tope, cantidad);  
    abb_guardar(dest, vector[(medio+base)/2], &cantidad); //guardo temporalmente X
    //tope = medio;
    //if()  ok = guardar_nuevo(orig, dest, vector, 0, medio, cantidad); 
    //abb_guardar(dest, vector[(medio+base)/2], &cantidad);
  }
  //return true;
  if(medio == cantidad/2 || !fin){  
    printf("oie kha\n");
    ok = guardar_nuevo(orig, dest, vector, 0, medio, cantidad); 
  }
  return true;
}

void abb_rebalanceado(abb_t* orig, abb_t* dest){
  abb_iter_t* iter = abb_iter_in_crear(orig);
  if(!iter){
    return;
  }
  size_t cantidad = abb_cantidad(orig);
  const char* vector[cantidad];
  
  for(size_t i = 0; i<cantidad ; i++){
    vector[i] = abb_iter_in_ver_actual(iter);
    abb_iter_in_avanzar(iter);
  }// O(nlogn)    
  
  guardar_nuevo(orig, dest, vector, 0, cantidad, cantidad); //O(nlogn) 
  printf("mi cantidad es %zu\n", abb_cantidad(dest));

  for(size_t i = 0; i<cantidad ; i++){    
    abb_guardar(dest, vector[i], abb_obtener(orig, vector[i]));
  } // O(nlogn)
  //  printf("mi cantidad es %zu\n", abb_cantidad(dest));
  abb_iter_in_destruir(iter);
}  
// 90 100 110 120 130 140 150 160 170 180 190 200 
// cantidad 12
//base tope  medio wardado 
//  0   12     6    150     
//  6   12     9    180
//  9   12     10   190
//  10  12     11   200
//  
//  9 + 10 /2 = 9
//  0   10     
//  
//
// 
//
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
  abb_guardar(abb, "32", &a);
  abb_guardar(abb, "34", &a);
  abb_guardar(abb, "50", &a);
  abb_guardar(abb, "60", &a);
  abb_guardar(abb, "70", &a);
  abb_guardar(abb, "80", &a);
  abb_guardar(abb, "85", &a);
  //abb_iter_t* iter = abb_iter_in_crear(abb);
  abb_iter_t* iter = abb_iter_crear_desde(abb, "3", "15");
  while(!abb_iter_in_al_final(iter)){
    printf("abb ver iteractual es %s\n", (char*)abb_iter_in_ver_actual(iter));
    //print_test("iter no esta al finl", !abb_iter_in_al_final(iter));
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  
  
  iter = abb_iter_crear_desde(abb, "10", "5");
  print_test("iter al final", abb_iter_in_al_final(iter));
  print_test("iter ver actual NULL", !abb_iter_in_ver_actual(iter));
  abb_iter_in_destruir(iter);
  
  abb_t* abb2 = abb_crear(comparar, NULL);
  
  abb_rebalanceado(abb, abb2);
  abb_iter_t* it = abb_iter_in_crear(abb2);
  
  while(!abb_iter_in_al_final(it)){
    printf("abb ver iteractual es %s\n", (char*)abb_iter_in_ver_actual(it));
    //print_test("iter no esta al finl", !abb_iter_in_al_final(iter));
    print_test("abb avanzar ", abb_iter_in_avanzar(it));
  }
  print_test("ESTE ARBOL ESTA BALANCEADO", balanceado(abb2));
  
  printf("cantidad de abb1 es %zu, canyidad de abb2 es %zu\n", abb_cantidad(abb), abb_cantidad(abb2));
  abb_destruir(abb2);
  abb_destruir(abb);
}

void prueba_balanceado_derecha(){
  int a = 3;
  abb_t* abb = abb_crear(comparar, NULL);
  abb_guardar(abb, "50", &a);
  abb_guardar(abb, "30", &a);
  abb_guardar(abb, "60", &a);
  abb_guardar(abb, "20", &a);
  
  print_test("arbol balanceado ", balanceado(abb));
  

  abb_destruir(abb);
  
  abb = abb_crear(comparar, NULL);
  abb_guardar(abb, "50", &a);
  abb_guardar(abb, "60", &a);
  abb_guardar(abb, "70", &a);
  abb_guardar(abb, "80", &a);
  abb_guardar(abb, "90", &a);
  abb_guardar(abb, "100", &a);
  abb_guardar(abb, "110", &a);
  abb_guardar(abb, "120", &a);
  abb_guardar(abb, "130", &a);
  abb_guardar(abb, "140", &a);
  abb_guardar(abb, "150", &a);
  //abb_guardar(abb, "160", &a); desde aca empieza a fallar
  //abb_guardar(abb, "170", &a);
  //abb_guardar(abb, "180", &a);

  abb_t* abb2 = abb_crear(comparar, NULL);
  
  abb_rebalanceado(abb, abb2);
  print_test("arbol1 no balanceado", !balanceado(abb));
  print_test("arbol2  balanceado", balanceado(abb2));
  
  abb_destruir(abb);
}

void prueba_balanceado_izquierda(){
  int a = 3;
  abb_t* abb = abb_crear(comparar, NULL);
  
  abb_guardar(abb, "200", &a);
  abb_guardar(abb, "190", &a);
  abb_guardar(abb, "180", &a);
  abb_guardar(abb, "170", &a);
  abb_guardar(abb, "160", &a);
  abb_guardar(abb, "150", &a);
  abb_guardar(abb, "140", &a);
  abb_guardar(abb, "130", &a);
  abb_guardar(abb, "120", &a);
  abb_guardar(abb, "110", &a);
  abb_guardar(abb, "100", &a);
  abb_guardar(abb, "90", &a); 
  abb_guardar(abb, "80", &a);// desde aca fallaba
  abb_guardar(abb, "70", &a);
  //aca extra
   abb_guardar(abb, "60", &a);
  abb_guardar(abb, "59", &a);
  abb_guardar(abb, "58", &a);
  abb_guardar(abb, "57", &a);
  abb_guardar(abb, "56", &a);
  abb_guardar(abb, "55", &a);
  abb_guardar(abb, "54", &a);
  abb_guardar(abb, "53", &a);
  abb_guardar(abb, "52", &a);
  abb_guardar(abb, "51", &a);
  abb_guardar(abb, "50", &a);
  abb_guardar(abb, "49", &a); 
  abb_guardar(abb, "48", &a);
  abb_guardar(abb, "47", &a);

  abb_t* abb2 = abb_crear(comparar, NULL);
  
  abb_rebalanceado(abb, abb2);
  print_test("arbol1 no balanceado", !balanceado(abb));
  print_test("arbol2  balanceado", balanceado(abb2));
  
  printf("cantidad abb %zu\n", abb_cantidad(abb2));
  
  abb_destruir(abb);
  abb_destruir(abb2);
}

void pruebas_abb_alumnos(){
  //  primera_prueba();
  //prueba_balanceado_derecha();
  prueba_balanceado_izquierda();
    
}

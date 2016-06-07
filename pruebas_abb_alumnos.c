#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "testing.h"
#include <string.h>

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
    abb_guardar(dest, vector[(medio+base)/2], &cantidad); 
  } 
  if(medio == cantidad/2 ){  
    ok = guardar_nuevo(orig, dest, vector, base, medio, cantidad); 
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
  }
  
  guardar_nuevo(orig, dest, vector, 0, cantidad, cantidad); 

  for(size_t i = 0; i<cantidad ; i++){    
    abb_guardar(dest, vector[i], abb_obtener(orig, vector[i]));
  }
  abb_iter_in_destruir(iter);
}  

void primera_prueba(){
  abb_t* abb = abb_crear(comparar, NULL);
  int vector[9] = {1,2,3,4,5,6,7,8,9};
  char* claves_esperadas[6] = {"4", "5", "6", "7", "9"};
  size_t i= 0;
  print_test("abb creado", abb);
  print_test("abb guardar 9",abb_guardar(abb, "9", &vector[0]));
  print_test("abb guardar 17", abb_guardar(abb, "17", &vector[1]));
  print_test("abb guardar 22", abb_guardar(abb, "22", &vector[2]));
  print_test("abb guardar 20", abb_guardar(abb, "20", &vector[3]));
  print_test("abb guardar 4", abb_guardar(abb, "4", &vector[4])); 
  print_test("abb guardar 3", abb_guardar(abb, "3", &vector[5]));
  print_test("abb guardar 4", abb_guardar(abb, "6", &vector[6]));
  print_test("abb guardar 5", abb_guardar(abb, "5", &vector[7]));
  print_test("abb guardar 7", abb_guardar(abb, "7", &vector[8]));
  abb_iter_t* iter = abb_iter_crear_desde(abb, "4", "9");
  printf("primera\n");
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  
  i = 0;
  printf("sgnda\n");
  char* claves_esperadas2[5] = {"7", "9", "17" , "20", "22"};
  iter = abb_iter_crear_desde(abb, "7", NULL);
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas2[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  
  i= 0;
  printf("trceraa\n");
  char* claves_esperadas3[5] = {"4", "5", "6", "7"};
  iter = abb_iter_crear_desde(abb, "4", "8");
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas3[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  
  abb_iter_in_destruir(iter);
  
  i = 0;
  printf("cuarto \n");
  char* claves_esperadas4[3] = {"17", "20", "22"}; 
  iter = abb_iter_crear_desde(abb, "13", "22");
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas4[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  
  iter = abb_iter_crear_desde(abb, "9", "4");
  print_test("iter esta al final", abb_iter_in_al_final(iter));
  abb_iter_in_destruir(iter);

  /* probando funcionalidad de la anterior primitiva */
  char* claves_esperadas5[9] = {"3", "4", "5", "6", "7", "9", "17", "20", "22"}; 
  iter = abb_iter_in_crear(abb);
  i = 0;
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas5[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
    
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
  //
  abb_guardar(abb, "80", &a);
  abb_guardar(abb, "70", &a);
  abb_guardar(abb, "60", &a);
  abb_guardar(abb, "40", &a);
  abb_guardar(abb, "32", &a);
  abb_guardar(abb, "30", &a);
  abb_guardar(abb, "29", &a);
  abb_guardar(abb, "28", &a);
  abb_guardar(abb, "27", &a);
  abb_guardar(abb, "26", &a);
  abb_guardar(abb, "25", &a);
  abb_guardar(abb, "24", &a); 

  abb_t* abb2 = abb_crear(comparar, NULL);
  
  abb_rebalanceado(abb, abb2);
  print_test("arbol1 no balanceado", !balanceado(abb));
  print_test("arbol2  balanceado", balanceado(abb2));
  
  abb_destruir(abb);
  abb_destruir(abb2);
}

void pruebas_abb_alumnos(){
  primera_prueba();
  prueba_balanceado_izquierda();   
}

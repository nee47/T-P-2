#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "testing.h"
#include <string.h>
#include <time.h>
#define VOL 10000

typedef struct pack{
  const char** claves;
  void** valores;
  size_t indice;
  bool* guardado;
}pack_t;

pack_t* pack_crear(size_t cantidad){
  pack_t* pack = malloc(sizeof(pack_t));
  if(!pack) return NULL;
  pack->claves = malloc(sizeof(char*) * cantidad);
  if(!pack->claves){
    free(pack);
    return NULL;
  }
  pack->valores = malloc(sizeof(void*) * cantidad);
  if(!pack->valores){
    free(pack->claves);
    free(pack);
    return NULL;
  }
  pack->guardado = malloc(sizeof(bool)*cantidad);
  pack->indice = 0;
  return pack;
}

bool obtener_par(const char* clave, void* dato, void* packi){
  pack_t* pack = packi;
  pack->claves[pack->indice] = clave;
  pack->valores[pack->indice] = dato;
  pack->guardado[pack->indice] = false;
  pack->indice++;
  return true;
}


char** crear_vector_cadenas(size_t largo){
  size_t a, r ;
  char cad[13];
  srand((unsigned int)time(NULL));
  char** vector = malloc(sizeof(char*) * largo);
  for( a = 0; a < largo; a++){
    r = /*a; */rand();
    snprintf(cad, sizeof(cad),"%zu", r );
    vector[a] = strdup(cad);
  }
  return vector;
}

void liberar_cadenas(char** vec, size_t largo){
  size_t a;
  for( a = 0; a < largo; a++){
    free(vec[a]);
  }
  free(vec);
}

int comparar(const char* cadena1, const char* cadena2){
  long int cad1, cad2;
  cad1 = strtol(cadena1, NULL, 10);
  cad2 = strtol(cadena2, NULL, 10);
  if(cad1 > cad2) return 1;
  if(cad1 < cad2) return -1;
  else return 0;
}
//23
bool guardar_nuevo(abb_t* orig, abb_t* dest, pack_t* pack, int base, int tope, size_t cantidad){
  int medio = (tope+base)/2;
  if (base>tope || base == medio){
    return false;
  }
  abb_guardar(dest, pack->claves[medio], pack->valores[medio]);
  pack->guardado[medio] = true;
  guardar_nuevo(orig, dest, pack, base,  medio, cantidad);
  guardar_nuevo(orig, dest, pack, medio+1, tope, cantidad);

  return false;
}

void destruir_pack(pack_t* pack){
  free(pack->valores);
  free(pack->claves);
  free(pack->guardado);
  free(pack);
}

void abb_rebalanceado(abb_t* orig, abb_t* dest){  
  size_t cantidad = abb_cantidad(orig);
  pack_t* pack = pack_crear(cantidad);
  abb_in_order(orig, obtener_par, pack);
  guardar_nuevo(orig, dest, pack, 0, cantidad, cantidad);
  for(size_t i = 0; i<cantidad ; i++){    
    if(!pack->guardado[i]) {
      abb_guardar(dest, pack->claves[i], pack->valores[i]);     
    }
  }
  destruir_pack(pack);
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
  int a = 3, x=23;
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
  abb_guardar(abb, "23", &x);
  abb_guardar(abb, "22", &a);
  abb_guardar(abb, "21", &a);
  //abb_guardar(abb, "20", &a);
  abb_guardar(abb, "19", &a);
  abb_guardar(abb, "18", &a);
  abb_guardar(abb, "17", &a);
  abb_guardar(abb, "16", &a);
  abb_guardar(abb, "15", &a);
  abb_guardar(abb, "14", &a);
  abb_guardar(abb, "13", &a);
  abb_guardar(abb, "12", &a); 
  
  abb_t* abb2 = abb_crear(comparar, NULL);
  // 24 25 26 27 28 29 30 32 40 60 70 
  
  
  abb_rebalanceado(abb, abb2);
  print_test("arbol1 no balanceado", !balanceado(abb));
  print_test("arbol2  balanceado", balanceado(abb2));
  print_test("obtener clave 23 es 23n", *(int*)abb_obtener(abb2, "23")== 23);
  abb_destruir(abb);
  abb_destruir(abb2);
}

void prueba_volumen(){
  bool ok;
  char** vec = crear_vector_cadenas(VOL);
  abb_t* abb = abb_crear(comparar, NULL);
  size_t a;
  print_test("ABB creado", abb);
  
  for( a = 0; a<VOL; a++){
    ok = abb_guardar(abb, vec[a], &a);
    if(!ok) break;
  }
  
  print_test("elemetnos guardados", ok);
  
  print_test("el abb esta desbalanceado", !balanceado(abb));
  
  abb_t* abb2 = abb_crear(comparar, NULL);
  
  abb_rebalanceado(abb, abb2);
  
  print_test("el abb2 esta balanceado", balanceado(abb2));
  
  abb_destruir(abb);
  abb_destruir(abb2);
  liberar_cadenas(vec, VOL);
}
// elementos   tiempo
//  100        0.00257
//  1000       0.164
//  10000      17.2

void pruebas_abb_alumnos(){
  primera_prueba();
  prueba_balanceado_izquierda();   
  prueba_volumen();
}

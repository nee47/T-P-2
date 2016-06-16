
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "testing.h"
#include <string.h>
#include <time.h>
#define VOL 5000

// Estructura que se va a utilizar para guardar los pares clave, dato del arbol
typedef struct pack{
  const char** claves;
  void** valores;
  size_t indice;
  bool* guardado;
}pack_t;

// Crea la estructura pack.
// Deulve NULL si hay algun fallo de malloc
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
// Funcion para extraer los datos del arbol, usando la primitiva in order
// Siempre devuelve true ya que se necesitan todos los elementos 
bool obtener_par(const char* clave, void* dato, void* packi){
  pack_t* pack = packi;
  pack->claves[pack->indice] = clave;
  pack->valores[pack->indice] = dato;
  pack->guardado[pack->indice] = false;
  pack->indice++;
  return true;
}


// Libera la memoria de todas las cadenas del vector
void liberar_cadenas(char** vec, size_t largo){
  size_t a;
  for( a = 0; a < largo; a++){
    free(vec[a]);
  }
  free(vec);
}
// Funcion comparar usada en el arbol
int comparar(const char* cadena1, const char* cadena2){
  long int cad1, cad2;
  cad1 = strtol(cadena1, NULL, 10);
  cad2 = strtol(cadena2, NULL, 10);
  if(cad1 > cad2) return 1;
  if(cad1 < cad2) return -1;
  else return 0;
}

// Vector que crea cadenas random a partir de numeros random
// Utilizable en las pruebas
// Puede haber claves repetidas
char** crear_vector_cadenas(size_t largo){
  size_t a, r ;
  char cad[13];
  srand((unsigned int)time(NULL));
  char** vector = malloc(sizeof(char*) * largo);
  if(!vector) return NULL;
  abb_t* abb= abb_crear(comparar, NULL);
  for( a = 0; a < largo; a++){
    r = /*a;*/ rand();
    snprintf(cad, sizeof(cad),"%zu", r );
    if(!abb_pertenece(abb, cad)){
      abb_guardar(abb, cad, &a);
      vector[a] = strdup(cad);
    }
    else a--;
  }
  abb_destruir(abb);
  return vector;
}

// Dado 2 vectores uno de origen y uno de destino. Se inserta 
// recursivamente todas las posiciones medio de un vector que tiene
// las claves obtenidas en orden del arbol de origen
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

// Libera la memoria pedida de la estructuara pack
void destruir_pack(pack_t* pack){
  free(pack->valores);
  free(pack->claves);
  free(pack->guardado);
  free(pack);
}

// Dado 2 arboles uno de origen, con o sin elementos y uno vacio
// Se inserta de tal manera de que el arbol de destino, quede
// balanceado con las misma claves y datos del arbol de origen
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
  printf("PRUEBA ABB ITER CREAR DESDE\n\n");
  abb_t* abb = abb_crear(comparar, NULL);
  int vector[9] = {1,2,3,4,5,6,7,8,9};
  char* claves_esperadas[6] = {"4", "5", "6", "7", "9"};
  size_t i= 0;
  printf("Primer test\n");
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
  print_test("ITER CREADO ABB ITER CREAR DESDE 4 - 9", iter);
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  print_test("ITER DESTRIODO", true);
  i = 0;


  printf("Segundo test\n");
  char* claves_esperadas2[5] = {"7", "9", "17" , "20", "22"};
  iter = abb_iter_crear_desde(abb, "7", NULL);
  print_test("ITER CREADO ABB ITER CREAR DESDE 7 - NULL", iter);
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas2[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  print_test("ITER DESTRUIDO ", true);
  i= 0;
  

  printf("Tercer test\n");
  char* claves_esperadas3[5] = {"4", "5", "6", "7"};
  iter = abb_iter_crear_desde(abb, "4", "8");
  print_test("ITER CREADO ABB ITER CREAR DESDE 4 - 8 ", iter);
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas3[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  
  abb_iter_in_destruir(iter);
  print_test("ITER DESTRUIDO", true);
  i = 0;


  printf("Cuarto test \n");
  char* claves_esperadas4[3] = {"17", "20", "22"}; 
  iter = abb_iter_crear_desde(abb, "13", "22");
  print_test("ITER CREADO ABB ITER CREAR DESDE 13 - 22", iter);
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas4[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  print_test("ITER DESTRUIDO", true);
  
  
  iter = abb_iter_crear_desde(abb, "9", "4");
  print_test("iter esta al final", abb_iter_in_al_final(iter));
  abb_iter_in_destruir(iter);

  /* probando funcionalidad de la anterior primitiva */
  printf("PRUEBA PARA VERIFICAR LA FUNCIONALIDAD DE ABB ITER IN CREAR\n");
  char* claves_esperadas5[9] = {"3", "4", "5", "6", "7", "9", "17", "20", "22"}; 
 
  iter = abb_iter_in_crear(abb);
  print_test("ITER CREADO ABB ITER IN CREAR", iter);
  i = 0;
  while(!abb_iter_in_al_final(iter)){
    print_test("abb ver iter actual correcta", strcmp((char*)abb_iter_in_ver_actual(iter), claves_esperadas5[i])== 0);    
    i++;
    print_test("abb avanzar ", abb_iter_in_avanzar(iter));
  }
  abb_iter_in_destruir(iter);
  print_test("ITER DESTRUIDO", true);
  abb_destruir(abb);
}

 
void prueba_balanceado(){
  printf("PRUEBA ABB REBALANCEADO \n\n");
  abb_t* abb = abb_crear(comparar, NULL);
  int vec[23] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
  print_test("abb guardar", abb_guardar(abb, "200", &vec[0]));
  print_test("abb guardar", abb_guardar(abb, "190", &vec[1]));
  print_test("abb guardar", abb_guardar(abb, "180", &vec[2]));
  print_test("abb guardar", abb_guardar(abb, "170", &vec[3]));
  print_test("abb guardar", abb_guardar(abb, "160", &vec[4]));
  print_test("abb guardar", abb_guardar(abb, "150", &vec[5]));
  print_test("abb guardar", abb_guardar(abb, "140", &vec[6]));
  print_test("abb guardar", abb_guardar(abb, "130", &vec[7]));
  print_test("abb guardar", abb_guardar(abb, "120", &vec[8]));
  print_test("abb guardar", abb_guardar(abb, "110", &vec[9]));
  print_test("abb guardar", abb_guardar(abb, "100", &vec[10]));
  print_test("abb guardar", abb_guardar(abb, "90", &vec[11])); 
  print_test("abb guardar", abb_guardar(abb, "80", &vec[12]));
  print_test("abb guardar", abb_guardar(abb, "70", &vec[13]));
  print_test("abb guardar", abb_guardar(abb, "60", &vec[14]));
  print_test("abb guardar", abb_guardar(abb, "40", &vec[15]));
  print_test("abb guardar", abb_guardar(abb, "32", &vec[16]));
  print_test("abb guardar", abb_guardar(abb, "30", &vec[17]));
  print_test("abb guardar", abb_guardar(abb, "29", &vec[18]));
  print_test("abb guardar", abb_guardar(abb, "28", &vec[19]));
  print_test("abb guardar", abb_guardar(abb, "27", &vec[20]));
  print_test("abb guardar", abb_guardar(abb, "26", &vec[21]));
  print_test("abb guardar", abb_guardar(abb, "25", &vec[22]));

  char* vec_claves[23] = {"200", "190", "180", "170", "160", "150", "140", "130", "120", "110", "100", "90", "80", "70", "60", "40", "32", "30", "29", "28", "27", "26", "25"};
  abb_t* abb2 = abb_crear(comparar, NULL);
  print_test("abb2 creado", abb2);
  print_test("abb2 cantidad es 0", abb_cantidad(abb2) == 0);
  abb_rebalanceado(abb, abb2);
  print_test("arbol1 no balanceado", !balanceado(abb));
  print_test("arbol2 despues de balancear esta  balanceado", balanceado(abb2));
  print_test("la cantidad de abb2 es la misma que abb", abb_cantidad(abb) == abb_cantidad(abb2));
  
  for(size_t i = 0 ; i < 23; i++){
    print_test(" clave pertenece abb pertenece a abb2 ", abb_pertenece(abb2, vec_claves[i]));
    print_test("obtener dato es el asociado a la clave", *(int*)abb_obtener(abb, vec_claves[i]) == vec[i]);

  }

  abb_destruir(abb);
  abb_destruir(abb2);
}

void prueba_volumen(){
  printf("PRUEBAS VOLUMEN \n\n");
  bool ok;
  char** vec = crear_vector_cadenas(VOL);
  abb_t* abb = abb_crear(comparar, NULL);
  size_t a;
  print_test("ABB creado", abb);
  size_t valores[VOL];
  for( a = 0; a<VOL; a++){
    valores[a] = a ;  
    ok = abb_guardar(abb, vec[a], &valores[a]);
    if(!ok) break;
  }
  printf("SE GUARDARON %zu elementos\n", abb_cantidad(abb));
  print_test("elementos guardados", ok);
  
  print_test("el abb esta desbalanceado", !balanceado(abb));
  
  abb_t* abb2 = abb_crear(comparar, NULL);
  
  abb_rebalanceado(abb, abb2);
  
  print_test("el abb2 esta balanceado", balanceado(abb2));
 
  for(a = 0; a < VOL; a++) {
    ok = abb_pertenece(abb2, vec[a]); 
    if (!ok) break;
  }

  print_test("Todos los elementos pertenecen", ok);

  for(a = 0; a < VOL; a++) {
    ok = *(size_t*)abb_borrar(abb2, vec[a]) == valores[a]; 
    if (!ok) break;
  }

  print_test("Todos los elementos fueron borrados exitosamente", ok);
  abb_destruir(abb);
  abb_destruir(abb2);
  liberar_cadenas(vec, VOL);
}

void pruebas_abb_alumnos(){
  primera_prueba();
  prueba_balanceado();   
  prueba_volumen();
}

/* abb_utils.c */
#include <stdlib.h>
#include "abb.h"
#include <string.h>
#include "abb_utils.h"
/* Estructura a usar */
struct pack{
  const char** claves;
  void** valores;
  size_t indice;
  bool* guardado;
};

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
/*
int compr(const char* cadena1, const char* cadena2){
  long int cad1, cad2;
  cad1 = strtol(cadena1, NULL, 10);
  cad2 = strtol(cadena2, NULL, 10);
  if(cad1 > cad2) return 1;
  if(cad1 < cad2) return -1;
  else return 0;
}*/

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

#include <stdlib.h>
#include <stdbool.h>
#include "mediana.h"
#include "heap.h"
#include <string.h>
#include <stdio.h>

struct mediana{
  heap_t* heap_max;
  heap_t* heap_min;
};

int comparar_max(const void* valor1, const void* valor2){
  const double* v1 = valor1;
  const double* v2 = valor2;
  if(*v1 > *v2) return 1;
  if(*v1 < *v2) return -1;
  return 0;
}

int comparar_min(const void* valor2, const void* valor1){
  const double* v1 = valor1;
  const double* v2 = valor2;
  if(*v1 > *v2) return 1;
  if(*v1 < *v2) return -1;
  return 0;
}

mediana_t* calculador_mediana_crear(){
  mediana_t* mediana = malloc(sizeof(mediana_t));
  if(!mediana) return NULL;
  mediana->heap_max = heap_crear(comparar_max);
  if(!mediana->heap_max) {
    free(mediana);
    return NULL;
  }
  mediana->heap_min = heap_crear(comparar_min);
  if(!mediana->heap_min){
    heap_destruir(mediana->heap_max, NULL);
    free(mediana);
    return NULL;
  }
  return mediana;
}

bool calculador_mediana_agregar_valor(mediana_t* mediana, double valor){
  bool res;
  double* val = malloc(sizeof(double));
  if(!val) return false;
  *val = valor;
  if(heap_esta_vacio(mediana->heap_max) && heap_esta_vacio(mediana->heap_min)){
    return heap_encolar(mediana->heap_min, val);       
  }
  if (valor > calculador_mediana_obtener_mediana(mediana)){
    res = heap_encolar(mediana->heap_min, val);
  }
  else  res = heap_encolar(mediana->heap_max, val);
  if(heap_cantidad(mediana->heap_min)  > heap_cantidad(mediana->heap_max)+1 ){
    heap_encolar(mediana->heap_max, heap_desencolar(mediana->heap_min));
  }
  if (heap_cantidad(mediana->heap_min)+1  < heap_cantidad(mediana->heap_max)){
    heap_encolar(mediana->heap_min, heap_desencolar(mediana->heap_max));
  }
  return res;
}

double calculador_mediana_obtener_mediana(mediana_t* mediana){
  
  if(heap_cantidad(mediana->heap_max)+heap_cantidad(mediana->heap_min) == 0) return 0;
  if(heap_cantidad(mediana->heap_max) == heap_cantidad(mediana->heap_min)){
    return (*(double*)heap_ver_max(mediana->heap_max) + *(double*)heap_ver_max(mediana->heap_min)) / 2;;
  }
  if(heap_cantidad(mediana->heap_min) > heap_cantidad(mediana->heap_max)){
    return *(double*)heap_ver_max(mediana->heap_min);
  }
  else return  *(double*)heap_ver_max(mediana->heap_max);  
}

void calculador_mediana_destruir(mediana_t* mediana){
  heap_destruir(mediana->heap_min, free);
  heap_destruir(mediana->heap_max, free);
  free(mediana);
}


// 3 4 7 10 13 50

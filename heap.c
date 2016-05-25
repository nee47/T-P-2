#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"
#include <stdio.h>
#define TAM_INI 190
#define REDIMEN 2

// Faltan las funciones heapify y crear_vector_ARr
struct heap{
  void** datos;
  size_t tam;
  size_t largo;
  cmp_func_t cmp;
};

// Cambia dos valores de un array. Recibe 2 posiciones del array.
// Pre: hay un array creado y las posiciones son validas.
void swap (void** array, size_t nodo, size_t padre) {
  void* aux = array[nodo];
  array[nodo] = array[padre];
  array[padre] = aux;
}

// Recibe una arreglo, su tamaño y una posicion inicial
// Envia al elemento del arreglo a donde debe pertenecer para 
// que el array siga manteniendo la propiedad de heap
// Pre: El array fue creado
// Post: EL elemento fue movido a su sitio correcto.
void down_heap(void** array, size_t tam, size_t actual, cmp_func_t cmp ) {
  size_t hijo_derecho = (2 * actual) + 2  , hijo_izquierdo = (2 * actual) +1, pos_max ;
  if(hijo_derecho < tam){
    if(cmp(array[hijo_izquierdo], array[hijo_derecho]) < 0){
      pos_max = hijo_derecho;
    }
    else pos_max = hijo_izquierdo;
    
    if (cmp(array[actual], array[pos_max]) < 0){
      swap(array, actual, pos_max);
    } 
    else return; 
    down_heap(array, tam, pos_max, cmp);
    return;
  } 
  if(hijo_derecho < tam || hijo_izquierdo < tam){
    if(cmp(array[actual], array[hijo_izquierdo]) < 0 ) swap(array, actual, hijo_izquierdo);
  } 
}

// Crea en memoria dinamica el heap con un tamaño como paramaetro
// post : Devuelve un heap
heap_t* creador(size_t largo){
  heap_t* heap = malloc(sizeof(heap_t));
  
  if(!heap) return NULL;
  heap->datos = malloc(sizeof(void*) * largo);
  
  if(!heap->datos){
    free(heap);
    return NULL;
  }
  return heap;
}

heap_t *heap_crear(cmp_func_t cmp){
  heap_t* heap = creador(TAM_INI);
  if(!heap) return NULL;
  heap->largo = TAM_INI;
  heap->tam = 0 ;
  heap->cmp = cmp;
  return heap;
}

// COpia todoslos elementos de un arreglo a otro
void copiar_array(void* arreglo1[], void* arreglo2[],  size_t n){
  for(size_t i = 0; i < n; i++){
    arreglo1[i] = arreglo2[i];
  }
}

// Rordena un arreglo para que tenga caracteristica de heap
void heapify(void** array, size_t n, cmp_func_t cmp){
  for(size_t i = n/2; i > 0; i--){
    down_heap(array, n, i-1, cmp);
  }
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
  heap_t* heap = creador(n);
  heap->largo = n;
  heap->tam = n;
  heap->cmp = cmp;
  copiar_array(heap->datos, arreglo, n);
  heapify(heap->datos, n, cmp);
  return heap;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
  heapify(elementos, cant, cmp);
  for(size_t i = 1; i < cant ; i++){
    swap(elementos, 0, cant-i);
    if(i < cant-1)down_heap(elementos, cant-i, 0, cmp);  
  }
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
  while(!heap_esta_vacio(heap)){
    if (destruir_elemento) {
      destruir_elemento(heap_desencolar(heap));
    }
    else heap_desencolar(heap);
  }
  free(heap->datos);
  free(heap);
}


size_t heap_cantidad(const heap_t *heap){
  return heap->tam;
}


bool heap_esta_vacio(const heap_t *heap){
  return heap->tam == 0 ? true : false;
}

// Recibe un heap y reduce el tamño de sus datos
// post: Devuelve true si puedo crear el nuevo vector, false en caso contrario.
bool redimensionar(heap_t* heap, size_t nuevo_largo ){
  void** aux = realloc(heap->datos, (sizeof(void*) * nuevo_largo));
  if (!aux) return false;
  heap->datos = aux;
  heap->largo = nuevo_largo;
  return true;
}

// Recibe una arreglo, su tamaño y una posicion inicial
// Envia al elemento del arreglo a donde debe pertenecer para 
// que el array siga manteniendo la propiedad de heap
// Pre: El array fue creado
// Post: EL elemento fue movido a su sitio correcto.
void upheap(void** a, size_t nodo, cmp_func_t cmp){
  size_t padre, actual = nodo;
  if (nodo != 0)  padre = (actual - 1)/2;
  else padre = 0 ;
  while( actual != 0 && cmp(a[actual], a[padre]) > 0 ){ // nodo < padre 
    swap(a, actual, padre);
    actual = padre;    
    padre = (actual - 1)/2;
  }
}

bool heap_encolar(heap_t *heap, void *elem){
  if(heap->tam + 2  > heap->largo ){
    size_t nuevo_largo = heap->largo * REDIMEN ;
    redimensionar(heap, nuevo_largo);
  }
  heap->datos[heap->tam] = elem ;
  upheap(heap->datos, heap->tam, heap->cmp);
  heap->tam++;
  return true;
}


void *heap_ver_max(const heap_t *heap){
  return heap_esta_vacio(heap) ? NULL : heap->datos[0];
}


void *heap_desencolar(heap_t *heap){
  if(heap_esta_vacio(heap)) return NULL;
  if(heap->largo > 4 * heap->tam && heap->tam > TAM_INI){
    redimensionar(heap, heap->largo / REDIMEN);
  }
  void* dato_salida = heap->datos[0];
  heap->datos[0] = heap->datos[heap->tam-1];
  down_heap(heap->datos, heap->tam, 0, heap->cmp);
  heap->tam--;
  return dato_salida;
}

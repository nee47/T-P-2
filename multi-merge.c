#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "heap.h"


typedef struct nodo{
  FILE* archivo;
  char* linea;

}nodo_t;

int comparar(const void* cad1, const void* cad2){
  const nodo_t* c1 = cad1;
  const nodo_t* c2 = cad2;
  int res = strcmp(c1->linea, c2->linea);
  if(res < 0) return 1;
  if(res > 0) return -1;
  return 0;
}

void destruye_nodo(void* nodex){
  nodo_t* nodo = nodex;
  fclose(nodo->archivo);
  free(nodo->linea);
  free(nodo);
}

// se ejecuta asi valgrind ./multi-merge archivo1.txt archivo2.txt archivo3.txt
int main(int argc, char** argv){
  if(argc < 2){
    printf("Uso ./multi-merge <Archivo1> <Archivo2> .. <ArchivoN>\n");
    exit(1);
  } 
  size_t  capacidad = 0;
  heap_t* heap = heap_crear(comparar);
  if(!heap) exit(1);
  for(size_t a = 1; a < argc; a++){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) exit(1);
    nodo->archivo = fopen(argv[a], "r");
    if(!nodo->archivo){
      free(nodo);
      heap_destruir(heap, destruye_nodo);
      printf("No se pudo abrir algun archivo\n");
      exit(1);   
    }
    nodo->linea = NULL;
    getline(&nodo->linea, &capacidad, nodo->archivo);
    if(!heap_encolar(heap, nodo)){
      heap_destruir(heap, destruye_nodo);
      fclose(nodo->archivo);
      exit(1);
    } 
  }
  bool fin = false;
  nodo_t* aux;
  while(!heap_esta_vacio(heap)){ 
    fin = false;
    aux = heap_desencolar(heap);
    printf("%s", aux->linea);
    if (getline(&aux->linea, &capacidad, aux->archivo) <= 0){
      fin = true;
      destruye_nodo(aux);
    }
    if(!fin && !heap_encolar(heap, aux)){
      heap_destruir(heap, destruye_nodo);
      exit(1);
    } 
  }
  
  heap_destruir(heap, NULL);
  return 0;
}

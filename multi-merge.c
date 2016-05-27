#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "heap.h"

int comparar(const void* cad1, const void* cad2){
  const char* c1 = cad1;
  const char* c2 = cad2;
  if(strcmp(c1, c2) < 0) return 1;
  if(strcmp(c1, c2) > 0) return -1;
  return 0;
}

// se ejecuta asi valgrind ./multi-merge archivo1.txt archivo2.txt archivo3.txt
int main(int argc, char** argv){
  if(argc < 2) exit(1);
  size_t  capacidad = 0;
  //ssize_t len = 0;
  char* linea = NULL;
  
  heap_t* heap = heap_crear(comparar);
  if(!heap) exit(1);
  FILE** archivo = malloc(sizeof(FILE*) * argc); 
  for(size_t a = 1; a < argc; a++){
    archivo[a-1] = fopen(argv[a], "r");
    if(!archivo[a-1]) exit(1);   
  }
  bool fin = false;
  for(size_t i = 0; i < argc-1; i++ ){
    getline(&linea, &capacidad, archivo[i]);
    if(!heap_encolar(heap, linea)){
      heap_destruir(heap, NULL);
      fclose(archivo[i]);
      exit(1);
    } 
    linea = NULL;
  }
  linea = NULL;
  char* aux;
  while(!heap_esta_vacio(heap)){ 
    fin = false;
    for(size_t i = 0; i < argc-1; i++ ){
      aux = heap_desencolar(heap);
      printf("%s", aux);         	
      if (getline(&aux, &capacidad, archivo[i]) <= 0){
	fin = true;
	free(aux);
      }
      if(!fin && !heap_encolar(heap, aux)){
	heap_destruir(heap, free);
	fclose(archivo[i]);
	exit(1);
      }
    }
  }
  for(size_t a = 0; a < argc-1; a++){
    fclose(archivo[a]);
  }
  free(archivo);
  heap_destruir(heap, free);
  return 0;
}

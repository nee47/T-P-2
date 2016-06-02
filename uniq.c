#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

int main(int argc, char**argv){
  if (argc != 2){
    printf("Uso: ./uniq <Archivo>\n");
    exit(1);
  }

  FILE* archi = fopen(argv[1], "r");
  if(!archi) {
    printf("Archivo no existente\n");
    exit(1);
  }
  size_t capacidad = 0;
  hash_t* hash = hash_crear(NULL);
  if(!hash) {
    fclose(archi);
    exit(1);
  }
  char* linea = NULL;
  ssize_t linealen = 0; 
  while((linealen = getline(&linea, &capacidad, archi) > 0)){
    if (!hash_pertenece(hash, linea))  printf("%s", linea);
    hash_guardar(hash, linea, &capacidad);
    free(linea);
    linea = NULL;
  }
  free(linea);
  hash_destruir(hash);
  fclose(archi);
  return 0;
}

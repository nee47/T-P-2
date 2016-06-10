#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"
#define ESPACIO ' '
#define FIN '\0'
#define MAX_LEN 254

// Recorta una cadena hasta el indice que se le pasa
// Devuelve una cadena alocada en memoria dinamica
// Devuelve NULL si falla malloc devuelve NULL
// Pre: str no es NULL
char *substr(const char *str, size_t n){
  char* cadena = malloc(sizeof(char) * (n+1));
  if(!cadena) return NULL;
  strncpy(cadena, str, n);
  cadena[n] = FIN;
  return cadena;
}

// Dada una cadena devuelve otra alocada en memoria dinamicamente recortada hasta donde se encuentre el primer caracter -espacio-
// Pre: linea1 no es NULL
char* obtener_campo1(char* linea1){
  char* campo_1 = substr(linea1, strlen(linea1) - strlen(strchr(linea1, ' ')));
  return campo_1;
}

// Junta 2 cadenas y las almacena en una tercera
// Todas las cadenas son distinto de NULL
char* acoplar(char** aux, char* linea1, char* linea2){
  linea1[strlen(linea1)-1] = FIN;
  *aux = stpcpy(*aux, linea1);   
  *aux = stpcpy(*aux, strchr(linea2, ESPACIO));   
  return *aux;
}

// Liberar un hash y cierra 2 archivos 
void liberar(hash_t* hash, FILE* a1, FILE* a2){
  if(hash) hash_destruir(hash);
  fclose(a1);
  fclose(a2);
} 

int main(int argc, char** argv){
  
  if(argc != 3){
    printf("Uso: ./join <Archivo1> <Archivo2>\n");
    exit(1);
  }
  
  FILE* archivo1 = fopen(argv[1], "r");
  FILE* archivo2 = fopen(argv[2], "r");
  if(!archivo1 || !archivo2) {
    if(archivo1) fclose(archivo1);
    if(archivo2) fclose(archivo2);
    printf("Error al abrir un archivo o no existe\n");
    exit(1);
  }
  hash_t* hash = hash_crear(free);
  if(!hash){
    liberar(hash, archivo1, archivo2);
    exit(1);
  }
  ssize_t len1 = 0, len2=0;
  size_t capacidad = 0;
  char* linea1 = NULL;
  char* linea2 = NULL;
  while((len2 = getline(&linea2, &capacidad, archivo2 ) > 0)){
    char* campo_1 = obtener_campo1(linea2);
    if(!hash_guardar(hash, campo_1, linea2)){
      liberar(hash, archivo1, archivo2);
      exit(1);
    }
    free(campo_1);
    linea2 = NULL;
  }
  
  while((len1 = getline(&linea1, &capacidad, archivo1 ) > 0)){
    char cadena[MAX_LEN];
    char* campo_1 = obtener_campo1(linea1);
    if(hash_pertenece(hash, campo_1)){
      char* aux = cadena;
      acoplar(&aux, linea1, (char*)hash_obtener(hash, campo_1));
      printf("%s", cadena);
    }
    free(campo_1);
  }
  liberar(hash, archivo1, archivo2);
  free(linea1);
  free(linea2);
  return 0;
}

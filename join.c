#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *substr(const char *str, size_t n){
  char* cadena = malloc(sizeof(char) * (n+1));
  strncpy(cadena, str, n);
  cadena[n] = '\0';
  return cadena;
}


int main(int argc, char** argv){
  
  if(argc != 3){
    printf("Uso: ./join <Archivo1> <Archivo2>\n");
    exit(1);
  }
  
  FILE* archivo1 = fopen(argv[1], "r");
  FILE* archivo2 = fopen(argv[2], "r");
  if(!archivo1 || !archivo2) {
    printf("Error al abrir un archivo o no existe\n");
    exit(1);
  }

  ssize_t len1 = 0, len2=0;
  size_t capacidad = 0;
  char* linea1 = NULL;
  char* linea2 = NULL;
  while((len1 = getline(&linea1, &capacidad, archivo1 ) > 0) && (len2 = getline(&linea2, &capacidad, archivo2 ) > 0)){
    char cadena[254];
    char* aux = cadena; 
    len1 = strlen(linea1);
    linea1[len1-1] = '\0';
    aux = stpcpy(aux, linea1);   
    aux = stpcpy(aux, strchr(linea2,' '));
    printf("%s", cadena);
  }
  return 0;
}

#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#define TAM_INI 179
#define CONDICION_CRECIENTE 3
#define CONDICION_DECRECIENTE 2

struct hash{
  lista_t** tabla;
  size_t largo;
  size_t tam;
  hash_destruir_dato_t destruir_dato; 
};

struct hash_iter{
  const hash_t* hash;
  size_t indice_actual;
  lista_iter_t* iter_lista;
};

typedef struct hash_nodo{
  char* clave;
  void* dato;
  
}hash_nodo_t;


size_t fh(const char *key, size_t len, size_t prime)
{
  size_t hash, i;
  for (hash=len, i=0; i<len; ++i)
    hash = (hash<<4)^(hash>>28)^key[i];
  return (hash % prime);
}


/* Crea el hash
 */


int esprimo(size_t numero){
  if(numero <= 1) return 0;
  size_t i;
  for (i=2; i*i <= numero; i++){
    if (numero % i == 0) return 0;
  }
  return 1;
}

size_t primo(size_t largo){
  size_t a;
  for( a = 0;  ;a++ ){
    if(esprimo(largo+a)) break;
  }
  return largo+a;
}

lista_t** tabla_crear(size_t largo){
  lista_t** tabla = malloc(sizeof(lista_t*) * largo);
  if(!tabla) return NULL;
  for (size_t i = 0; i < largo; i++){
    tabla[i] = NULL;
  }
  return tabla;
}


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
  hash_t* hash = malloc(sizeof(hash_t));
  if (!hash) return NULL;
  hash->tabla = tabla_crear(TAM_INI); 
  if (!hash->tabla) {
    free(hash);
    return NULL;
  }
  hash->largo = TAM_INI;
  hash->tam = 0;
  hash->destruir_dato = destruir_dato;
  return hash;
}

void destruir_nodo(hash_nodo_t* nodo){
  free(nodo->clave);
  free(nodo);
}

// Recibe una clave y un dato y crea un nodo con los elementos
// post: Si el nodo se pudo crear devuelve el nodo sino NULL
hash_nodo_t* hash_nodo_crear(const char* clave, void*  dato){
  hash_nodo_t* paquete = malloc(sizeof(hash_nodo_t));
  if (!paquete) return NULL;
  paquete->clave = strdup(clave);
  if (!paquete->clave) {
    free(paquete);
    return NULL;
  }
  paquete->dato = dato;
  return paquete;
}

// Recibe una lista y una clave. Hace una busqueda del elemento. Si lo encontro devuelve un iter no al final. Si no lo encontro devuelve el iter al final.
// Pre: Habia una lista y el iter fue creado.
// Post: Devuelve un iter que esta al final o no.
lista_iter_t* busqueda(lista_t* lista, const char* clave){
  lista_iter_t* iter = lista_iter_crear(lista);
  if(!iter) return NULL;
  hash_nodo_t* aux= lista_iter_ver_actual(iter);
  while (!lista_iter_al_final(iter)  && strcmp(aux->clave, clave) != 0) {
    lista_iter_avanzar(iter);
    aux= lista_iter_ver_actual(iter);
  }
  return iter;
}

// Recibe un hash, una tabla , un nodo y un largo.
// Almacena el  nodo
// Post: Devuelve false si malloc devuelve NULL, true en caso contrario
bool almacenar(hash_t* hash, lista_t** tabla, hash_nodo_t* nodo, size_t largo){
  bool resu = true;
  size_t indice = fh(nodo->clave, strlen(nodo->clave), largo );
  if (!tabla[indice]){ 
    tabla[indice] = lista_crear();
    if(!tabla[indice]) return false;
  }
  lista_iter_t* iter_l = busqueda(tabla[indice], nodo->clave);
  if(!iter_l) return false;
  if (lista_iter_al_final(iter_l)) {  //no lo encontro
    resu = lista_insertar_primero(tabla[indice], nodo);
    if (resu)  hash->tam++;
    lista_iter_destruir(iter_l);
    return resu;
  }
  hash_nodo_t* aux = lista_iter_ver_actual(iter_l);
  if (hash->destruir_dato) hash->destruir_dato(aux->dato); 
  aux->dato = nodo->dato;
  destruir_nodo(nodo);
  lista_iter_destruir(iter_l);
  return resu;
}


void tabla_destruir(lista_t* lista, hash_destruir_dato_t destruir_dato){ // Funcion que cree parecida a la de la lista, ya que la otra no me servia
  while (!lista_esta_vacia(lista)){
    hash_nodo_t* aux = lista_borrar_primero(lista);
    if(destruir_dato) destruir_dato(aux->dato);
    destruir_nodo(aux);
  }
  lista_destruir(lista, NULL);
}
  

void destruir_tablas(lista_t** tabla, size_t tope, hash_destruir_dato_t destruir_dato){
  for (size_t i = 0; i < tope; i++){
    if (tabla[i]) {
      tabla_destruir(tabla[i], destruir_dato);
    }
  } 
}
// Aumenta de capacidad la tabla asociada al hash
// Pre: Hash creado
// Post: LA tabla tiene mas slots
void redimensionar(hash_t* hash, size_t nuevo_largo){
  size_t largo_primo = primo(nuevo_largo), a, tam_respaldo=hash->tam;
  lista_t** nueva_tabla = tabla_crear(largo_primo);
  if(!nueva_tabla) return;
  lista_iter_t* iter_lista = NULL;
  hash_nodo_t* nodo;
  hash->tam = 0;
  for ( a = 0; a < hash->largo ; a++){
    if (hash->tabla[a]){
      iter_lista = lista_iter_crear(hash->tabla[a]);
      if (iter_lista){
	while (!lista_iter_al_final(iter_lista)){
	  nodo = lista_iter_ver_actual(iter_lista);
	  almacenar(hash, nueva_tabla, nodo, largo_primo);
	  lista_iter_avanzar(iter_lista);
	}
	lista_iter_destruir(iter_lista);      
	lista_destruir(hash->tabla[a], NULL);
      }    
      else {
	destruir_tablas(nueva_tabla, a, hash->destruir_dato);
	hash->tam = tam_respaldo;
	free(nueva_tabla);
	return;
      }
    }
  }
  hash->largo = largo_primo;
  free(hash->tabla) ;
  hash->tabla = nueva_tabla;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){
  if (hash->tam > CONDICION_CRECIENTE * hash->largo ) {
    size_t  nuevo_largo = ( hash->largo * CONDICION_CRECIENTE);
    redimensionar(hash, nuevo_largo);
  }
  hash_nodo_t* nodo = hash_nodo_crear(clave, dato);
  if (!nodo) return false;
  return almacenar(hash, hash->tabla, nodo, hash->largo); 
}


void *hash_borrar(hash_t *hash, const char *clave){
  if (!hash_pertenece(hash, clave)) return NULL;
  size_t indice = fh(clave, strlen(clave), hash->largo);  
  lista_iter_t* iter_l = busqueda(hash->tabla[indice], clave);
  if (!iter_l) return NULL;
  hash_nodo_t* aux = lista_iter_borrar(iter_l);
  void* dato_salida = aux->dato; 
  destruir_nodo(aux);
  hash->tam--;
  lista_iter_destruir(iter_l);
  if (hash->tam <  CONDICION_CRECIENTE * hash->largo && hash->largo > TAM_INI) {
    size_t  nuevo_largo = hash->largo /  CONDICION_DECRECIENTE;
    redimensionar(hash, nuevo_largo);
  }
  return dato_salida;
}


void *hash_obtener(const hash_t *hash, const char *clave){
  if(!hash_pertenece(hash, clave)) return NULL;
  size_t indice = fh(clave, strlen(clave), hash->largo);
  lista_iter_t* iter_l = busqueda(hash->tabla[indice], clave);
  if(!iter_l) return NULL;
  hash_nodo_t* aux = lista_iter_ver_actual(iter_l);
  void* dato_salida = aux->dato;
  lista_iter_destruir(iter_l);
  return dato_salida;
}


bool hash_pertenece(const hash_t *hash, const char *clave){
  size_t indice = fh(clave, strlen(clave),  hash->largo);
  if (!hash->tabla[indice]) return false;
  lista_iter_t* iter_l = busqueda(hash->tabla[indice], clave);
  if (lista_iter_al_final(iter_l)) {
    lista_iter_destruir(iter_l);
    return false;
  }
  lista_iter_destruir(iter_l);
  return true;
}

size_t hash_cantidad(const hash_t *hash){
  return hash->tam;
}



void hash_destruir(hash_t *hash){
  destruir_tablas(hash->tabla, hash->largo, hash->destruir_dato);
  free(hash->tabla);
  free(hash);
}

/* Iterador del hash */


hash_iter_t *hash_iter_crear(const hash_t *hash){
  hash_iter_t* iter = malloc(sizeof(hash_iter_t));
  if (!iter) return NULL; // corregido
  iter->hash = hash;
  iter->iter_lista = NULL;
  iter->indice_actual = 0;
  for(size_t i= 0; i < hash->largo; i++){
    if (hash->tabla[i]){
      iter->iter_lista = lista_iter_crear(hash->tabla[i]);  
      iter->indice_actual = i ;
      return iter;
    }
    iter->indice_actual = i ;
  }
  return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
  if(hash_iter_al_final(iter)) return false;
  lista_iter_avanzar(iter->iter_lista);
  if (lista_iter_al_final(iter->iter_lista)) {
    iter->indice_actual++;
    while (iter->indice_actual < iter->hash->largo){
      if (iter->hash->tabla[iter->indice_actual]){
	lista_iter_destruir(iter->iter_lista);
	iter->iter_lista = lista_iter_crear(iter->hash->tabla[iter->indice_actual]);
	return true;
      }
      iter->indice_actual++;
    }
  }
  return true;
}


const char *hash_iter_ver_actual(const hash_iter_t *iter){
  if (hash_iter_al_final(iter)) return NULL;
  hash_nodo_t* aux = lista_iter_ver_actual(iter->iter_lista);
  if (!aux) return NULL;
  return aux->clave;
}


bool hash_iter_al_final(const hash_iter_t *iter){
  if (iter->hash->tam == 0 || iter->indice_actual > iter->hash->largo-1) return true;
  if (iter->indice_actual == iter->hash->largo-1 && lista_iter_al_final(iter->iter_lista)) return true;
  return false;
}

void hash_iter_destruir(hash_iter_t* iter){
  if (iter->iter_lista)  lista_iter_destruir(iter->iter_lista);
  free(iter);
}

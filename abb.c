#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include "abb.h"
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include "pila.h"


typedef struct abb_nodo{
  struct abb_nodo* izquierda;
  struct abb_nodo* derecha;
  void* dato;
  char* clave;
}abb_nodo_t;

struct abb{
  abb_nodo_t* raiz;
  abb_destruir_dato_t destruir_dato;
  abb_comparar_clave_t cmp;
  size_t cantidad_nodos;
};

struct abb_iter{
  char* clave_hasta;
  char* clave_desde;
  abb_nodo_t* anterior;
  abb_comparar_clave_t cmp;
  pila_t* pila;
};

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
  abb_t* abb = malloc(sizeof(abb_t));
  if(!abb) return NULL;
  abb->raiz = NULL;
  abb->destruir_dato = destruir_dato;
  abb->cmp = cmp; 
  abb->cantidad_nodos = 0;
  return abb;
}

// Crea el nodo en memoria dinamica!
abb_nodo_t* crear_abb_nodo(const char* clave, void*dato){
  abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
  if(!nodo) return NULL;
  nodo->izquierda = NULL;
  nodo->derecha = NULL;
  nodo->dato = dato;
  nodo->clave = strdup(clave);
  return nodo;
}

// Recibe la clave a buscar, puntero a abb_nodo_t* osea doble puntero
// Buscar recursivamente por el hijo derecha e izquierda.
// Devuelve un puntero hacia donde se deberia insertar
// ese puntero deberia estar apuntando hacia NULL *arbol_devuelto == NULL  en caso contrario *arbol_devuelto != NULL . La clave buscada ya existia y devuelve el puntero a su ubicacion.
// Funcion utilizada en guardar, obtener, pertence.

abb_nodo_t** buscar_arbol(const char* clave, abb_nodo_t** arbol, abb_comparar_clave_t cmp) {
  if (!(*arbol)) {
    return arbol;
  }
  abb_nodo_t** aux = NULL ;
  if(cmp((*arbol)->clave, clave) < 0){  // a < b 
    if(!(*arbol)->derecha) return &(*arbol)->derecha;
    aux = buscar_arbol(clave, &(*arbol)->derecha, cmp);
    return aux;
  }
  
  if(cmp((*arbol)->clave, clave) > 0){ // a > b
    if(!(*arbol)->izquierda) return &(*arbol)->izquierda;
    aux = buscar_arbol(clave, &(*arbol)->izquierda, cmp);
    return aux;
  }
  //if(cmp((*arbol)->clave, clave) == 0)
  return arbol;
}

bool abb_guardar(abb_t *abb, const char *clave, void *dato){
  abb_nodo_t** arbol = buscar_arbol(clave, &abb->raiz, abb->cmp); 
  if (!*arbol){
    *arbol = crear_abb_nodo(clave, dato);
    if (!*arbol) return false;
    abb->cantidad_nodos++;
  }
  else { // clave repetida
    if (abb->destruir_dato) abb->destruir_dato((*arbol)->dato);
    (*arbol)->dato = dato;
  }
  return true;
}

// Funcion que calcula el el nodo con valor minimo 
// Recibe puntero a un nodo arbol (no un arbol en si ya que se necesita modificar)
// Devuelve un puntero al padre  del nodo arbol mas chico
// Funcion necesaria para borrar 
abb_nodo_t** min(abb_nodo_t** arbol, abb_comparar_clave_t cmp){ //2 //3
  abb_nodo_t** aux = arbol;
  if (!(*arbol)->izquierda) return aux;//2//3
  aux =  min(&(*arbol)->izquierda, cmp); //1ra// 
  return aux;
}

void borrar_nodo(abb_nodo_t* nodo){
  free(nodo->clave);
  free(nodo);
}

void swap(abb_nodo_t* arbol1, abb_nodo_t* arbol2){
  void* dato_aux = arbol1->dato;
  char* clave_aux = arbol1->clave;
  arbol1->dato = arbol2->dato;
  arbol1->clave = arbol2->clave;
  arbol2->dato = dato_aux;
  arbol2->clave = clave_aux;
}
bool eliminar_hoja(abb_nodo_t** arbol){
  if(!(*arbol)->derecha && !(*arbol)->izquierda){
    borrar_nodo(*arbol);
    *arbol = NULL;
    return true;
  }
  return false;
}

// Devuelve el padre de un nodo. 
// Duelve un nodo padre si es que lo tiene sino devuelve el mismo nodo
abb_nodo_t** arbol_padre(abb_nodo_t** arbol, const char* clave, abb_comparar_clave_t cmp ){
  abb_nodo_t** aux = arbol;
  if((*arbol)->izquierda){
    if(cmp((*arbol)->izquierda->clave, clave) == 0) return aux;
  }
  if((*arbol)->derecha){
    if(cmp((*arbol)->derecha->clave, clave) == 0) return aux;
  }
 
  if(cmp((*arbol)->clave, clave) < 0){
    aux = arbol_padre(&(*arbol)->derecha, clave, cmp);
    return aux;
  }
  if(cmp((*arbol)->clave, clave) > 0){
    aux = arbol_padre(&(*arbol)->izquierda, clave, cmp);
    return aux;
  }
  
  return aux;
}

// Funcion del 2do caso de borrado (el arbol tiene hijo derecho o hijo izquierdo).
// Recibe puntero al arbol y un abb 
// Contempla los 2 casos y devuelve el dato asociado a la clave .  
bool borrar_1_hijo(abb_nodo_t** arbol, abb_t* abb){
  abb_nodo_t** padre = arbol_padre(&abb->raiz, (*arbol)->clave, abb->cmp);  
  abb_nodo_t** aux;
  if(*arbol == *padre)   aux = &abb->raiz;
  else aux = padre;
  abb_nodo_t* nodo_a_borrar = *arbol ;
  if((*arbol)->derecha && !(*arbol)->izquierda) { /* 3er caso   */
    abb_nodo_t* derecha = (*arbol)->derecha;
    if((*padre)->derecha && (*arbol)->clave == (*padre)->derecha->clave ){
      (*padre)->derecha = derecha;
    }
    else if((*padre)->izquierda && (*arbol)->clave == (*padre)->izquierda->clave ) (*padre)->izquierda = derecha;
    else *aux = derecha;
    borrar_nodo(nodo_a_borrar);
    abb->cantidad_nodos--;
    return true;
  }
  
  if (!(*arbol)->derecha && (*arbol)->izquierda){
    abb_nodo_t* izquierda = (*arbol)->izquierda;
    if((*padre)->izquierda && (*arbol)->clave == (*padre)->izquierda->clave){   
      (*padre)->izquierda = izquierda;
    }
    else if((*padre)->derecha && (*arbol)->clave == (*padre)->derecha->clave) (*padre)->derecha = izquierda;
    else *aux = izquierda;
    borrar_nodo(nodo_a_borrar);
    abb->cantidad_nodos--;
    return true;
  }
  return false;
}
  
void *abb_borrar(abb_t *abb, const char *clave){
  if(!abb->raiz) return NULL;
  abb_nodo_t** arbol = buscar_arbol(clave, &abb->raiz, abb->cmp);
  if (!(*arbol)) return NULL;
  void* dato_salida = (*arbol)->dato;
  if(borrar_1_hijo(arbol, abb))
    return dato_salida;
  
  if((*arbol)->derecha && (*arbol)->izquierda){
    abb_nodo_t** minimo = min(&(*arbol)->derecha, abb-> cmp);
    swap(*arbol, *minimo);
    abb_nodo_t* derecha_minimo = (*minimo)->derecha; 
    abb_nodo_t* nodo_a_borrar = (*minimo);
    (*minimo)->derecha = (*arbol)->derecha;
    *minimo  = derecha_minimo;
    borrar_nodo(nodo_a_borrar);
    abb->cantidad_nodos--;
    return dato_salida;
  }

  if (eliminar_hoja(arbol)){
    abb->cantidad_nodos--;
    return dato_salida;
  }
  return dato_salida;
}

void *abb_obtener(const abb_t *abb, const char *clave){
  if (abb->cantidad_nodos == 0) return NULL;
  abb_nodo_t* raiz = abb->raiz;
  abb_nodo_t** arbol = buscar_arbol(clave, &raiz, abb->cmp);
  return *arbol ? (*arbol)->dato : NULL; 
}

bool abb_pertenece(const abb_t *abb, const char *clave){
  if (abb->cantidad_nodos == 0) return false;
  abb_nodo_t* raiz = abb->raiz;
  abb_nodo_t** arbol = buscar_arbol(clave, &raiz, abb->cmp);
  return *arbol ? (*arbol)->dato : NULL;
}

size_t abb_cantidad(const abb_t *arbol){
  return  arbol->cantidad_nodos  ;
}

// Destruye y libera memoria de todos los nodos del arbol.
// Recibe la raiz del arbol y recursivamente lo recorre y elimna
// en pos orden : izquierda derecha borrar_nodo
void pos_orden_destruir(abb_nodo_t* raiz, abb_destruir_dato_t destruir){
  if(!raiz) return;
  pos_orden_destruir(raiz->izquierda, destruir);
  pos_orden_destruir(raiz->derecha, destruir);
  
  if (destruir) destruir(raiz->dato);
  borrar_nodo(raiz);
}

void abb_destruir(abb_t *abb){
  pos_orden_destruir(abb->raiz, abb->destruir_dato);
  free(abb);
}

/* Iterador del abb */
// Apila el nodo actual y todos los nodos izquierdos recursivamente 
void apilar_nodos(abb_iter_t* iter, abb_nodo_t* arbol){
  if (!arbol) return ;
  pila_apilar(iter->pila, arbol);
  apilar_nodos(iter, arbol->izquierda);
}

// Crea iterador
abb_iter_t *abb_iter_in_crear(const abb_t *abb){
  abb_iter_t* iter = malloc(sizeof(abb_iter_t));
  if (!iter) return NULL;
  iter->pila = pila_crear(); 
  iter->clave_desde = NULL;
  iter->clave_hasta = NULL;
  if (abb->cantidad_nodos == 0) return iter;
  apilar_nodos(iter, abb->raiz);
  return iter;
}

void vaciar_pila(pila_t* pila){
  while(!pila_esta_vacia(pila)) pila_desapilar(pila);
}
// Avanza iterador
bool abb_iter_in_avanzar(abb_iter_t *iter){
  bool fin = false;
  int res;
  if(abb_iter_in_al_final(iter)) return false;
  abb_nodo_t* desapilado  = pila_desapilar(iter->pila);
  if(iter->clave_hasta)  res = iter->cmp(desapilado->clave, iter->clave_hasta);
  
  if(iter->clave_hasta){ 
    if(res == 0 ){
      vaciar_pila(iter->pila);
      fin = true;
    } 
  }
  if(!fin && desapilado->derecha) apilar_nodos(iter, desapilado->derecha);
  abb_nodo_t* actual = pila_ver_tope(iter->pila);
  if (actual && iter->clave_hasta && iter->cmp(actual->clave, iter->clave_hasta)>0) vaciar_pila(iter->pila);
  return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
  abb_nodo_t* aux = pila_ver_tope(iter->pila);
  return aux ? aux->clave : NULL;
}

// Comprueba si terminó la iteración

bool abb_iter_in_al_final(const abb_iter_t *iter){
  return pila_esta_vacia(iter->pila);
}

// Destruye iterador
void abb_iter_in_destruir(abb_iter_t* iter){
  free(iter->clave_desde);
  if(iter->clave_hasta) free(iter->clave_hasta);
  pila_destruir(iter->pila);
  free(iter);
}

bool in_orden(abb_nodo_t* arbol,bool visitar(const char *clave, void *dato, void *extra), void* extra ){
  if (!arbol) return true; 
  if(!in_orden(arbol->izquierda, visitar, extra)) return false;
  if(!visitar(arbol->clave, arbol->dato, extra)) return false;
  if(!in_orden(arbol->derecha, visitar, extra)) return false; 
  return true;
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
  in_orden(arbol->raiz, visitar, extra);
}

abb_iter_t* abb_iter_crear_desde(abb_t* arbol, const char* clave_desde, const char* clave_hasta){
  abb_iter_t* iter = abb_iter_in_crear(arbol);
  if(!iter) return NULL;
  iter->cmp = arbol->cmp; 
  iter->clave_desde = strdup(clave_desde);
  if(clave_hasta) iter->clave_hasta = strdup(clave_hasta);
  bool fin = false;
  
  if(clave_hasta && arbol->cmp(clave_desde, clave_hasta) > 0) vaciar_pila(iter->pila);
  else{
    while(!fin){
      abb_nodo_t* desapilado  = pila_desapilar(iter->pila);
      int res = arbol->cmp(desapilado->clave, clave_desde);
      if( res == 0  || res > 0 ){
	pila_apilar(iter->pila, desapilado); 
	fin = true;
      }
      if(!fin && desapilado->derecha) apilar_nodos(iter, desapilado->derecha);
    }
  }
  iter->anterior = NULL; 
  return iter;
} 

// Fuente: www.geeksforgeeks.org/how-to-determinate-if-a-binary-tree-is-balanced/        
// Devuelve true si el arbol esta balanceado, false en caso contrairo.
bool isbalanced(abb_nodo_t* nodo, int* altura){
  int lh = 0, rh = 0;
  
  if(!nodo) {
    *altura = 0;
    return true;
  }
  
  bool l = isbalanced(nodo->izquierda, &lh);
  bool r = isbalanced(nodo->derecha, &rh);
  
  *altura = (lh > rh ? lh : rh) +1 ;
  
  if ((lh - rh >= 2) || (rh - lh >= 2)) return false;
  else return l && r;
}

bool balanceado(abb_t* abb){
  int altura = 0;
  return isbalanced(abb->raiz, &altura);
}

#include "lista.h"
#include <stdlib.h>

typedef struct nodo{
    struct nodo* siguiente;
    void* dato;
}nodo_t;


struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter{
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
};

lista_t *lista_crear(void){
  lista_t* lista = malloc(sizeof(lista_t));
  if (!lista) return NULL;
  lista->primero = NULL ;
  lista->ultimo = NULL;
  lista->largo = 0;
  return lista;
}

bool lista_esta_vacia(const lista_t *lista){
  return (lista->largo == 0 && lista->primero == NULL && lista->ultimo == NULL ) ;
}

nodo_t*  nodo_crear(void* dato){
  nodo_t* nodo = malloc(sizeof(nodo_t));
  if (!nodo)  return NULL;
  nodo->siguiente = NULL;
  nodo->dato = dato;
  return nodo;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
  nodo_t* nodo = nodo_crear(dato);
  if (!nodo) return false;
  nodo->siguiente = lista->primero;
  if (lista_esta_vacia(lista))  lista->ultimo = nodo;
  lista->primero = nodo;
  lista->largo++ ;
  return true;
}



bool lista_insertar_ultimo(lista_t *lista, void *dato){
  if (lista_esta_vacia(lista)){
    return lista_insertar_primero(lista, dato);  
  }
  nodo_t* nodo = nodo_crear(dato);
  if (!nodo) return false;
  lista->ultimo->siguiente = nodo;
  lista->ultimo = nodo;
  lista->largo++ ;
  return true;
}


void *lista_borrar_primero(lista_t *lista){
  if (lista_esta_vacia(lista))  return NULL;
  void* dato_salida = lista->primero->dato;
  nodo_t* aux = lista->primero->siguiente;
  free(lista->primero) ;
  lista->primero = aux;
  lista->largo --;
  if (!lista->primero) lista->ultimo = NULL;
  return dato_salida;
}


void *lista_ver_primero(const lista_t *lista){
  return lista_esta_vacia(lista) ? NULL : lista->primero->dato ;
}


size_t lista_largo(const lista_t *lista){
  return lista->largo ;
}


void lista_destruir(lista_t *lista, void destruir_dato(void *)){
  while (!lista_esta_vacia(lista)){
    if (destruir_dato){
      destruir_dato(lista->primero->dato);
    }
     lista_borrar_primero(lista);
  }
 
  free(lista);
}

lista_iter_t *lista_iter_crear(lista_t *lista){
  lista_iter_t* iter = malloc(sizeof(lista_iter_t));
  if (!iter)  return NULL;
  iter->actual = lista->primero;
  iter->anterior = NULL;
  iter->lista = lista;
  return iter;
}


bool lista_iter_avanzar(lista_iter_t *iter){
  if (lista_iter_al_final(iter)) return false;
  iter->anterior = iter->actual;
  iter->actual = iter->actual->siguiente;
  return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
  return lista_iter_al_final(iter) ? NULL : iter->actual->dato;
}


bool lista_iter_al_final(const lista_iter_t *iter){
  return iter->actual ? false : true;
}

void lista_iter_destruir(lista_iter_t *iter){
  free(iter);
}


bool lista_iter_insertar(lista_iter_t *iter, void *dato){
  bool res;
  if (iter->actual == iter->lista->primero){
    res = lista_insertar_primero(iter->lista, dato);
    iter->actual = iter->lista->primero ;
    return res;
  }
  nodo_t* nodo = nodo_crear(dato);
  if (!nodo) return false;
  nodo->siguiente = iter->actual;
  if (lista_iter_al_final(iter)) iter->lista->ultimo = nodo; 
  iter->actual = nodo;
  iter->anterior->siguiente = nodo;
  iter->lista->largo++ ;
  return true;
}


void *lista_iter_borrar(lista_iter_t *iter){
  if (lista_iter_al_final(iter))  return NULL ;
  void* dato_salida = iter->actual->dato;
  if (iter->actual == iter->lista->primero) {
    iter->actual = iter->lista->primero->siguiente;
    return lista_borrar_primero(iter->lista);
  }
  if (!iter->actual->siguiente)  iter->lista->ultimo = iter->anterior;
  iter->anterior->siguiente = iter->actual->siguiente;
  free(iter->actual);
  iter->actual = iter->anterior->siguiente;
  iter->lista->largo-- ;
  return dato_salida;


}


void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra){
  nodo_t* nodo = lista->primero;
  while (nodo && visitar(nodo->dato, extra))  nodo = nodo->siguiente;
}


#include "pila.h"
#include <stdlib.h>
#define NUMERO_REDIMENSIONADOR 2
#define TAMANO_INI 50

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t tam;
    size_t largo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear(){
  pila_t* pilanueva=malloc(sizeof(pila_t));
  if (!pilanueva) return NULL;
  pilanueva->datos=malloc(sizeof(void*)*TAMANO_INI); 
  if (pilanueva->datos==NULL){
    free(pilanueva);
    return NULL;
  }
  pilanueva->tam=0;
  pilanueva->largo=TAMANO_INI;
  return pilanueva;
}


// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){
 free(pila->datos);
 free(pila);
 	
} 

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){	
  return  pila->tam==0; 
}


bool redimensionar(pila_t* pila,size_t newsize){
  void** aux_pila=realloc(pila->datos,(sizeof(void*)*newsize ));
  if (aux_pila ){ //evito perder la pila en caso de NULL
    pila->datos= aux_pila;
    pila->largo = newsize ;
    return true;
  }
  return false;
}


// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor){
  bool result=true;
  if((pila->tam)>=(pila->largo)){        
    result= redimensionar(pila,NUMERO_REDIMENSIONADOR*(pila->tam));
  }
  if (result){
    pila->datos[pila->tam]=valor;
    pila->tam++;
  }
  return result; 
	 	
}
// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila))  return NULL;
	return (pila->datos[(pila->tam)-1]);	    
}


// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila){
  if (pila_esta_vacia(pila)){
    return NULL;
  }
  pila->tam--;
  if (((pila->largo)>4*(pila->tam)) && ((pila->tam)>TAMANO_INI)){
    redimensionar(pila,pila->largo / NUMERO_REDIMENSIONADOR );
  }
  return (pila->datos[pila->tam]);	
}

// ...

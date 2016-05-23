#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>

  //  Primitivas básicas
   
struct lista;
typedef struct lista lista_t;


// Crea una lista.
// Post devuelve una lista vacia.
lista_t *lista_crear(void);

// Devuelve true o false depende de si hay elementos en la lista.
// Pre: La lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Se inserta un dato en la primera posicion.
// Si hay algun elemento este nuevo valor se inserta una poscion antes
// del primer elemento.
// Pre: La lista fue creada.
// Post: Se agrego el nuevo dato al inicio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Se agrega el dato al final de la lista y devuelve true o false
// Pre: La lista fue creada 
// Post: Se agrego un elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saco el primer elemento de la lista. Si la lista tiene elementos
// se devuelve el valor del primero, si esta vacia devuelve NULL.
// Pre: La lista fue creada.
// Post: Se  de vuelve el valor del elemento borrado. 
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primero de la lista. Si la lista tiene elementos
// se devuelve el valor del primero, si esta vacia devuelve NULL
// Pre: La lista fue creada.
// Post: Devuelve el valor del primer elemento
void *lista_ver_primero(const lista_t *lista);

// Devuelve la cantidad de elementos de la lista.
// Pre: La lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista.Si recibe destruir_dato por parametro,
// para cada uno de los elementos de la lista llama a destruir_dato
// Pre: La lista fue creada. destruir_dato es capaz de destruir
// los datos de la lista,o NULL en caso de que no se utilice.
// Post: Se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

    //Primitivas de iteración
struct lista_iter;
typedef struct lista_iter lista_iter_t;

// Crea un iterador apuntando al primero de la lista.
// Pre: La lista fue creada.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza el iterador una posicion (anterior y actual) y devuelve true.
// En  caso el iterador este al final se devuelve false.
// Pre: La lista fue creada.
// Post: Devuelve true o false segun este al final. 
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene  el valor actual del iterador.En caso de que la lista este vacia
// se obtiene NULL.
// Pre: La lista y el iterador fueron creados.
// Post: Se devolvio el valor del iterador actual si la lista no esta vacia.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Se compara si el iterador actual esta apuntado a NULL. Si es asi se
// devuelve true.
// Pre: La lista y el iterador fueron creados.
// Post: Devuelve true o false dependiendo si el iterador esta al final.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: La lista fue creada. 
void lista_iter_destruir(lista_iter_t *iter);

    //Primitivas de listas junto con iterador

// Recibe un dato y si lo pudo insetar devuelve true, en caso contrario false
//  Se inserta el valor en la posicion actual del iterador.
// Pre: La lista y el iterador fueron creados.
// Post: Se devuelve true si se pudo insertar, en caso contrario false
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borrra el elemento actual del iterador. Si no esta vacia devuelve su valor.
// Si La lista esta vacia devuelve NULL.
// Pre: La lista y el iterador fueron creados.

void *lista_iter_borrar(lista_iter_t *iter);

   // Primitivas de iterador interno (la función de callback "visitar" recibe el dato y un puntero extra, y devuelve true si se debe seguir iterando, false en caso contrario):

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

   // Función de pruebas:

void pruebas_lista_alumno(void);
#endif // LISTA_H




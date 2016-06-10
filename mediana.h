#ifndef MEDIANA_H
#define MEDIANA_H

#include <stdbool.h>
#include <stddef.h>

// Structs de la mediana
struct mediana;
typedef struct mediana mediana_t;

// Crea la mediana
mediana_t* calculador_mediana_crear();

// Agrega un valor a la estructura mediana manteniendo el orden para calcular la mediana
// Devuelve true si se pudo agregar a la estructura y false en caso contrario o en caso de que malloc falle. 
bool calculador_mediana_agregar_valor(mediana_t* mediana, double valor);

// Devuelve el valor de la posicion medio entre una cierta cantidad impar de numeros de la estrucura. En caso que la cantidad de numeros guardados sea par, se devolvera el promedio entre los valores 2 las 2 posiciones que ocupan el medio
// En caso de no haber elementos se devolvera 0.
double calculador_mediana_obtener_mediana(mediana_t* mediana);

// Destruye y libera la memoria pedida de la mediana
void calculador_mediana_destruir(mediana_t* mediana);


void pruebas_mediana_alumnos();


#endif //MEDIANA_H

#ifndef MEDIANA_H
#define MEDIANA_H

#include <stdbool.h>
#include <stddef.h>

struct mediana;
typedef struct mediana mediana_t;

mediana_t* calculador_mediana_crear();

bool calculador_mediana_agregar_valor(mediana_t* mediana, double valor);

double calculador_mediana_obtener_mediana(mediana_t* mediana);

void calculador_mediana_destruir(mediana_t* mediana);





#endif //MEDIANA_H

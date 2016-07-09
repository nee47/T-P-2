#ifndef ABB_UTILS_H
#define ABB_UTILS_H

#include <stdbool.h>
#include <stddef.h>

struct pack;
typedef struct pack pack_t;


pack_t* pack_crear(size_t cantidad);
bool obtener_par(const char* clave, void* dato, void* packi);
bool guardar_nuevo(abb_t* orig, abb_t* dest, pack_t* pack, int base, int tope, size_t cantidad);
void destruir_pack(pack_t* pack);
void abb_rebalanceado(abb_t* orig, abb_t* dest);

#endif

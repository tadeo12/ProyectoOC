#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"


void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    *m = malloc(sizeof(tMapeo));
    if(*m == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    //TODO
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    //TODO
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    //TODO
}

tValor m_recuperar(tMapeo m, tClave c){
    //TODO
}

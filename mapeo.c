#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"


void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    //CODIGO
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    //CODIGO
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    //CODIGO
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    //CODIGO
}

tValor m_recuperar(tMapeo m, tClave c){
    //CODIGO
}

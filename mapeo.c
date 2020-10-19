#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"


void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    *m = malloc(sizeof(tMapeo));
    if(*m == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);   //La longitud de la tabla es inicialmente el mayor entre 10 y ci
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
    crear_lista((*m) -> tabla_hash);                //Creo una lista pasándole el puntero de la lista
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    //TODO
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int valorHash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket= m->tabla_hash+valorHash;
    tPosicion pos= l_primera(bucket);
    tEntrada entrada= l_recuperar(bucket,pos);
    while(m->comparador(entrada->clave, c)!=0) {//mientras no encuentre la clave
        pos=l_siguiente(bucket,pos);
        entrada=l_recuperar(bucket,pos);
    }
    fEliminarEntrada()
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    //TODO
}

tValor m_recuperar(tMapeo m, tClave c){
    //TODO
}

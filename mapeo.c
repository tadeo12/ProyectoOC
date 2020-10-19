#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"


void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    *m = malloc(sizeof(tMapeo));
    if(*m == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);                               //La longitud de la tabla es inicialmente el mayor entre 10 y ci
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
    (*m) -> tabla_hash = malloc( ((*m) -> longitud_tabla) * sizeof(tLista));    //Creo los buckets
    for(int i = 0; i < (*m) -> longitud_tabla; i++)
        crear_lista((*m) -> tabla_hash + i);                                    //Creo las listas en los buckets
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
    int claveHash = m -> hash_code(c) % m -> longitud_tabla;
    tLista bucket = *((m -> tabla_hash) + claveHash);
    tPosicion p = l_primera(bucket);
    tValor aRetornar = NULL;
    tEntrada entrada = (tEntrada) l_recuperar(bucket, p);
    while(l_siguiente(bucket, p) != NULL && m -> comparador(entrada -> clave, c) == 0){     //Hasta que termine de ver todo el bucket o lo encuentre
        p = l_siguiente(bucket, p);
        entrada = (tEntrada) l_recuperar(bucket, p);
    }
    if(entrada != NULL && m -> comparador(entrada -> clave, c) == 0){                       //Si es la entrada correcta tiene la clave correcta
       aRetornar = entrada -> valor;
    }
    return aRetornar;
}

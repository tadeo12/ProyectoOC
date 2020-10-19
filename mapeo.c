#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"


void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    *m = malloc(sizeof(tMapeo));
    if(*m == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
    (*m) -> tabla_hash = malloc(sizeof(tLista));
    crear_lista((*m) -> tabla_hash);
}


/*
struct mapeo{
    unsigned int longitud_tabla;
    unsigned int cantidad_elementos;
    int (*hash_code)(void *);
    int (*comparador)(void *, void *);
    tLista * tabla_hash;
};

typedef void * tClave;
typedef void * tValor;
typedef struct entrada * tEntrada;
typedef struct mapeo * tMapeo;
*/
/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existía en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
tValor m_insertar(tMapeo m, tClave c, tValor v){
    tValor toReturn = NULL;
    if(m == NULL)
        exit(MAP_ERROR_MEMORIA);
    tEntrada e;
    e -> clave = c;
    e -> valor = v;

    unsigned int longitud = ((m)-> longitud_tabla);
    unsigned int cantElem = ((m)-> cantidad_elementos);

    if((longitud/cantElem) >= (longitud * (75/100))){
        //redimensionar
    }else{
        int claveHash = ( m -> hash_code(c) ) % ( m -> longitud_tabla );
        tPosicion actual = l_primera( (m) -> tabla_hash );
        actual -> elemento = NULL;
        int encontre = 0;
        while(!encontre && ( actual != l_ultima((m) -> tabla_hash) )){
            actual = l_siguiente( ((m) -> tabla_hash) ,actual);
            actual
        }
    }
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

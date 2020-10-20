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
        redimensionar(longitud * 2,m);
    }else{
        int claveHash = ( m -> hash_code(c) ) % ( m -> longitud_tabla );
        tPosicion actual = l_primera( (m) -> tabla_hash );
        actual -> elemento = NULL;
        int encontre = ( l_primera( (m) -> tabla_hash ) == NULL );
        tEntrada entrada;
        if(!encontre){
            while(!encontre && ( actual != l_ultima((m) -> tabla_hash) )){
                actual = l_siguiente( ((m) -> tabla_hash) ,actual);
                entrada = ( tEntrada ) actual -> elemento;
                if(!comparador(entrada -> clave, c)){
                    encontre = 1;
                    toReturn = entrada -> valor;
                    entrada -> valor = v;
                }
            }
            if(!encontre){
                actual = ( l_ultima( (m) -> tabla_hash ) );
                l_insertar((m) -> tabla_hash, actual -> siguiente, e);
                (m) -> cantidad_elementos++;
            }//end if ultimo

        }//end if1
        else{
            actual = ( l_ultima( (m) -> tabla_hash ) );
            l_insertar((m) -> tabla_hash, actual -> siguiente, e);
            (m) -> cantidad_elementos++;
        }

    }//end else
}

void redimensionar(int longitud, tMapeo * m){
    tLista * aux = (*m) -> tabla_hash;
    (*m) -> tabla_hash = malloc( longitud * sizeof( tLista ) );
    tLista aux1;
    tPosicion pos = l_primera((*m) -> tabla_hash);
   for(int i = 0; i < ( (*m) -> longitud_tabla ) ; i++){
        aux1 = *(aux + i);
        l_insertar( (*m) -> tabla_hash , pos, aux1 );
        pos = l_siguiente( (*m) -> tabla_hash, pos );
   }
   for(int i=( (*m) -> longitud_tabla ); i < longitud ; i++){
         crear_lista((*m) -> tabla_hash + i);
   }

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

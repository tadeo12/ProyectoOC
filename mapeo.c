#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"


void (*funcionEliminarClave)(void *)= NULL;
void (*funcionEliminarValor)(void *)= NULL;

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
 Retorna NULL si la clave C no exist�a en M, o Vi en caso contrario.
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

void fEliminarEntrada(tEntrada entrada){
    funcionEliminarClave(entrada->clave);
    funcionEliminarValor(entrada->valor);
    free(entrada);
    entrada=NULL;
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int valorHash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket= m->tabla_hash+valorHash;
    tPosicion fin= l_fin(bucket);
    tPosicion pos= l_primera(bucket);
    tEntrada entrada= l_recuperar(bucket,pos);
    while(m->comparador(entrada->clave, c)!=0  && pos!=fin) {
    //mientras no encuentre la clave y no haya recorrido toda la lista
        pos=l_siguiente(bucket,pos);
        entrada=l_recuperar(bucket,pos);
    }
    if(pos!=fin){
        funcionEliminarClave=fEliminarC;
        funcionEliminarValor=fEliminarV;
        l_eliminar(bucket,pos,fEliminarEntrada);
        m->cantidad_elementos--;
    }
}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
tPosicion pos;
tEntrada e;
    for(int i=0; i < ( (*m) -> longitud_tabla); i++){
        pos = l_primera(*( ((*m) -> tabla_hash ) + i));
        while( pos != l_fin( *( ((*m) -> tabla_hash ) + i) )){
           e = (tEntrada)( pos -> elemento );
           if( e != NULL){
                fEliminarC( e -> clave );
                fEliminarV( e -> valor );
           }
           free(e);
           e = NULL;
           pos = l_siguiente(*( ((*m) -> tabla_hash)),pos);
        }
        free(( ((*m) -> tabla_hash ) + i));
        *( ((*m) -> tabla_hash ) + i) = NULL;
    }
    free(m);
    m = NULL;
    pos = NULL;
}

tValor m_recuperar(tMapeo m, tClave c){
    int claveHash = m -> hash_code(c) % m -> longitud_tabla;
    tLista bucket = *((m -> tabla_hash) + claveHash);
    tPosicion p = l_primera(bucket);
    tPosicion fin = l_ultima(bucket);
    tValor aRetornar = NULL;
    tEntrada entrada = (tEntrada) l_recuperar(bucket, p);

    while(p != fin && m -> comparador(entrada -> clave, c) == 0){     //Hasta que termine de ver todo el bucket o lo encuentre
        p = l_siguiente(bucket, p);
        entrada = (tEntrada) l_recuperar(bucket, p);
    }
    if(entrada != NULL && m -> comparador(entrada -> clave, c) == 0){                       //Si es la entrada correcta tiene la clave correcta
       aRetornar = entrada -> valor;
    }
    return aRetornar;
}

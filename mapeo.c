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

void redimensionar(int longitud, tMapeo m){
    tLista* aux1 = (m -> tabla_hash);
    m -> tabla_hash = malloc( longitud * sizeof( tLista ) );
    tPosicion pos;
    int largo, largoAnterior = m -> longitud_tabla;
    m -> longitud_tabla = longitud;
    for(int i = 0 ; i < longitud ; i++){
        crear_lista(m -> tabla_hash + i);
    }
    for(int i = 0; i < largoAnterior ; i++){
        pos = l_primera(*aux1);
        largo = l_longitud(*aux1);
        for(int j = 0; j < largo; j++){
            tEntrada entrada = l_recuperar(*aux1, pos);
            m_insertar(m, &(entrada -> clave), &(entrada -> valor));
            if(j < largo - 1)
                pos = l_siguiente(*aux1, pos);
        }
        //TODO eliminar la lista
        aux1++;
    }

}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    tValor toReturn = NULL;

    float longitud = ((m)-> longitud_tabla);
    float cantElem = ((m)-> cantidad_elementos);

    if((cantElem / longitud) >= (0.75))
        redimensionar(m -> longitud_tabla * 2,m);
    int claveHash = ( m -> hash_code(c) ) % ( m -> longitud_tabla );
    printf("Lista: %d", claveHash);
    int encontre = 0;
    tLista lista = *(m -> tabla_hash + claveHash);
    int largo = l_longitud(lista);
    tPosicion actual = l_primera(lista);
    tEntrada entrada;
    printf(" Intento buscar con largo %d\n", largo);
    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(lista, actual);
        if( m -> comparador( &(entrada -> clave), &c) == 0){
            encontre = 1;
            toReturn = entrada -> valor;
            entrada -> valor = v;
        }
        if(i < longitud - 1)
            actual = l_siguiente(lista, actual);
    }

    if(!encontre){
        printf("Como no existe, lo creo\n");
        tEntrada entrada = malloc(sizeof(tEntrada));
        entrada -> clave = c;
        entrada -> valor = v;
        l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)), entrada);
        (m) -> cantidad_elementos++;
    }
    return toReturn;
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
    tValor aRetornar = NULL;
    tEntrada entrada;

    int largo = l_longitud(bucket), encontre = 0;

    printf("Intento buscar elemento en bucket %d de largo %d\n", claveHash, largo);
    for(int i = 0; i < largo && !encontre; i++){
        printf("Intento leer\n");
        entrada = l_recuperar(bucket, p);
        if( m -> comparador( &(entrada -> clave), &c) == 0){
            encontre = 1;
            aRetornar = entrada -> valor;
        }
        if(i < largo - 1)
            p = l_siguiente(bucket, p);
    }

    return aRetornar;
}

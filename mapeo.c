#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"


void (*funcionEliminarClave)(void *)= NULL;
void (*funcionEliminarValor)(void *)= NULL;

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    *m = malloc(sizeof(struct mapeo));
    if(*m == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);                               //La longitud de la tabla es inicialmente el mayor entre 10 y ci
    (*m) -> cantidad_elementos = 0;
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
    (*m) -> tabla_hash = malloc( ((*m) -> longitud_tabla) * sizeof(tLista));    //Creo los buckets
    for(int i = 0; i < (*m) -> longitud_tabla; i++)
        crear_lista((*m) -> tabla_hash + i);                                    //Creo las listas en los buckets
}

void fEliminarEntrada(tElemento entrada){
    tEntrada entry=(tEntrada) entrada;
    funcionEliminarClave(entry->clave);
    funcionEliminarValor(entry->valor);
    free(entrada);
    entrada=NULL;
}

static void insertarInterno(tMapeo m, tEntrada* entrada){
    int claveHash = ( m -> hash_code((*entrada) -> clave) ) % ( m -> longitud_tabla );
    l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)) , *entrada);//l_ultima(*(m -> tabla_hash + claveHash))
    (m) -> cantidad_elementos++;
}

void fEliminarNada(tElemento entrada){
}

void redimensionar(int longitud, tMapeo m){
    tLista* aux1 = (m -> tabla_hash);

    m -> tabla_hash = malloc( longitud * sizeof( struct celda ) );
    tPosicion pos;
    int largo, largoAnterior = m -> longitud_tabla;
    m -> longitud_tabla = longitud;
    m -> cantidad_elementos = 0;
    for(int i = 0 ; i < longitud ; i++){
        crear_lista(m -> tabla_hash + i);
    }
    for(int i = 0; i < largoAnterior ; i++){
        pos = l_primera(*(aux1 + i));
        largo = l_longitud(*(aux1 + i));
        for(int j = 0; j < largo; j++){
            tEntrada entrada = l_recuperar(*(aux1 + i), pos);
            insertarInterno(m, &entrada);
            if(j < largo - 1)
                pos = l_siguiente(*(aux1 + i), pos);
        }
        l_destruir(aux1 + i, fEliminarNada);
    }
    free(aux1);

}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    tValor toReturn = NULL;
    printf("pase m_insertar mapeo\n");
    int longitud = ((m)-> longitud_tabla);
    printf("longitud hola insertar xd : %i \n",longitud);
    int cantElem = ((m)-> cantidad_elementos);
    printf("cant elementos 0 ||| tiene: %i\n",cantElem);
    if((cantElem / longitud) >= (0.75)){
        redimensionar(m -> longitud_tabla * 2,m);
    }
    printf("redimensionar lo pase como champion \n");
    int claveHash = ( m -> hash_code(c) ) % ( m -> longitud_tabla );
    printf("claveHash en mapeo %i \n",claveHash);
    int encontre = 0;
    tLista lista = *(m -> tabla_hash + claveHash);
    printf("pase la lista \n");
    int largo = l_longitud(lista);
    printf("largo lista bucket mapeo %i\n",largo);
    tPosicion actual = l_primera(lista);
    tEntrada entrada;
    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(lista, actual);
        printf("");
        if( m -> comparador( (entrada -> clave), c) == 0){
            encontre = 1;
            toReturn = entrada -> valor;
            entrada -> valor = v;
        }
        if(i < longitud - 1)
            actual = l_siguiente(lista, actual);
    }
    printf("encontre insertar  %i \n",encontre);
    if(!encontre){
        tEntrada entrada = malloc(sizeof(struct entrada));
        entrada -> clave = c;
        //printf("clave mapeo insertar no encontre %c\n",*((char**)(entrada->clave)));
        //printf("valor mapeo insertar no encontre %i\n",*((int**)(entrada->valor)));
        entrada -> valor = v;
        printf("casi termino\n");
        l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)) , entrada);//l_ultima(*(m -> tabla_hash + claveHash))
        printf("inserte\n");
        (m) -> cantidad_elementos++;
        printf("deberia de ser 1 ||| es : m cant elem %i\n",((m) -> cantidad_elementos++));
        printf("\n");
        printf("\n");
        printf("\n");
    }
    return toReturn;
}





void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int valorHash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket= *(m -> tabla_hash + valorHash );
    tPosicion pos= l_primera(bucket);
    tEntrada entrada= l_recuperar(bucket,pos);

    int largo = l_longitud(bucket);
    int encontre = 0;
    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(bucket, pos);
        if( m -> comparador( (entrada -> clave), c) == 0){
            encontre = 1;
            funcionEliminarClave=fEliminarC;
            funcionEliminarValor=fEliminarV;
            l_eliminar(bucket,pos,fEliminarEntrada);
            m->cantidad_elementos--;
        }
        if(i < largo - 1)
            pos = l_siguiente(bucket, pos);
    }
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tLista* lista = (*m) -> tabla_hash;
    for(int i = 0; i < (*m) -> longitud_tabla; i++){
        funcionEliminarClave=fEliminarC;
        funcionEliminarValor=fEliminarV;
        l_destruir(lista + i, fEliminarNada);
    }
    free(((*m) -> tabla_hash));
    free(*m);
    *m = NULL;
}

tValor m_recuperar(tMapeo m, tClave c){
    printf("estoy en mapeo m_recuperar \n");
    printf("longitud mapeo %i\n",( m -> longitud_tabla ));
    int claveHash = (  m -> hash_code(c) ) % ( m -> longitud_tabla );
    printf("CLAVEHASH en mapeo m_recuperar %i\n",claveHash);
    tLista bucket = *((m -> tabla_hash) + claveHash);
    printf("pase la lista en m_recuperar en mapeo\n");
    tPosicion p = l_primera(bucket);
    printf("pase la tPosicion en m_recuperar en mapeo\n");
    tValor aRetornar = NULL;
    tEntrada entrada;

    int largo = l_longitud(bucket), encontre = 0;
    printf("largo %i\n",largo);

    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(bucket, p);
        printf("pase entrada en for en m_recuperar en mapeo\n");
        if(( m -> comparador( (entrada -> clave), c) ) == 0){
            encontre = 1;
            aRetornar = entrada -> valor;
            printf("ENCONTRE ES 1\n");
        }
        printf("holis\n");
        //if(i < largo - 1)
            p = l_siguiente(bucket, p);
    }
    if(aRetornar!=NULL){
         int * n = (int *) (aRetornar);
        printf("valor a retornar %d \n", (*n));
    }else printf("nullo a retornar recuperar \n");
    return aRetornar;
}

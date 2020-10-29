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
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);//la longitud minima es 10
    (*m) -> cantidad_elementos = 0;
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
    (*m) -> tabla_hash = malloc( ((*m) -> longitud_tabla) * sizeof(struct celda));
    for(int i = 0; i < (*m) -> longitud_tabla; i++)
        crear_lista((*m) -> tabla_hash + i);//creo cada bucket
}

void fEliminarEntrada(tElemento entrada){//
    tEntrada entry=(tEntrada) entrada;
    funcionEliminarClave(entry->clave);//elimino la clave de la entrada
    funcionEliminarValor(entry->valor);//enlimino el valor de la entrada
    free(entrada);
    entrada=NULL;
}

static void insertarInterno(tMapeo m, tEntrada* entrada){
    int claveHash = ( m -> hash_code((*entrada) -> clave) ) % ( m -> longitud_tabla );//obtengo el hashCode
    l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)) , *entrada);//inserto al final de bucket
    (m) -> cantidad_elementos++;//aumento la cantidad de elements
}

void fEliminarNada(tElemento entrada){      //Método para que no elimina las entradas, lo usa el redimencsionar
}

void redimensionar(int longitud, tMapeo m){
    tLista* aux1 = (m -> tabla_hash);//

    m -> tabla_hash = malloc( longitud * sizeof( struct celda ) );
    tPosicion pos;
    int largo, largoAnterior = m -> longitud_tabla;
    m -> longitud_tabla = longitud;
    m -> cantidad_elementos = 0;
    for(int i = 0 ; i < longitud ; i++){
        crear_lista(m -> tabla_hash + i);//creo los nuevos buckets
    }
    for(int i = 0; i < largoAnterior ; i++){
        pos = l_primera(*(aux1 + i));//guardo la primera posicion de cada bucket (lista)
        largo = l_longitud(*(aux1 + i));//obtengo la longitud del bucket
        for(int j = 0; j < largo; j++){
            tEntrada entrada = l_recuperar(*(aux1 + i), pos);//obtengo la entrada en cada bucket
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
    int longitud = (m-> longitud_tabla);
    int cantElem = (m-> cantidad_elementos), largo, encontre = 0;
    if((cantElem / longitud) >= (0.75)){                //Checkeo si supero el factor de carga
        redimensionar(m -> longitud_tabla * 2,m);       //Duplico el tamaño del mapeo
    }
    int claveHash = ( m -> hash_code(c) ) % ( m -> longitud_tabla );
    int encontre = 0;
    tLista lista = *(m -> tabla_hash + claveHash);
    largo = l_longitud(lista);
    tPosicion actual = l_primera(lista);
    tEntrada entrada;
    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(lista, actual);
        if(( m -> comparador( (entrada -> clave), c) )== 0){        //Si lo encuentro, actualizo su valor y retorno el anterior
            encontre = 1;
            toReturn = entrada -> valor;
            entrada -> valor = v;
        }
        if(i < longitud - 1)
            actual = l_siguiente(lista, actual);
    }

    if(!encontre){
        tEntrada entrada = malloc(sizeof(struct entrada));
        entrada -> clave = c;
        entrada -> valor = v;
        l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)) , entrada);
        (m) -> cantidad_elementos++;
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
        if( m -> comparador( &(entrada -> clave), &c) == 0){
            encontre = 1;
            funcionEliminarClave=fEliminarC;
            funcionEliminarValor=fEliminarV;
            l_eliminar(bucket,pos,fEliminarEntrada);                //Si encuentro la entrada, la elimino
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
        l_destruir(lista + i, fEliminarEntrada);            //Elimino todas las listas con sus entradas
    }
    free(((*m) -> tabla_hash));                             //Libero el arreglo de listas
    free(*m);                                               //Libero el mapeo
    *m = NULL;
}

tValor m_recuperar(tMapeo m, tClave c){
    int claveHash = m -> hash_code(c) % m -> longitud_tabla;
    tLista bucket = *((m -> tabla_hash) + claveHash);
    tPosicion p = l_primera(bucket);
    tValor aRetornar = NULL;
    tEntrada entrada;
    int largo = l_longitud(bucket), encontre = 0;

    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(bucket, p);                       //Por cada elemento en el bucket correspondiente checkeo si es el que busco
        if( m -> comparador( (entrada -> clave), c) == 0){
            encontre = 1;
            aRetornar = entrada -> valor;
        }
        if(i < largo - 1)                                       //En la última vuelta no tiene que ir al siguiente (se sale de la lista si hace siguiente del ultimo)
            p = l_siguiente(bucket, p);
    }
    return aRetornar;
}


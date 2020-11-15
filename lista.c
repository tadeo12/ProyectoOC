#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void crear_lista(tLista * l){
    *l = malloc(sizeof(struct celda));
    if(*l==NULL){
        printf("ERROR: No se pudo reservar espacio en memoria.");
        exit(LST_ERROR_MEMORIA);
    }
    (*l) -> elemento=NULL;
    (*l) -> siguiente=NULL;                                     //se crea una lista vacia con solo el dummy
}

void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion posSiguiente = p -> siguiente;
    p -> siguiente = malloc(sizeof(struct celda));
    if(p -> siguiente == NULL){
        printf("ERROR: No se pudo reservar espacio en memoria.");
        exit(LST_ERROR_MEMORIA);
    }
    p -> siguiente -> elemento = e;
    p -> siguiente -> siguiente = posSiguiente;
}

void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    tPosicion celdaAEliminar = p -> siguiente;
    if(celdaAEliminar == NULL){
        printf("ERROR: Posicion invalida.");
        exit(LST_POSICION_INVALIDA);
    }
    p -> siguiente = celdaAEliminar -> siguiente;       //a la posicion anterior le asigno el elemento en la posicion siguiente a la que quiero eliminar
    fEliminar(celdaAEliminar -> elemento);
    free(celdaAEliminar);                               //libero espacio en memoria
}

void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    if(*l != NULL){                                     // si no llegue al final
        l_destruir(&((*l) -> siguiente), fEliminar);    //recorro recursivamente
        if((*l) -> elemento != NULL)
            fEliminar((*l) -> elemento);
        free(*l);
        *l = NULL;
    }
}

tElemento l_recuperar(tLista l, tPosicion p){
    if(p -> siguiente == NULL){
        printf("ERROR: Posicion invalida.");
        exit(LST_POSICION_INVALIDA);
    }
    return p -> siguiente-> elemento;          //como se usan posiciones indirectas, retorno el elemento de la siguiente
}

tPosicion l_primera(tLista l){
    return l;
}

tPosicion l_ultima(tLista l){
    tPosicion celdaActual = l;
    if(celdaActual -> siguiente != NULL)
        while(celdaActual -> siguiente -> siguiente != NULL){
            //me fijo si el siguiente de la pos actual (indirecta) es nulo para saber si llegue a la ultima de la lista
            celdaActual = celdaActual -> siguiente;
        }
    return celdaActual;
}

tPosicion l_siguiente(tLista l, tPosicion p){
    if(p->siguiente==NULL){
        printf("ERROR: No hay siguiente.");
        exit(LST_NO_EXISTE_SIGUIENTE);
    }
    return p -> siguiente;
}

tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion celdaActual = l;                      //guardo la primera posicion para buscar
    if(l == p){
        printf("ERROR: No hay anterior.");
        exit(LST_NO_EXISTE_ANTERIOR);
    }
    while(celdaActual -> siguiente != p){           //mientras la celdaSiguiente de la actual no sea
                                                    //la que estoy buscando
        celdaActual = celdaActual -> siguiente;
    }
    return celdaActual;
}

int l_longitud(tLista l){
    int result = 0;
    tPosicion celdaActual = l;                      //guardo la primera celda
    while(celdaActual -> siguiente != NULL){
        celdaActual = celdaActual -> siguiente;
        result++;                                   //cuento la cantidad de pelementos que hay en la lista
    }
    return result;
}

tPosicion l_fin(tLista l){
    tPosicion pos = l;
    while(pos -> siguiente != NULL){                 //si la siguiente no es nula, es decir no es la pos final
        pos = pos -> siguiente;                       //busco la pos  siguiente
    }
    return pos;
}

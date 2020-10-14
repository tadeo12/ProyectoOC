#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void crear_lista(tLista * l){
    *l = malloc(sizeof(struct celda));
    if(*l==NULL)
        exit(LST_ERROR_MEMORIA);
    (*l) -> elemento=NULL;
    (*l) -> siguiente=NULL;
}

void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion aux=p->siguiente;
    p->siguiente=malloc(sizeof(struct celda));
    if(p->siguiente==NULL)
        exit(LST_ERROR_MEMORIA);
    p->siguiente->elemento=e;
    p->siguiente->siguiente=aux;
}

void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    tPosicion celdaAEliminar = p->siguiente;
    if(celdaAEliminar == NULL)
        exit(LST_POSICION_INVALIDA);
    p->siguiente=celdaAEliminar->siguiente;
    fEliminar(celdaAEliminar->elemento);
    free(celdaAEliminar);
    celdaAEliminar=NULL;
}

void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    tPosicion celdaActual=*l;
    while(celdaActual->siguiente!=NULL){
        tPosicion celdaAEliminar=celdaActual;
        celdaActual=celdaActual->siguiente;
        if(celdaAEliminar->elemento!=NULL)
            fEliminar(celdaAEliminar->elemento);
        celdaAEliminar->siguiente=NULL;
        free(celdaAEliminar);
        celdaAEliminar=NULL;
    }
    if(celdaActual->elemento!=NULL)
        fEliminar(celdaActual->elemento);
    free(celdaActual);
    celdaActual=NULL;
}

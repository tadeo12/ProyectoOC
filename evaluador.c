#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"

// Macros con códigos de finalización
#define ERROR_APERTURA_ARCHIVO        -1
#define ERROR_INVOCACION_PROGRAMA     -2


int fCompararPalabras(void * a, void * b){
    return strcmp((char *)a,(char *)b);
}

int fHashPalabras(void * a){
    //toda palabra tiene al menos una letra
    char primerLetra =*(char *)a;
    return (int) a;
}

void leerPalabras(FILE archivo, tMapeo m){


}

void evaluador (char ruta_archivo[]){
    FILE *archivo=fopen(ruta_archivo,"r");
    if(archivo==NULL)
        return -1;
    tMapeo mapeo;
    crear_mapeo(&mapeo,20,&fHashPalabras,&fCompararPalabras);
    leerPalabras(archivo,mapeo);



    return 0;
}

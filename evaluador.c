#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"

// Macros con c�digos de finalizaci�n
#define ERROR_APERTURA_ARCHIVO        -1
#define ERROR_INVOCACION_PROGRAMA     -2

int fCompararPalabras(void * a, void * b){
    char* A = ( (char*) a);
    char* B = ( (char*) b);
    return strcmp(A,B);
}

int fHashPalabras(void * a){
    char* str = ((char*) a);
    int hash = 5381;                        //Numero primo base
    int c;

    while (*str){
        str++;
        c = *str;
        hash = ((hash << 5) + hash) + c;    //hash * 33 + el caracter
    }
    hash = hash < 0 ? -hash : hash;         //Si llega a ser negativo
    return hash;
}

void fEliminarPalabra(void * a){
    free(a);
}


void fEliminarInt(void * a){
    free(a);
}

void leerPalabras(FILE * archivo, tMapeo m){
    int *cantidad;
    int * aux;
    char*palabra=malloc(30*sizeof(char));
    while(!feof(archivo)){
        fscanf(archivo, "%30s[^\n] ", palabra);
        if(!feof(archivo))
            fgetc(archivo);                      //consume espacios
        cantidad=malloc(sizeof(int));
        aux=(int *)m_recuperar(m, palabra);
        if(aux!=NULL)
            (*cantidad)=(*aux) +1;
        else
            (*cantidad)=1;
        m_insertar(m,palabra,cantidad);
        palabra = malloc(30*sizeof(char));      //reservo espacio para nueva palabra
    }
}


int main(int argc, char *argv[]){
    tMapeo mapeo;
    int * aux;
    int opcion;
    char * ruta_archivo, * palabra;
    FILE * archivo;
    if(argc!=2){
        printf("ERROR: Cantidad de argumentos");
        return ERROR_INVOCACION_PROGRAMA;
    }
    ruta_archivo = argv[1];
    palabra = malloc(sizeof(char)*30);
    archivo=fopen(ruta_archivo,"r");
    if(archivo==NULL){
        printf("ERROR: Archivo no encontrado");
        return ERROR_APERTURA_ARCHIVO;
    }
    crear_mapeo(&mapeo,20,&fHashPalabras,&fCompararPalabras);
    leerPalabras(archivo,mapeo);
    printf("Archivo leido\n");
    printf("Menu de operaciones \n");
    printf("Consultar cantidad de apariciones(1) o salir(2) ?\n");
    scanf("%d",&opcion);
    while(opcion == 1){
        printf("Ingrese la palabra a buscar \n");

        scanf("%s", palabra);
        aux=(int *)m_recuperar(mapeo,palabra);
        if (aux)
            printf("la palabra %s aparece %d veces en el archivo \n",palabra,*aux);
        else
            printf("la palabra %s no aparece en el archivo \n",palabra);
        printf("consultar cantidad de apariciones(1) o salir(2) ?\n");
        scanf("%d",&opcion);
    }
    free(palabra);                                              //Libero la palabra que la reserv� al inicio
    m_destruir(&mapeo,&fEliminarPalabra,&fEliminarInt);
    return 0;
}


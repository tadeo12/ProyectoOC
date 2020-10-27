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
    char primerLetra =*(char *)a;
    return (int) primerLetra;
}

void fEliminarPalabra(void * a){
    free(a);
}


void fEliminarInt(void * a){

}

void leerPalabras(FILE * archivo, tMapeo m){
    char caracter;
    char* palabra=malloc(30*sizeof(char));
    int contador=0;
    int * aux;
    int cantidad;
    while(!feof(archivo)){
        caracter=fgetc(archivo);
        if(caracter>32 && caracter<=126 && contador<30){//es un caracter imprimible y distinto a espacio(32)
            *(palabra+contador)=caracter;
        }
        else{
            if(contador>0){//terminamos de leer una palabra
                aux=(int *)m_recuperar(m, palabra);
                if(aux==NULL)
                    cantidad=0;
                else
                    cantidad=*aux;
                m_insertar(m,palabra,&cantidad);
                palabra=malloc(30*sizeof(char)); //reservo espacio para nueva palabra
            }
        }
    }

}

void evaluador (char *ruta_archivo){
    FILE *archivo=fopen(ruta_archivo,"r");
    if(archivo==NULL){
        printf("error, no se pudo leer el archivo");
        exit(ERROR_APERTURA_ARCHIVO);
    }
    tMapeo mapeo;
    crear_mapeo(&mapeo,20,&fHashPalabras,&fCompararPalabras);
    leerPalabras(archivo,mapeo);
    printf("Archivo leido\n");
    printf("Menu de operaciones \n");
    printf("consultar cantidad de apariciones(1) o salir(2) ?\n");
    int i;
    scanf("%d",&i);
    char palabra[30];int * aux;
    while(i==1){
        printf("Ingrese la palabra a buscar \n");
        scanf("%s", palabra);
        aux=m_recuperar(mapeo,palabra);
        if (aux==NULL){
            printf("la palabra %s no aparece en el archivo \n",palabra);
        }
        else
            printf("la palabra %s aparece %d veces en el archivo \n",palabra,*aux);
        printf("consultar cantidad de apariciones(1) o salir(2) ?\n");
        scanf("%d",&i);
    }
    m_destruir(&mapeo,&fEliminarPalabra,&fEliminarInt);

}

int main (int argc, char *argv[]){
    if(argc!=2)
        return ERROR_INVOCACION_PROGRAMA;
    evaluador(argv[1]);
    return 0;
}

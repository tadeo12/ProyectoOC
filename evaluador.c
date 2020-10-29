#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"

// Macros con códigos de finalización
#define ERROR_APERTURA_ARCHIVO        -1
#define ERROR_INVOCACION_PROGRAMA     -2

int fCompararPalabras(void * a, void * b){
    char* A = ( (char*) a);
    char* B = ( (char*) b);
    printf("%s %s \n", A,  B);
    return strcmp(A,B);
}

int fHashPalabras(void * a)
{

    //printf("hi\n");
    char* str = ((char*) a);
    printf("str: %c\n",*(str));
    int hash = 5381;
    int c;

    while (*str){
        str++;
        c = *str;
        hash = ((hash << 5) + hash) + c;
    }
    printf("hash devuelve %i\n", hash);
    return hash;
}

void fEliminarPalabra(void * a){
    free(a);
}


void fEliminarInt(void * a){
    free(a);
}

int esLetra(char c){
    return (c>64 && c<91) || (c>96 && c<123);
}

void leerPalabras(FILE * archivo, tMapeo m){

    //int aux, * ant;
   /* while(fscanf(archivo, " %29s", palabra) == 1){
        //aux = *m_recuperar(m, palabra);
        printf("%s\n", palabra);
        aux = 1;
        m_insertar(m, &palabra, &aux);

        if(*ant)
            *ant += 1;
        else{
            aux = 1;
            *ant = aux;
            m_insertar(m, palabra, aux);
        }
    }*/
    int *cantidad;
    char caracter;
    int contador=0;
    int * aux;
    //const char s[2] = " ";
    //char linea[1024];
    //char**palabra=malloc(30*sizeof(char));
    char*palabra=malloc(30*sizeof(char));
    //*palabra=malloc(30*sizeof(char));
    while(!feof(archivo)){
        //palabra=malloc(30*sizeof(char));
        fscanf(archivo, "%30s[^\n] ", palabra);
       // if(!feof(archivo))
            fgetc(archivo);
        // if(contador<30){

           /* caracter=linea[contador];
            printf("caracter %c \n", caracter);
            if(caracter!=' '){
                printf("caracter(!= espacio) %c \n", caracter);
                *((*palabra)+contador)=caracter;
                printf("palabra antes de else: %s\n", *palabra);
            }
            else{*/
                //if(contador>0){//terminamos de leer una palabra
                //contador=0;
                printf("PALABRA ANTES DE INGRESAR: %s\n", palabra);
                /*printf("primer caracter de la palabra: %d\n",  *palabra);
                printf("segundo caracter de la palabra: %d\n", *(palabra+1));*/
                cantidad=malloc(sizeof(int));
                aux=(int *)m_recuperar(m, palabra);
                printf("volvi del recuperar \n");
                if(aux!=NULL){
                    printf("if aux es null %i  \n",aux==NULL);
                    (*cantidad)=(*aux) +1;
                    printf("cantidad: %d \n",*cantidad);
                }
                else{
                    printf("else direccion aux %d  \n",aux);
                    (*cantidad)=1;
                }
                m_insertar(m,palabra,cantidad);
                printf("palabra insertada %s\n", palabra);
                palabra=malloc(30*sizeof(char)); //reservo espacio para nueva palabra
                //*palabra=malloc(30*sizeof(char)); //reservo espacio para nueva palabra
                //if(palabra==NULL)
                    //printf("NULL PALABRA \n");
                printf("\n");
            }
        //contador++;
        }


int main(int argc, char *argv[]){
    if(argc!=2)
        return ERROR_INVOCACION_PROGRAMA;
    char *ruta_archivo= argv[1];
    /*char ** pal=malloc(30 * sizeof(char));
    **pal=' ';
    fHashPalabras(pal);*/
/*
    char ** palabra=malloc(sizeof(char)*30);
    printf("Ingrese la palabra a buscar \n");
    scanf("%s", *palabra);
    tMapeo mapeo;
    crear_mapeo(&mapeo,20,&fHashPalabras,&fCompararPalabras);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    printf("Ingrese la palabra a buscar \n");
    scanf("%s", *palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
    m_recuperar(mapeo,palabra);
*/

    //char * ruta_archivo = "test.txt";

    FILE *archivo=fopen(ruta_archivo,"r");
    if(archivo==NULL){
        printf("error, no se pudo leer el archivo");
        return ERROR_APERTURA_ARCHIVO;
    }
    tMapeo mapeo;
    crear_mapeo(&mapeo,20,&fHashPalabras,&fCompararPalabras);
    leerPalabras(archivo,mapeo);
    printf("Archivo leido\n");
    printf("Menu de operaciones \n");
    printf("consultar cantidad de apariciones(1) o salir(2) ?\n");
    int i;
    scanf("%d",&i);
    char * palabra=malloc(sizeof(char)*30);
    //*palabra=malloc(sizeof(char)*30);
    int * aux;
    /*printf("Ingrese la palabra a buscar \n");
    scanf("%s", *palabra);
    int * v= 3;
    m_insertar(mapeo,palabra, &v);*/
    while(i == 1){
        printf("Ingrese la palabra a buscar \n");
        //fgets(palabra, 30, stdin);


        scanf("%s", palabra);

        /*printf("palabra a buscar: %s\n", *palabra);
        printf("primer caracter de la palabra: %d\n",  **palabra);
        printf("segundo caracter de la palabra: %d\n", *((*palabra)+1));
        printf("tercer caracter de la palabra: %d\n",  *((*palabra)+2));
        printf("cuarto caracter de la palabra: %d\n",  *((*palabra)+3));
        printf("quinto caracter de la palabra: %d\n",  *((*palabra)+4));
*/
        aux=(int *)m_recuperar(mapeo,palabra);
        //int num=(*aux==NULL);
        //printf("num %d \n", num);
        if (aux==NULL ){
            printf("entre otro if \n");
            printf("la palabra %s no aparece en el archivo \n",palabra);
        }
        else{
            printf("entre else \n");
            //printf("la palabra aparece %i veces en el archivo \n",*aux);
            printf("la palabra %s aparece %d veces en el archivo \n",palabra,*aux);

        }

        printf("consultar cantidad de apariciones(1) o salir(2) ?\n");
        scanf("%d",&i);
    }
    m_destruir(&mapeo,&fEliminarPalabra,&fEliminarInt);
    return 0;
}


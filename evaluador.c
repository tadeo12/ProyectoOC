#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"

// Macros con códigos de finalización
#define ERROR_APERTURA_ARCHIVO        -1
#define ERROR_INVOCACION_PROGRAMA     -2

int fCompararPalabras(void * a, void * b){
    char* A = *( (char**) a);
    char* B = *( (char**) a);
    printf("%s %s \n", A,  B);
    return strcmp(A,B);
}

int fHashPalabras(void * a)
{

    //printf("hi\n");
    char* str = *((char**) a);
    //printf("str: %c\n",((str)));//deberia de ser h
    int hash = 5381;
    int c;

    while (*str){
        str++;
        c = *str;
        hash = ((hash << 5) + hash) + c;
    }
    printf("Devuelvo %i\n", hash);
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
    char**palabra=malloc(30*sizeof(char));
    *palabra=malloc(30*sizeof(char));
    while(!feof(archivo)){
        //caracter=fgetc(archivo);
   // while(fgets(linea, 1024, archivo)!=NULL){
       // printf("iteracion while \n");
        //contador=0;
        //caracter=linea[0];
        //char **palabra=malloc(30*sizeof(char)+ sizeof(void*));
       // while(caracter!=NULL){
        //scanf("%10000[^\n]s", texto);
        //*palabra=strtok(linea,s);
        //*((*palabra)+4)='x';
        //printf("quinto caracter de la palabra: %c\n",  *((*palabra)+4));
        fscanf(archivo, "%30s[^\n]\s ", *palabra); fgetc(archivo);
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

                printf("palabra antes de ingresar: %s\n", *palabra);
                /*printf("primer caracter de la palabra: %d\n",  **palabra);
                printf("segundo caracter de la palabra: %d\n", *((*palabra)+1));
                printf("tercer caracter de la palabra: %d\n",  *((*palabra)+2));
                printf("cuarto caracter de la palabra: %d\n",  *((*palabra)+3));
                printf("quinto caracter de la palabra: %d\n",  *((*palabra)+4));
                */
                aux=(int *)m_recuperar(m, palabra);
                //printf("volvi del recuperar \n");
                printf("\n");
                cantidad=malloc(sizeof(int));
                if(aux==NULL)
                    *cantidad=1;
                else
                    *cantidad=(*aux) +1;
                m_insertar(m,palabra,cantidad);
                printf("palabra insertada %s\n", *palabra);
                char**palabra=malloc(30*sizeof(char)); //reservo espacio para nueva palabra
                printf("\n");
            }
        //contador++;
        }


int main(int argc, char *argv[]){
    /*
    if(argc!=2)
        return ERROR_INVOCACION_PROGRAMA;
    char *ruta_archivo= argv[1];
    */
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

    char * ruta_archivo = "t.txt";

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
    char ** palabra=malloc(sizeof(char)*30);
    *palabra=malloc(sizeof(char)*30);
    int * aux;
    /*printf("Ingrese la palabra a buscar \n");
    scanf("%s", *palabra);
    int * v= 3;
    m_insertar(mapeo,palabra, &v);*/
    while(i == 1){
        printf("Ingrese la palabra a buscar \n");
        //fgets(palabra, 30, stdin);


        scanf("%s", *palabra);

        /*printf("palabra a buscar: %s\n", *palabra);
        printf("primer caracter de la palabra: %d\n",  **palabra);
        printf("segundo caracter de la palabra: %d\n", *((*palabra)+1));
        printf("tercer caracter de la palabra: %d\n",  *((*palabra)+2));
        printf("cuarto caracter de la palabra: %d\n",  *((*palabra)+3));
        printf("quinto caracter de la palabra: %d\n",  *((*palabra)+4));
*/
        aux=(int *)m_recuperar(mapeo,palabra);
        if (aux==NULL){
            printf("la palabra %s no aparece en el archivo \n",*palabra);
        }
        else
            printf("la palabra %s aparece %d veces en el archivo \n",*palabra,*aux);
        printf("consultar cantidad de apariciones(1) o salir(2) ?\n");
        scanf("%d",&i);
    }
    m_destruir(&mapeo,&fEliminarPalabra,&fEliminarInt);
    return 0;
}


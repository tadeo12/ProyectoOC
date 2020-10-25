#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"


#define MAPEO

typedef struct persona{
    char nombre[10];
} *tPersona;

void test_lista(){
    tLista lista;
    crear_lista(&lista);
    tPosicion p = l_primera(lista);
    char *m, n = 'A';
    char n2 = 'C';
    char n3 = 'B';
    l_insertar(lista, p, &n);
    m = (char*) (l_recuperar(lista, l_primera(lista)));
    printf("Primero %c\n", *m);
    printf("Inserto B y C\n");
    l_insertar(lista, l_fin(lista), &n2);
    l_insertar(lista, l_ultima(lista), &n3);
    m = (char*) (l_recuperar(lista, l_primera(lista)));
    printf("Primero %c\n", *m);
    m = (char*) (l_recuperar(lista, l_siguiente(lista, l_primera(lista))));
    printf("Segundo %c\n", *m);
    m = (char*) (l_recuperar(lista, l_ultima(lista)));
    printf("Ultimo %c\n", *m);
}

void asesinar(tElemento p){
    free(p);
}


void test_lista_eliminar(){
    tPersona primera, segunda;
    primera= malloc(sizeof(struct persona));
    strcpy(primera->nombre, "carlos") ;

    segunda= malloc(sizeof(struct persona));
    strcpy(segunda->nombre, "charly") ;

    tLista lista;
    crear_lista(&lista);
    tPosicion p=  l_primera(lista);


    l_insertar(lista, p, primera);

    p= l_siguiente(lista,p);

    l_insertar(lista, p, segunda);

    printf("antes de eliminar todo bien \n");

    tPersona per = (tPersona) (l_recuperar(lista, l_ultima(lista)));
    printf("ultimo de la lista %s  \n", per->nombre );

    //l_eliminar(lista, l_ultima(lista),&asesinar);

    l_destruir(&lista,&asesinar);

    printf("despues de eliminar todo bien \n");

    per = (tPersona) (l_recuperar(lista, l_ultima(lista)));
    printf("ultimo de la lista %s ", per->nombre );

}

int funcionHash(void * a){
    return *((int*)a);
}

int comparador(void * a, void * b){
    return *((int*) a) - *((int*) b);
}

void test_mapeo(){
    tMapeo mapeo;
    printf("Creo mapeo\n");
    crear_mapeo(&mapeo, 10, &funcionHash, &comparador);

    int c1 = 3, v1 = 1;
    int c2 = 49, v2 = 2;
    int c3 = 1, v3 = 3;
    int c4 = 54, v4 = 4;
    int c5 = 37, v5 = 5;
    int c6 = 31, v6 = 6;
    int c7 = 9, v7 = 7;
    int c8 = 12, v8 = 8;
    int c9 = 11, v9 = 9;
    int c10 = 5, v10 = 10;
    int c11 = 24, v11 = 11;

    printf("Ingreso elemento\n");

    m_insertar(mapeo, &c1, &v1);
    m_insertar(mapeo, &c2, &v2);
    m_insertar(mapeo, &c3, &v3);
    m_insertar(mapeo, &c4, &v4);
    m_insertar(mapeo, &c5, &v5);
    m_insertar(mapeo, &c6, &v6);
    m_insertar(mapeo, &c7, &v7);
    m_insertar(mapeo, &c8, &v8);
    m_insertar(mapeo, &c9, &v9);
    m_insertar(mapeo, &c10, &v10);
    m_insertar(mapeo, &c11, &v11);

    printf("Termino de ingresar elementos\n");

    int * valor = (int *) m_recuperar(mapeo, &c11);
    printf(" valor esperado: %d | Devuelve: %d\n",v11,*valor);
    printf("\n");
    if(valor != NULL)
        printf("Recupero clave 24 : %d\n", *valor);
    valor = (int *) m_recuperar(mapeo, &c7);
    printf(" valor esperado: %d | Devuelve: %d\n",v7,*valor);
    printf("\n");
    if(valor != NULL)
        printf("Recupero clave 9 : %d\n", *valor);
    valor = (int *) m_recuperar(mapeo, &c3);
    printf(" valor esperado: %d | Devuelve: %d\n",v3,*valor);
    printf("\n");
    if(valor != NULL)
        printf("Recupero clave 1 : %d\n", *valor);
    valor = (int *) m_recuperar(mapeo, &c6);
    printf(" valor esperado: %d | Devuelve: %d\n",v6,*valor);
    printf("\n");
    if(valor != NULL)
        printf("Recupero clave 31 : %d\n", *valor);
    int c12 = 2;
    valor = (int *) m_recuperar(mapeo, &c12);
     if(valor != NULL)
        printf(" valor esperado: %d | Devuelve: NULL\n",*valor);
    else printf("no se encontro clave %i en mapeo\n",c12);
    printf("\n");
    if(valor != NULL)
        printf("Recupero clave 2 : %d\n", *valor);

    //insertar sobre existentes
    printf("INSERTAR SOBRE EXISTENTES\n");
    int v12 = 30;
    int v13 = 31;
    int v14 = 69;

    valor = (int *)m_insertar(mapeo,&c2,&v12);
    printf("clave %i",c2);
    printf(" valor esperado: %d | Devuelve: %d\n",v2,*valor);
    printf("\n");
    valor = (int *)m_insertar(mapeo,&c3,&v13);
    printf("clave %i",c3);
    printf(" valor esperado: %d | Devuelve: %d\n",v3,*valor);
    printf("\n");
    valor = (int *)m_insertar(mapeo,&c4,&v14);
    printf("clave %i",c4);
    printf(" valor esperado: %d | Devuelve: %d\n",v4,*valor);
    printf("\n");

}

int main()
{
    #ifdef LISTA
    test_lista();
    test_lista_eliminar();
    #endif // LISTA

    #ifdef MAPEO
    test_mapeo();
    #endif // MAPEO


    return 0;
}

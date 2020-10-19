#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


#define LISTA

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


    l_insertar(lista, p,&primera);

    p= l_siguiente(lista,p);

    l_insertar(lista, p,&segunda);

    printf("antes de eliminar todo bien \n");

    tPersona per = (tPersona) (l_recuperar(lista, l_ultima(lista)));
    printf("ultimo de la lista %s  \n", per->nombre );

    l_eliminar(lista,p,&asesinar);

    printf("despues de eliminar todo bien \n");

    per = (tPersona) (l_recuperar(lista, l_ultima(lista)));
    printf("ultimo de la lista %s ", per->nombre );

}

int main()
{
    #ifdef LISTA
    //test_lista();
    test_lista_eliminar();
    #endif // LISTA


    return 0;
}

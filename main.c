#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define LISTA

void test_lista(){
    tLista lista;
    crear_lista(&lista);
    tPosicion p = l_primera(lista);
    char *m, n = 'A';
    l_insertar(lista, p, &n);
    m = (char*) (l_recuperar(lista, l_primera(lista)));
    printf("Primero %c\n", *m);
    char n2 = 'C';
    l_insertar(lista, l_fin(lista), &n2);
    m = (char*) (l_recuperar(lista, l_ultima(lista)));
    printf("Ultimo %c\n", *m);
    m = (char*) (l_recuperar(lista, l_primera(lista)));
    printf("Primero %c\n", *m);
}

int main()
{
    #ifdef LISTA
    test_lista();
    #endif // LISTA
    return 0;
}

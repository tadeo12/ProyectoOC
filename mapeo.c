#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"


void (*funcionEliminarClave)(void *)= NULL;
void (*funcionEliminarValor)(void *)= NULL;

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    *m = malloc(sizeof(struct mapeo));
    if(*m == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m) -> longitud_tabla = (10 < ci ? ci : 10);                               //La longitud de la tabla es inicialmente el mayor entre 10 y ci
    (*m) -> cantidad_elementos = 0;
    (*m) -> hash_code = fHash;
    (*m) -> comparador = fComparacion;
    (*m) -> tabla_hash = malloc( ((*m) -> longitud_tabla) * sizeof(tLista));    //Creo los buckets
    for(int i = 0; i < (*m) -> longitud_tabla; i++)
        crear_lista((*m) -> tabla_hash + i);                                    //Creo las listas en los buckets
}

void fEliminarEntrada(tElemento entrada){
    tEntrada entry=(tEntrada) entrada;
    funcionEliminarClave(entry->clave);
    funcionEliminarValor(entry->valor);
    free(entrada);
    entrada=NULL;
}

static void insertarInterno(tMapeo m, tEntrada* entrada){
    int claveHash = ( m -> hash_code((*entrada) -> clave) ) % ( m -> longitud_tabla );
    l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)) , *entrada);//l_ultima(*(m -> tabla_hash + claveHash))
    (m) -> cantidad_elementos++;
}

void fEliminarNada(tElemento entrada){
}

void redimensionar(int longitud, tMapeo m){
    tLista* aux1 = (m -> tabla_hash);

    m -> tabla_hash = malloc( longitud * sizeof( struct celda ) );
    tPosicion pos;
    int largo, largoAnterior = m -> longitud_tabla;
    m -> longitud_tabla = longitud;
    m -> cantidad_elementos = 0;
    for(int i = 0 ; i < longitud ; i++){
        crear_lista(m -> tabla_hash + i);
    }
    for(int i = 0; i < largoAnterior ; i++){
        pos = l_primera(*(aux1 + i));
        largo = l_longitud(*(aux1 + i));
        for(int j = 0; j < largo; j++){
            tEntrada entrada = l_recuperar(*(aux1 + i), pos);
            insertarInterno(m, &entrada);
            if(j < largo - 1)
                pos = l_siguiente(*(aux1 + i), pos);
        }
        l_destruir(aux1 + i, fEliminarNada);
    }
    free(aux1);

}

tValor m_insertar(tMapeo m, tClave c, tValor v){

    tValor toReturn = NULL;
    printf("INSERTAR valor parametro en insertar: %d \n", *(int *)v);
    int longitud = ((m)-> longitud_tabla);
    printf("longitud insertar: %d \n",longitud);
    int cantElem = ((m)-> cantidad_elementos);
    printf("cant elem insertar: %d \n",cantElem);
    if((cantElem / longitud) >= (0.75)){
        redimensionar(m -> longitud_tabla * 2,m);
    }
    int claveHash = ( m -> hash_code(c) ) % ( m -> longitud_tabla );
    printf("claveHash en insertar: %d \n", claveHash);
    int encontre = 0;
    tLista lista = *(m -> tabla_hash + claveHash);
    int largo = l_longitud(lista);
    tPosicion actual = l_primera(lista);
    tEntrada entrada;
    //printf("largo antes del for %d \n",largo);
    for(int i = 0; i < largo && !encontre; i++){
        printf("toy en for \n");
        entrada = l_recuperar(lista, actual);
        //m->hash_code(entrada -> clave);
        if(( m -> comparador( (entrada -> clave), c) )== 0){
            encontre = 1;
            toReturn = entrada -> valor;//toReturn= *(entrada).valor
            entrada -> valor = v; //*(entrada).valor=v
            if (toReturn!=NULL){
                if (toReturn==entrada -> valor)
                    printf("Misma direccion \n");
                printf("valorAnterior: %d valorNuevo: %d \n",*(int *) toReturn,*(int *)(entrada -> valor));}
            else
                printf("valorAnterior: null valorNuevo: %d \n",*(int *)(entrada -> valor));
        }
        if(i < longitud - 1)
            actual = l_siguiente(lista, actual);
    }

    if(!encontre){
        tEntrada entrada = malloc(sizeof(struct entrada));
        //printf("clave antes de guardar %s \n",*(char **)c);
        entrada -> clave = c;
        entrada -> valor = v;
        //printf("clave al guardar %s \n",*(char **)(entrada->clave));
        l_insertar(*(m -> tabla_hash + claveHash), l_fin(*(m -> tabla_hash + claveHash)) , entrada);//l_ultima(*(m -> tabla_hash + claveHash))

        (m) -> cantidad_elementos++;
        printf("%d \n",(m) -> cantidad_elementos);
        if (toReturn!=NULL)
            printf("(no encontre)valorAnterior: %d valorNuevo: %d \n",*(int *) toReturn,*(int *)(entrada -> valor));
        else
            printf("(no encontre)valorAnterior: null valorNuevo: %d \n",*(int *)(entrada -> valor));
    }
    return toReturn;
}



void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int valorHash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket= *(m -> tabla_hash + valorHash );
    tPosicion pos= l_primera(bucket);
    tEntrada entrada= l_recuperar(bucket,pos);

    int largo = l_longitud(bucket);
    int encontre = 0;
    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(bucket, pos);
        if( m -> comparador( &(entrada -> clave), &c) == 0){
            encontre = 1;
            funcionEliminarClave=fEliminarC;
            funcionEliminarValor=fEliminarV;
            l_eliminar(bucket,pos,fEliminarEntrada);
            m->cantidad_elementos--;
        }
        if(i < largo - 1)
            pos = l_siguiente(bucket, pos);
    }
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tLista* lista = (*m) -> tabla_hash;
    for(int i = 0; i < (*m) -> longitud_tabla; i++){
        funcionEliminarClave=fEliminarC;
        funcionEliminarValor=fEliminarV;
        l_destruir(lista + i, fEliminarNada);
    }
    free(((*m) -> tabla_hash));
    free(*m);
    *m = NULL;
}

tValor m_recuperar(tMapeo m, tClave c){

    printf("RECUPERAR clave parametro %s \n",(char *)c);
    int claveHash = m -> hash_code(c) % m -> longitud_tabla;
    tLista bucket = *((m -> tabla_hash) + claveHash);
    tPosicion p = l_primera(bucket);
    tValor aRetornar = NULL;
    tEntrada entrada;

    int largo = l_longitud(bucket), encontre = 0;
    printf("longitud %i \n",largo);
    for(int i = 0; i < largo && !encontre; i++){
        entrada = l_recuperar(bucket, p);
        printf("entro al for , elementos del bucket: %d \n", largo);
        if( m -> comparador( (entrada -> clave), c) == 0){
            printf("comparador dio 0\n");
            encontre = 1;
            aRetornar = entrada -> valor;
            printf("clave guardada %s clave buscada %s \n",(char *)(entrada -> clave),(char *)c);
            printf("valorEntrada %d \n",*(int *)(entrada -> valor));
        }
        if(i < largo - 1)
            p = l_siguiente(bucket, p);
    }
    if(aRetornar!=NULL){
        printf("entre if \n");

        int* n = (int*) aRetornar;
        printf("valor a retornar %d \n", *n);
    }
    else
       printf("retorno null \n");
    printf("fin retornar \n");
    return aRetornar;
}

#ifndef LISTAVO_TURNO_H_INCLUDED
#define LISTAVO_TURNO_H_INCLUDED
#include <stdlib.h>
#include "turno.h"

typedef struct nodo{
    turno vipd;
    struct nodo *siguiente;
} Nodo;

typedef struct{
    Nodo *acceso;
    Nodo *cursor;
    Nodo *aux;
} ListaTurnoVO;

void init_listaVO(ListaTurnoVO *l){
    l->acceso = NULL; l->aux = NULL; l->cursor = NULL;
}
void reset_listaVO(ListaTurnoVO *l){
    l->cursor = l->acceso; l->aux = NULL;
}

// CORRECCION: Era void, se cambio a int.
int isempty_listaVO(ListaTurnoVO l){
    return (l.acceso == NULL);
}
int isOos_listaVO(ListaTurnoVO *l){
    return (l->cursor == NULL);
}
void forward_listaVO(ListaTurnoVO *l){
    if(l->cursor != NULL) {
        l->aux = l->cursor;
        l->cursor = l->cursor->siguiente;
    }
}
turno copy_listaVO(ListaTurnoVO *l){
    return l->cursor->vipd;
}
void supress_listaVO(ListaTurnoVO *l){
    Nodo *eliminar;
    if(isOos_listaVO(l)){ return; }
    eliminar = l->cursor;
    if(l->cursor == l->acceso){
        l->acceso = l->cursor->siguiente;
        l->cursor = l->acceso;
        l->aux = NULL;
    } else {
        l->aux->siguiente = l->cursor->siguiente;
        l->cursor = l->cursor->siguiente;
    }
    free(eliminar); // Faltaba liberar la memoria dinamica
}

void insert_listaVO(ListaTurnoVO *l, turno t){
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) exit(1);

    nuevo->vipd = t; // CORRECCION: Era punto (.)

    // Logica base reparada (Atencion: Para el PMI final esto debera comparar fechas para insertar ordenado)
    if(l->cursor == l->acceso){
        nuevo->siguiente = l->cursor;
        l->acceso = nuevo; // CORRECCION: Asignacion de punteros valida
        l->cursor = nuevo;
    } else {
        nuevo->siguiente = l->cursor;
        l->cursor = nuevo;
        l->aux->siguiente = nuevo;
    }
}
#endif // LISTAVO_TURNO_H_INCLUDED

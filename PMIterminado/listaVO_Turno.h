#ifndef LISTAVO_TURNO_H_INCLUDED
#define LISTAVO_TURNO_H_INCLUDED
#include <stdlib.h>
#include "turno.h"

typedef struct nodo{
    Turno vipd;
    struct nodo *siguiente;
} Nodo;

typedef struct{
    Nodo *acceso;
    Nodo *cursor;
    Nodo *aux;
} ListaTurnoVO;

// Modificadores (Reciben por referencia con *)
void init_listaVO(ListaTurnoVO *l){
    l->acceso = NULL; l->aux = NULL; l->cursor = NULL;
}
void reset_listaVO(ListaTurnoVO *l){
    l->cursor = l->acceso; l->aux = NULL;
}
void forward_listaVO(ListaTurnoVO *l){
    if(l->cursor != NULL) {
        l->aux = l->cursor;
        l->cursor = l->cursor->siguiente;
    }
}

// Observadores (Reciben por valor para evitar errores de tipo en CB 16.01)
int isempty_listaVO(ListaTurnoVO l){
    return (l.acceso == NULL);
}
int isOos_listaVO(ListaTurnoVO l){
    return (l.cursor == NULL);
}
Turno copy_listaVO(ListaTurnoVO l){
    return l.cursor->vipd;
}

void supress_listaVO(ListaTurnoVO *l){
    Nodo *eliminar;
    if(isOos_listaVO(*l)){ return; }
    eliminar = l->cursor;
    if(l->cursor == l->acceso){
        l->acceso = l->cursor->siguiente;
        l->cursor = l->acceso;
        l->aux = NULL;
    } else {
        l->aux->siguiente = l->cursor->siguiente;
        l->cursor = l->cursor->siguiente;
    }
    free(eliminar);
}

void insert_listaVO(ListaTurnoVO *l, Turno t){
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) exit(1);

    nuevo->vipd = t;

    if(l->cursor == l->acceso){
        nuevo->siguiente = l->cursor;
        l->acceso = nuevo;
        l->cursor = nuevo;
    } else {
        nuevo->siguiente = l->cursor;
        l->cursor = nuevo;
        l->aux->siguiente = nuevo;
    }
}
#endif // LISTAVO_TURNO_H_INCLUDED

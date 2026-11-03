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

// Observadores por valor (CB 16.01)
int isfull_listaVO(ListaTurnoVO l){ return 0; } // Requerido por el PDF
int isempty_listaVO(ListaTurnoVO l){ return (l.acceso == NULL); }
int isOos_listaVO(ListaTurnoVO l){ return (l.cursor == NULL); }
Turno copy_listaVO(ListaTurnoVO l){ return l.cursor->vipd; }

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

// Insercion ORDENADA de menor a mayor (por ID_Turno, que equivale a Fecha cronologica)
void insert_listaVO(ListaTurnoVO *l, Turno t){
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) exit(1);
    nuevo->vipd = t;
    nuevo->siguiente = NULL;

    if (l->acceso == NULL) {
        l->acceso = nuevo;
        l->cursor = nuevo;
        l->aux = NULL;
        return;
    }

    if (getId(t) < getId(l->acceso->vipd)) {
        nuevo->siguiente = l->acceso;
        l->acceso = nuevo;
        l->cursor = nuevo;
        l->aux = NULL;
        return;
    }

    Nodo *actual = l->acceso;
    Nodo *anterior = NULL;
    while (actual != NULL && getId(actual->vipd) < getId(t)) {
        anterior = actual;
        actual = actual->siguiente;
    }

    nuevo->siguiente = actual;
    if (anterior != NULL) anterior->siguiente = nuevo;
    
    l->cursor = nuevo;
    l->aux = anterior;
}
#endif // LISTAVO_TURNO_H_INCLUDED

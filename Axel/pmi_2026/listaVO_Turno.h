#ifndef LISTAVO_TURNO_H_INCLUDED
#define LISTAVO_TURNO_H_INCLUDED
#include <string.h>
#include "turno.h"
#include "paciente.h"

//struct nodo
typedef struct{
    turno vipd;
    struct nodo *siguiente;
}Nodo;

//struct lista
typedef struct{
    Nodo *acceso;   //cabeza de lista
    Nodo *cursor;   //cursor de lista
    Nodo *aux;      //anterior de lista
}ListaTurnoVO;

void init_listaVO(ListaTurnoVO *l){
    l->acceso=NULL;
    l->aux=NULL;
    l->cursor=NULL;
}

void reset_listaVO(ListaTurnoVO *l){
    l->cursor=l->acceso;
    l->aux=NULL;
}

void isempty_listaVO(ListaTurnoVO l){
    return (l.acceso==NULL);
}

void isOos_listaVO(ListaTurnoVO *l){
    return (l.cursor==NULL);
}

void forward_listaVO(ListaTurnoVO *l){
    if(!isOos_listaVO(l)){
        l->aux=l->cursor;
        l->cursor=l->cursor->siguiente;
    }
}

int copy_listaVO(ListaTurnoVO l){
    if (!isOos_listaVO(l)){
        return l.cursor->vipd;
    }
}

void supress_listaVO(ListaTurnoVO *l){
    Nodo *eliminar;
    if(isOos_listaVO(l)){return;}
    eliminar=l->cursor;
    if(l->cursor==l->acceso){
        l->acceso=l->cursor->siguiente;
        l->cursor=l->acceso;
        l->aux=NULL;
    }
}

#endif // LISTAVO_TURNO_H_INCLUDED

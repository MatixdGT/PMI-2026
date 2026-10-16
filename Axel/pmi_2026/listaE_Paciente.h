#ifndef LISTAE_PACIENTE_H_INCLUDED
#define LISTAE_PACIENTE_H_INCLUDED
#include <string.h>
#include <malloc.h>
#include "paciente.h"
#define MAX 3       //esta asi para las pruebas, debemos cambiar

typedef struct{
    Paciente paciente[MAX];
    int cursor;
    int ultimo;
}ListaPacientesE;

void init_listaE(ListaPacientesE *l){
    l->cursor=-1;
    l->ultimo=-1;
}

void forward_listaE(ListaPacientesE *l){
    l->cursor++;
}

void reset_listaE(ListaPacientesE *l){
    l->cursor=0;
}

void isfull_listaE(ListaPacientesE l){
    return (l.ultimo == MAX-1);
}

void isempty_listaE(ListaPacientesE l){
    return (l.ultimo == -1);
}

void isOos_listaE(ListaPacientesE l){           //consultar si es el cursor o el ultimo el que tiene que comparar
    return (l.cursor == l.ultimo+1);
}

void copy_listaE(ListaPacientesE l){
    return l->paciente[l->cursor]
}

void insert_listaE(ListaPacientesE *l, paciente x){
    if (isempty_listaE(l)){
        l->cursor++;
        l->ultimo++;
    }else{
        l->ultimo++;
        }
    l->paciente[l->ultimo]=x;
    }

void supress_listaE(ListaPacientesE *l){
    int i;
    for(i=l->cursor;i<l->ultimo;i++){
        l->paciente[i]=l->paciente[i+1];
    }
    l->ultimo--;
    if(isempty_listaE(l)){
        l->cursor=-1;
    }
}



#endif // LISTAE_PACIENTE_H_INCLUDED

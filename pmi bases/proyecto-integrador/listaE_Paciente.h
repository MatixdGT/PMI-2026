#ifndef LISTAE_PACIENTE_H_INCLUDED
#define LISTAE_PACIENTE_H_INCLUDED
#include "paciente.h"
#define MAX 3

typedef struct{
    Paciente paciente[MAX];
    int cursor;
    int ultimo;
} ListaPacientesE;

void init_listaE(ListaPacientesE *l){ l->cursor = 0; l->ultimo = -1; }
void forward_listaE(ListaPacientesE *l){ l->cursor++; }
void reset_listaE(ListaPacientesE *l){ l->cursor = 0; }

// CORRECCION: Todas estas eran void pero retornaban un valor. Ahora son int.
int isfull_listaE(ListaPacientesE l){ return (l.ultimo == MAX-1); }
int isempty_listaE(ListaPacientesE l){ return (l.ultimo == -1); }
int isOos_listaE(ListaPacientesE l){ return (l.cursor > l.ultimo); }

Paciente copy_listaE(ListaPacientesE l){ return l.paciente[l.cursor]; }

// CORRECCION: Al ser lista desordenada, se inserta siempre al final.
void insert_listaE(ListaPacientesE *l, Paciente x){
    if (l->ultimo < MAX - 1) {
        l->ultimo++;
        l->paciente[l->ultimo] = x;
    }
}

void supress_listaE(ListaPacientesE *l){
    int i;
    for(i = l->cursor; i < l->ultimo; i++){
        l->paciente[i] = l->paciente[i+1];
    }
    l->ultimo--;
}
#endif // LISTAE_PACIENTE_H_INCLUDED

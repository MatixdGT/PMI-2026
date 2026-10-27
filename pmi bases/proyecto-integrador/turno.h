#ifndef TURNO_H_INCLUDED
#define TURNO_H_INCLUDED
#include "paciente.h"
#include "fecha.h"

typedef struct turno{
    int id_turno;
    long dni_paciente;
    float monto;
    int mutual; //Codificada 1.OSDE     2.PAMI      3.OCECAC        4.DOSPU
    Fecha fecha;
    int estado; //Codificado 1.Solicitado   2.Confirmado    3.Atendido  4.Cancelado
} turno;

void setId(turno *t, int id){ t->id_turno = id; }
void setPaciente(turno *t, long dni){ t->dni_paciente = dni; }
void setMonto(turno *t, float monto){ t->monto = monto; }

int setMutual(turno *t, int mutual){
    if(mutual < 1 || mutual > 4){ // CORRECCION: Era &&
        return 0;
    } else {
        t->mutual = mutual;
        return 1;
    }
}
void setFechaTurno(turno *t, int dia, int mes, int anio, int hora, int minuto, int disponible){
    setFecha(&(t->fecha), dia, mes, anio, hora, minuto, disponible);
}
int setEstado(turno *t, int estado){
    if(estado < 1 || estado > 4){ // CORRECCION: Era &&
        return 0;
    } else {
        t->estado = estado;
        return 1;
    }
}

int getId(turno t){ return t.id_turno; }
long getPaciente(turno t){ return t.dni_paciente; }
float getMonto(turno t){ return t.monto; }
int getMutual(turno t){ return t.mutual; }
Fecha getFechaTurno(turno t){ return t.fecha; }
int getEstado(turno t){ return t.estado; }
#endif // TURNO_H_INCLUDED

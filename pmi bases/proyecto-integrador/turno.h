#ifndef TURNO_H_INCLUDED
#define TURNO_H_INCLUDED
#include "paciente.h"
#include "fecha.h"

typedef struct turno {
    int id_turno;
    long dni_paciente;
    float monto;
    int mutual; //Codificada 1.OSDE     2.PAMI      3.OCECAC        4.DOSPU
    Fecha fecha;
    int estado; //Codificado 1.Solicitado   2.Confirmado    3.Atendido  4.Cancelado
} Turno;

// FUNCIONES SET
void setId(Turno *t, int id) {
    t->id_turno = id;
}

void setPaciente(Turno *t, long dni) {
    t->dni_paciente = dni;
}

void setMonto(Turno *t, float monto) {
    t->monto = monto;
}

void setMutual(Turno *t, int mutual) {
    t->mutual = mutual;
}

void setFechaTurno(Turno *t, int dia, int mes, int anio, int hora, int minuto, int disponible) {
    setFecha(&(t->fecha), dia, mes, anio, hora, minuto, disponible);
}

void setEstado(Turno *t, int estado) {
    t->estado = estado;
}

// FUNCIONES GET
int getId(Turno t) {
    return t.id_turno;
}

long getPaciente_dni(Turno t) {
    return t.dni_paciente;
}

float getMonto(Turno t) {
    return t.monto;
}

int getMutual(Turno t) {
    return t.mutual;
}

Fecha getFechaTurno(Turno t) {
    return t.fecha;
}

int getEstado(Turno t) {
    return t.estado;
}

#endif // TURNO_H_INCLUDED

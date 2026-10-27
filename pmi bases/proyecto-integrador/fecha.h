#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED
#include <stdio.h>

typedef struct {
    int dia;
    int mes;
    int anio;
    int hora;
    int minuto;
    int disponible; // 0= Ocupado, 1= Libre
} Fecha;

void initFecha(Fecha *f) {
    f->dia = 0;
    f->mes = 0;
    f->anio = 2026;
    f->hora = 0;
    f->minuto = 0;              
    f->disponible = 1;
}

// FUNCIONES SET (Asignacion directa sin ifs)
void setDia(Fecha *f, int dia) { f->dia = dia; }
void setMes(Fecha *f, int mes) { f->mes = mes; }
void setAnio(Fecha *f, int anio) { f->anio = anio; }
void setHora(Fecha *f, int hora) { f->hora = hora; }
void setMinuto(Fecha *f, int minuto) { f->minuto = minuto; }
void setDisponible(Fecha *f, int disponible) { f->disponible = disponible; }

void setFecha(Fecha *f, int dia, int mes, int anio, int hora, int minuto, int disponible) {
    setAnio(f, anio);
    setMes(f, mes);
    setDia(f, dia);
    setHora(f, hora);
    setMinuto(f, minuto);
    setDisponible(f, disponible);
}

void setFecha_completa(Fecha *f, Fecha fnueva) {
    *f = fnueva;
}

// FUNCIONES GET
int getDia(Fecha f) { return f.dia; }
int getMes(Fecha f) { return f.mes; }
int getAnio(Fecha f) { return f.anio; }
int getHora(Fecha f) { return f.hora; }
int getMinuto(Fecha f) { return f.minuto; }
int getDisponible (Fecha f) { return f.disponible; }

void mostraFecha(Fecha f) {
    printf("%d/%d/%d\t%d:%dHS\n", f.dia, f.mes, f.anio, f.hora, f.minuto);
}

Fecha getFechaCompleta(Fecha f) {
    return f;
}

#endif // FECHA_H_INCLUDED

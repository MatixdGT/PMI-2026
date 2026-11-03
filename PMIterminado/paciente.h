#ifndef PACIENTE_H_INCLUDED
#define PACIENTE_H_INCLUDED
#include <string.h>

typedef struct {
    char nombre[20];
    char apellido[20];
    long dni;
    int osocial; // Codificado 0:No posee    1:Posee
} Paciente;

// FUNCIONES SET
void setNombre(Paciente *p, char nuevoNombre[]) {
    strcpy(p->nombre, nuevoNombre);
}

void setApellido(Paciente *p, char nuevoApellido[]) {
    strcpy(p->apellido, nuevoApellido);
}

void setDni(Paciente *p, long nuevoDni) {
    p->dni = nuevoDni;
}

void setOsocial(Paciente *p, int nuevoOsocial) {
    p->osocial = nuevoOsocial;
}

// FUNCIONES GET
char* getNombre(Paciente p) {
    char *nom = (char*)malloc(strlen(p.nombre) + 1);
    if (nom == NULL) {
        exit(1);
    }
    strcpy(nom, p.nombre);
    return nom;
}

char* getApellido(Paciente p) {
    char *app = (char*)malloc(strlen(p.apellido) + 1);
    if (app == NULL) {
        exit(1);
    }
    strcpy(app, p.apellido);
    return app;
}

long getDni(Paciente p) {
    return p.dni;
}

int getOsocial(Paciente p) {
    return p.osocial;
}

#endif // PACIENTE_H_INCLUDED

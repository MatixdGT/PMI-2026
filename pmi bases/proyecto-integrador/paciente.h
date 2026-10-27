#ifndef PACIENTE_H_INCLUDED
#define PACIENTE_H_INCLUDED
#include <string.h>
#include <stdlib.h>

typedef struct{
    char nombre[20];
    char apellido[20];
    long dni;
    int osocial; // Codificado 0:No posee    1:Posee
} Paciente;

void setNombre(Paciente *p, char nuevoNombre[]){
    strcpy(p->nombre, nuevoNombre);
}
void setApellido(Paciente *p, char nuevoApellido[]){
    strcpy(p->apellido, nuevoApellido);
}
void setDni(Paciente *p, long nuevoDni){
    p->dni = nuevoDni;
}
void setOsocial(Paciente *p, int nuevoOsocial){
    p->osocial = nuevoOsocial;
}

// CORRECCION: Se pasa por puntero constante y se retorna el string directo (sin malloc)
const char* getNombre(const Paciente *p){
    return p->nombre;
}
// CORRECCION: Estaba como "void getApellido", debe retornar const char*
const char* getApellido(const Paciente *p){
    return p->apellido;
}
long getDni(Paciente p){
    return p.dni;
}
int getOsocial(Paciente p){
    return p.osocial;
}
#endif // PACIENTE_H_INCLUDED

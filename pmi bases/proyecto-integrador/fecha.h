#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

typedef struct{
    int dia;
    int mes;
    int anio;
    int hora;
    int minuto;
    int disponible; //Codificado 0= Ocupado   1= Libre
}Fecha;
//Inicializando fecha en 2026
void initFecha(Fecha *f){
    (*f).dia = 0;
    (*f).mes = 0;
    (*f).anio = 2026;
    (*f).hora = 0;
    (*f).minuto = 0;              //saca controles , los controles van en el main ,las funciones solamente retorna y ya
    (*f).disponible = 1;
}
//Funciones SET
void setDia(Fecha *f, int dia){
    /*
    int maxDias=31;
    if((*f).mes==4 || (*f).mes==6 || (*f).mes==9 || (*f).mes==11){
        maxDias=30;
    }
    else if((*f).mes==2){
        maxDias=28;
    }
    if(dia<1 || dia>maxDias){
        return 0;
    }
    else{
        (*f).dia = dia;
        return 1;
    }
    */

    (*f).dia = dia;
}
int setMes(Fecha *f, int mes){
    if(mes<1||mes>12){
        return 0;
    }
    else{
        (*f).mes = mes;
        return 1;
    }
}
int setAnio(Fecha *f, int anio){
    if(anio<2026||anio>9999){
        return 0;
    }
    else{
        (*f).anio = anio;
        return 1;
    }
}
int setHora(Fecha *f, int hora){
    if(hora<0 || hora>23){
        return 0;
    }
    else{
        (*f).hora = hora;
        return 1;
    }
}
int setMinuto(Fecha *f, int minuto){
    if(minuto!=0 && minuto!=30){
        return 0;
    }
    else{
        (*f).minuto = minuto;
        return 1;
    }
}
void setDisponible(Fecha *f, int disponible){
    (*f).disponible = disponible;
}
 //PREGUNTAR POR DISPONIBLE y POR ANIO y CONTROLES DE ENTRADA DE DATOS
void setFecha(Fecha *f, int dia, int mes, int anio, int hora, int minuto, int disponible){
    setAnio(f, anio);
    setMes(f, mes);
    setDia(f, dia);
    setHora(f, hora);
    setMinuto(f, minuto);
    setDisponible(f, disponible);
}

void setFecha_completa(Fecha *f, Fecha fnueva){
    *f = fnueva;
}

//PREGUNTAR POR LA MEJOR OPCION DE LAS 2 SETFECHA

//Funciones GET
int getDia(Fecha f){
    return f.dia;
}
int getMes(Fecha f){
    return f.mes;
}
int getAnio(Fecha f){
    return f.anio;
}
int getHora(Fecha f){
    return f.hora;
}
int getMinuto(Fecha f){
    return f.minuto;
}
int getDisponible (Fecha f){
    return f.disponible;
}

void mostraFecha(Fecha f){
    int dia, mes, anio, hora, minuto;
    dia = f.dia;
    mes = f.mes;
    anio = f.anio;
    hora = f.hora;
    minuto = f.minuto;
    printf("%d/%d/%d\t%d:%dHS\n",dia,mes,anio,hora,minuto);

}

Fecha getFechaCompleta(Fecha f) {
    return f;
}

#endif // FECHA_H_INCLUDED

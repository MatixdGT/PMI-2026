#include <stdio.h>
#include <stdlib.h>
#include "fecha.h"
#include "paciente.h"
#include "turno.h"
#include "listaE_Paciente.h"
#include "listaVO_Turno.h"

#define Maxarr 390

// ====================================================================
// FUNCIONALIDAD (o): Cargar la agenda automáticamente al iniciar.
// Crea los 390 turnos de 30 min desde 1/11/2026 8:00hs hasta 31/12/2026.
// ====================================================================
void inicializar_agenda(Fecha agenda[]) {
    int i = 0;
    int mes, dia, hora, minuto;

    for (mes = 11; mes <= 12; mes++) {
        int limite_dias = (mes == 11) ? 30 : 31;

        for (dia = 1; dia <= limite_dias; dia++) {
            for (hora = 8; hora < 13; hora++) {
                for (minuto = 0; minuto <= 30; minuto += 30) {
                    if (i < Maxarr) {
                        // 1 = Libre
                        setFecha(&agenda[i], dia, mes, 2026, hora, minuto, 1);
                        i++;
    }}}}}
    printf("-> [SISTEMA] Agenda inicializada con %d turnos libres.\n", i);
}

// ====================================================================
// FUNCIONALIDAD (b): Busca fecha libre en agenda segun dia/mes
// ====================================================================
void buscar_fecha_libre(Fecha agenda[]) {
    int dia_buscado, mes_buscado;
    printf("Ingrese el MES (11 o 12): ");
    scanf("%d", &mes_buscado);
    printf("Ingrese el DIA: ");
    scanf("%d", &dia_buscado);

    printf("\n--- TURNOS LIBRES EL %02d/%02d/2026 ---\n", dia_buscado, mes_buscado);
    int encontrados = 0;
    int i;
    for (i = 0; i < Maxarr; i++) {
        if (getMes(agenda[i]) == mes_buscado &&
            getDia(agenda[i]) == dia_buscado &&
            getDisponible(agenda[i]) == 1) {

            printf("ID Turno: %d | Horario: %02d:%02d hs\n",
                   i, getHora(agenda[i]), getMinuto(agenda[i]));
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No hay turnos libres para la fecha seleccionada.\n");
    }
}
// ==========================================================================================
// FUNCIONALIDAD (d): Busca fecha Ocupada en agenda segun mes y que pase el id y las horas
// ==========================================================================================

void buscar_fecha_Ocupada(Fecha agenda[]) {
    int mes_buscado;
    printf("Ingrese el MES a buscar (11 para Noviembre, 12 para Diciembre): ");
    scanf("%d", &mes_buscado);

    while (mes_buscado != 11 && mes_buscado != 12) {
        printf("Error. El mes debe ser 11 o 12. Intente de nuevo: ");
        scanf("%d", &mes_buscado);
    }

    printf("\n--- TURNOS OCUPADOS EL %02d/2026 ---\n", mes_buscado);
    int encontrados = 0;
    int i;
    for (i = 0; i < Maxarr; i++) {
        if (getMes(agenda[i]) == mes_buscado && getDisponible(agenda[i]) == 0) {

            printf("ID Turno: %d | Horario: %02d:%02d hs\n", i, getHora(agenda[i]), getMinuto(agenda[i]));
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No hay turnos Ocupados para el mes seleccionada.\n");
    }
}


// ==========================================================================================
// Cargar paciente por teclado
// ==========================================================================================
void cargar_paciente(ListaPacientesE *lp) {
    if (isfull_listaE(*lp)) {
        printf("Error: La lista de pacientes esta llena.\n");
        return;
    }

    Paciente nuevoPac;
    char nombreax[20], apellidoax[20];
    long dniax;
    int osocialax;

    printf("\n--- CARGAR NUEVO PACIENTE ---\n");
    getchar();
    printf("Ingrese el Nombre: ");
    scanf(" %[^\n]s", nombreax);
    getchar();
    printf("Ingrese el Apellido: ");
    scanf(" %[^\n]s", apellidoax);
    getchar();
    printf("Ingrese el DNI: ");
    scanf("%ld", &dniax);
    while (dniax <= 0) {
        printf("DNI invalido. Ingrese nuevamente: ");
        scanf("%ld", &dniax);
    }

    do {
        printf("¿Tiene Obra Social? (1=Si, 0=No): ");
        scanf("%d", &osocialax);
    } while (osocialax != 0 && osocialax != 1);


    setNombre(&nuevoPac, nombreax);
    setApellido(&nuevoPac, apellidoax);
    setDni(&nuevoPac, dniax);
    setOsocial(&nuevoPac, osocialax);

    insert_listaE(lp, nuevoPac);
    printf("¡Paciente cargado con exito!\n");

}

// ==========================================================================================
// Eliminar paciente por teclado (Validando que no tenga turnos)
// ==========================================================================================
void eliminar_paciente(ListaPacientesE *lp, ListaTurnoVO *lt) {
    if (isempty_listaE(*lp)) {
        printf("No hay pacientes registrados para eliminar.\n");
        return;
    }

    long dni_buscado;
    printf("Ingrese el DNI del paciente a eliminar: ");
    scanf("%ld", &dni_buscado);

    int encontrado = 0;
    reset_listaE(lp);
    while (!isOos_listaE(*lp)) {
        Paciente p = copy_listaE(*lp);
        if (getDni(p) == dni_buscado) {
            encontrado = 1;
            break;
        }
        forward_listaE(lp);
    }

    if (encontrado == 0) {
        printf("El paciente con DNI %ld no existe en el sistema.\n", dni_buscado);
        return;
    }

    // Validacion cruzada: revisar si el DNI esta en la lista de turnos
    int tiene_turnos = 0;
    reset_listaVO(lt);
    while (!isOos_listaVO(lt)) {
        Turno t = copy_listaVO(lt);
        if (getPaciente_dni(t) == dni_buscado) {
            tiene_turnos = 1;
            break;
        }
        forward_listaVO(lt);
    }

    if (tiene_turnos == 1) {
        printf("ERROR: No se puede eliminar el paciente. Tiene turnos pendientes o en historial.\n");
    } else {
        supress_listaE(lp);
        printf("Paciente eliminado exitosamente.\n");
    }
}

// ==========================================================================================
// Mostrar datos de un paciente segun DNI
// ==========================================================================================
void M_datos_paciente_dni(ListaPacientesE lp) {
    if (isempty_listaE(lp)) {
        printf("No hay pacientes cargados.\n");
        return;
    }

    long dni_buscado;
    printf("Ingrese el DNI a buscar: ");
    scanf("%ld", &dni_buscado);

    int encontrado = 0;
    reset_listaE(&lp);
    while (!isOos_listaE(lp)) {
        Paciente p = copy_listaE(lp);
        if (getDni(p) == dni_buscado) {


            char *nom = getNombre(p);
            char *ape = getApellido(p);

            printf("\n--- DATOS DEL PACIENTE ---\n");
            printf("DNI: %ld\n", getDni(p));
            printf("Nombre: %s %s\n", nom, ape);
            printf("Obra Social: %s\n", getOsocial(p) == 1 ? "SI (1)" : "NO (0)");


            free(nom);
            free(ape);

            encontrado = 1;
            break;
        }
        forward_listaE(&lp);
    }

    if (encontrado == 0) {
        printf("No se encontro ningun paciente con el DNI %ld.\n", dni_buscado);
    }
}

// ==========================================================================================
// Mostrar todos los pacientes
// ==========================================================================================
void mostrar_todos_los_pacientes(ListaPacientesE lp) {
    if (isempty_listaE(lp)) {
        printf("No hay pacientes cargados.\n");
        return;
    }

    printf("\n--- LISTA COMPLETA DE PACIENTES ---\n");
    reset_listaE(&lp);
    int contador = 1;

    while (!isOos_listaE(lp)) {
        Paciente p = copy_listaE(lp);


        char *nom = getNombre(p);
        char *ape = getApellido(p);

        printf("%d. DNI: %ld | Nombre: %s %s | Obra Social: %s\n",
               contador, getDni(p), nom, ape, getOsocial(p) == 1 ? "SI" : "NO");


        free(nom);
        free(ape);

        forward_listaE(&lp);
        contador++;
    }
}

void solicitar_turno(ListaTurnoVO *lt, ListaPacientesE *lp, Fecha agenda[]) {
    int mes, dia, hora, minuto, disponible;
    Fecha Tfec;
    long dni;

    printf("--- SOLICITAR TURNO ---\n");
    printf("Ingrese DNI del paciente: ");
    scanf("%ld", &dni);

    //Para verificar si esta en la lista de pacientes

    int existe = 0;
    reset_listaE(lp);
    while (!isOos_listaE(*lp)) {
        Paciente p = copy_listaE(*lp);
        if (getDni(p) == dni) {
            existe = 1;
            break;
        }
        forward_listaE(lp);
    }
    if (!existe) {
        printf("El paciente no esta registrado. Carguelo a continuacion:\n");
        cargar_paciente(lp);
    }
    carga_fecha(Tfec);
    //BUSCAR ID PARA COLOCAR EN TURNO

    int id_encontrado = -1;
    int i;
    for ( i = 0; i < Maxarr; i++) {
        if ((getMes(agenda[i]) == mes) && (getDia(agenda[i]) == dia) && (getHora(agenda[i]) == hora) &&(getMinuto(agenda[i]) == minuto)) {

            if (getDisponible(agenda[i]) == 1) {
                id_encontrado = i;
            } else {
                printf("Error: Ese turno ya esta ocupado\n");
                return;
            }
            break;
        }
    }
    if (id_encontrado == -1) {
        printf("Error: Fecha invalida en la agenda\n");
        return;
    }

// CREAR EL TURNO

    Turno nuevoTurno;
    setId(&nuevoTurno, id_encontrado);
    setPaciente(&nuevoTurno, dni);
    setFechaTurno(&nuevoTurno, dia, mes, 2026, hora, minuto, 0); // Copia fecha
    setEstado(&nuevoTurno, 1); // Estado: 1 = Solicitado
    setMonto(&nuevoTurno, 0.0);
    setMutual(&nuevoTurno, 0);

    setDisponible(&agenda[id_encontrado], 0);


    insert_listaVO(lt, nuevoTurno);
    printf(" Turno solicitado con exito! Asignado ID Turno: %d\n", id_encontrado);
}


void carga_fecha (Fecha fecha_turno){
int mes, dia, hora, minuto, disponible;
initFecha(&fecha_turno);

    printf("Ingrese la fecha del turno\n");
do {
    printf("Ingrese mes (11 o 12)\n");
    scanf("%d", &mes);
    }
while (mes != 11 && mes != 12);

    setMes(&fecha_turno, mes);

do {
    printf("Ingrese dia\n");
    scanf("%d", &dia);
        if ((mes==11 &&(dia>=1 || dia<=30)) || (mes==12 && (dia>=1 || dia<=31)))
        {
            setDia(&fecha_turno, dia);
        }
        else
        {
            printf("Noviembre tiene 30 dias y Diciembre tiene 31 dias\n");
        }
    }
while (dia<1 || dia>31);

do {
    printf("Ingrese hora\n");
    scanf("%d", &hora);
        if (hora>=8 && hora<=12)
        {
            setHora(&fecha_turno, hora);
        }
        else
        {
            printf("Los turnos se dan de 8:00hs a 12:30hs\n");
        }
    }
while (hora<8 || hora>12);

do {
    printf("Ingrese minuto (Los turnos se dan cada 30 minutos)\n");
    scanf("%d", &minuto);
    }
while (minuto != 0 && minuto != 30);

    setMinuto(&fecha_turno, minuto);

disponible = 0;
setDisponible(&fecha_turno, disponible);

return;
}




int main(){
    Fecha AGENDA[Maxarr];
    ListaPacientesE LPacientes;
    ListaTurnoVO LTurnoVO;

    // Tareas OBLIGATORIAS al arrancar el programa
    inicializar_agenda(AGENDA);
    init_listaE(&LPacientes);
    init_listaVO(&LTurnoVO);
    int op1;
    int op2 = -1;

    do {
        printf("\n==== SISTEMA DE TURNOS MEDICOS 2026 ====\n\n");
        printf("(1). Gestion de Agenda \n");
        printf("(2). Gestion de Pacientes \n");
        printf("(3). Gestion de Turnos \n");
        printf("(4). Recursividad y Reportes \n");
        printf("(0). Salir\n");
        printf("Selecione una opcion: ");
        scanf("%d", &op1);
        printf("\n");

        switch(op1) {
            case 1: {
                do {
                    printf("\n=== MENU ===\n== Gestion de Agenda == \n\n");
                    printf("(1). Busca Fecha libre en la agenda \n");
                    printf("(2). Busca fecha ocupada en agenda \n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2) {
                        case 1:

                            buscar_fecha_libre(AGENDA);


                            break;
                        case 2:
                            buscar_fecha_Ocupada(AGENDA);
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcion invalida.\n");
                    }
                } while (op2 != 0);
                break;
            }

            case 2: {
                do {
                    printf("\n=== MENU ===\n== Gestion de Pacientes == \n\n");
                    printf("(1). Cargar paciente por teclado \n");
                    printf("(2). Eliminar paciente por teclado \n");
                    printf("(3). Mostrar datos de un paciente segun DNI\n");
                    printf("(4). Mostrar todos los pacientes\n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2){
                        case 1:{cargar_paciente(&LPacientes);break;}
                        case 2:{eliminar_paciente(&LPacientes, &LTurnoVO); break;}
                        case 3:{M_datos_paciente_dni(LPacientes);break;}
                        case 4:{mostrar_todos_los_pacientes(LPacientes);break;}
                        case 0: break;
                    }
                } while (op2 != 0);
                break;
            }

            case 3: {
                do {
                    printf("\n=== MENU ===\n== Gestion de Turnos == \n\n");
                    printf("(1). Solicitar un turno\n");
                    printf("(2). Mostrar turnos de un paciente por DNI\n");
                    printf("(3). Mostrar datos del turno buscado.\n");
                    printf("(4). Confirmar el turno \n");
                    printf("(5). Cancelar turno\n");
                    printf("(6). Proximo paciente\n");
                    printf("(7). Mostrar todos los turnos\n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2) {

                        case 1: solicitar_turno(&LTurnoVO,&LPacientes,AGENDA); break;
                        case 2: break;
                        case 3: break;
                        case 4: break;
                        case 5: break;
                        case 6: break;
                        case 7: break;
                        case 0: break;
                        default: printf("Opcion invalida.\n");
                    }
                } while (op2 != 0);
                break;
            }

            case 4: {
                do {
                    printf("\n=== MENU ===\n== Recursividad y Reportes == \n\n");
                    printf("(1). Mostrar turnos asignados por mes\n");
                    printf("(2). Contar turnos atendidos por dia y mes\n");
                    printf("(3). Calcular el monto ingresado en un mes dado\n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2){
                        case 1: break;
                        case 2: break;
                        case 3: break;
                        case 0: break;
                        default: printf("Opcion invalida.\n");
                    }
                } while (op2 != 0);
                break;
            }

            case 0:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }
    } while (op1 != 0);
 //free(LPacientes);
// free(LTurnoVO);
    return 0;
}

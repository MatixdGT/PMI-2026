#include <stdio.h>
#include <stdlib.h>
#include "fecha.h"
#include "paciente.h"
#include "turno.h"
#include "listaE_Paciente.h"
#include "listaVO_Turno.h"

#define Maxarr 390

const char* obtener_nombre_estado(int estado) {
    switch(estado) {
        case 1: return "Solicitado";
        case 2: return "Confirmado";
        case 3: return "Atendido";
        case 4: return "Cancelado";
        default: return "Desconocido";
    }
}

int es_laborable(int dia, int mes) {
    int offset = (mes == 11) ? 0 : 2; 
    int dia_semana = (dia - 1 + offset) % 7;
    if (dia_semana == 0 || dia_semana == 6) return 0; 
    if (mes == 11 && dia == 9) return 0;  
    if (mes == 11 && dia == 23) return 0; 
    if (mes == 12 && dia == 7) return 0;  
    if (mes == 12 && dia == 8) return 0;  
    if (mes == 12 && dia == 24) return 0; 
    if (mes == 12 && dia == 25) return 0; 
    if (mes == 12 && dia == 31) return 0; 
    return 1; 
}

void inicializar_agenda(Fecha agenda[]) {
    int i = 0, mes, dia, hora, minuto;
    for (mes = 11; mes <= 12; mes++) {
        int limite_dias = (mes == 11) ? 30 : 31;
        for (dia = 1; dia <= limite_dias; dia++) {
            if (es_laborable(dia, mes) == 0) continue;
            for (hora = 8; hora < 13; hora++) {
                for (minuto = 0; minuto <= 30; minuto += 30) {
                    if (i < Maxarr) {
                        setFecha(&agenda[i], dia, mes, 2026, hora, minuto, 1);
                        i++;
                    }
                }
            }
        }
    }
    printf("-> [SISTEMA] Agenda inicializada con %d turnos laborables.\n", i);
}

void buscar_fecha_libre(Fecha agenda[]) {
    int dia_buscado, mes_buscado, encontrados = 0, i;
    printf("Ingrese el MES (11 o 12): "); scanf("%d", &mes_buscado);
    printf("Ingrese el DIA: "); scanf("%d", &dia_buscado);

    printf("\n--- TURNOS LIBRES EL %02d/%02d/2026 ---\n", dia_buscado, mes_buscado);
    for (i = 0; i < Maxarr; i++) {
        if (getMes(agenda[i]) == mes_buscado && getDia(agenda[i]) == dia_buscado && getDisponible(agenda[i]) == 1) {
            printf("ID Turno: %d | Horario: %02d:%02d hs\n", i, getHora(agenda[i]), getMinuto(agenda[i]));
            encontrados++;
        }
    }
    if (encontrados == 0) printf("No hay turnos libres para la fecha seleccionada.\n");
}

void buscar_fecha_Ocupada(Fecha agenda[]) {
    int mes_buscado, encontrados = 0, i;
    printf("Ingrese el MES a buscar (11 o 12): ");
    scanf("%d", &mes_buscado);
    while (mes_buscado != 11 && mes_buscado != 12) {
        printf("Error. El mes debe ser 11 o 12: "); scanf("%d", &mes_buscado);
    }

    printf("\n--- TURNOS OCUPADOS EL %02d/2026 ---\n", mes_buscado);
    for (i = 0; i < Maxarr; i++) {
        if (getMes(agenda[i]) == mes_buscado && getDisponible(agenda[i]) == 0) {
            printf("================\n ID Turno: %d \n Horario: %02d:%02d hs\n Dia: %02d\n================\n\n",
                   i, getHora(agenda[i]), getMinuto(agenda[i]), getDia(agenda[i]));
            encontrados++;
        }
    }
    if (encontrados == 0) printf("No hay turnos Ocupados para el mes seleccionado.\n");
}

void cargar_paciente(ListaPacientesE *lp) {
    int continuar;
    do {
        if (isfull_listaE(*lp)) {
            printf("Error: La lista de pacientes esta llena.\n");
            return;
        }

        Paciente nuevoPac;
        char nombreax[20], apellidoax[20];
        long dniax; int osocialax;

        printf("\n--- CARGAR NUEVO PACIENTE ---\n");
        printf("Ingrese el Nombre: "); scanf(" %[^\n]", nombreax);
        printf("Ingrese el Apellido: "); scanf(" %[^\n]", apellidoax);
        
        printf("Ingrese el DNI: "); scanf("%ld", &dniax);
        while (dniax <= 0) {
            printf("DNI invalido. Ingrese nuevamente: "); scanf("%ld", &dniax);
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

        printf("\n¿Desea seguir cargando otro paciente? (1=Si, 0=No): ");
        scanf("%d", &continuar);
    } while (continuar == 1);
}

void eliminar_paciente(ListaPacientesE *lp, ListaTurnoVO *lt) {
    if (isempty_listaE(*lp)) {
        printf("No hay pacientes registrados.\n");
        return;
    }
    long dni_buscado;
    printf("Ingrese el DNI a eliminar: "); scanf("%ld", &dni_buscado);

    int encontrado = 0, tiene_turnos = 0;
    reset_listaE(lp);
    while (!isOos_listaE(*lp)) {
        Paciente p = copy_listaE(*lp);
        if (getDni(p) == dni_buscado) { encontrado = 1; break; }
        forward_listaE(lp);
    }

    if (!encontrado) {
        printf("Paciente inexistente.\n"); return;
    }

    reset_listaVO(lt);
    while (!isOos_listaVO(*lt)) {
        Turno t = copy_listaVO(*lt);
        if (getPaciente_dni(t) == dni_buscado) { tiene_turnos = 1; break; }
        forward_listaVO(lt);
    }

    if (tiene_turnos) {
        printf("ERROR: No se puede eliminar. Tiene turnos asociados.\n");
    } else {
        supress_listaE(lp);
        printf("Paciente eliminado exitosamente.\n");
    }
}

void mostrar_paciente_dni(ListaPacientesE lp) {
    if (isempty_listaE(lp)) { printf("No hay pacientes cargados.\n"); return; }
    long dni_buscado;
    printf("Ingrese el DNI a buscar: "); scanf("%ld", &dni_buscado);

    int encontrado = 0;
    reset_listaE(&lp);
    while (!isOos_listaE(lp)) {
        Paciente p = copy_listaE(lp);
        if (getDni(p) == dni_buscado) {
            char *nom = getNombre(p), *ape = getApellido(p);
            printf("\n===============\n DNI: %ld\n Nombre: %s %s\n Obra Social: %s\n===============\n", 
                   getDni(p), nom, ape, getOsocial(p) == 1 ? "SI" : "NO");
            free(nom); free(ape);
            encontrado = 1; break;
        }
        forward_listaE(&lp);
    }
    if (!encontrado) printf("Paciente no encontrado.\n");
}

void mostrar_todos_pacientes(ListaPacientesE lp) {
    if (isempty_listaE(lp)) { printf("No hay pacientes cargados.\n"); return; }
    printf("\n--- LISTA COMPLETA DE PACIENTES ---\n");
    reset_listaE(&lp);
    int c = 1;
    while (!isOos_listaE(lp)) {
        Paciente p = copy_listaE(lp);
        char *nom = getNombre(p), *ape = getApellido(p);
        printf("%d. DNI: %ld | %s %s | OS: %s\n", c++, getDni(p), nom, ape, getOsocial(p) == 1 ? "SI" : "NO");
        free(nom); free(ape);
        forward_listaE(&lp);
    }
}

void carga_fecha(Fecha *fecha_turno) {
    int mes, dia, hora, minuto;
    initFecha(fecha_turno);
    printf("Ingrese mes (11 o 12): "); scanf("%d", &mes); setMes(fecha_turno, mes);
    printf("Ingrese dia: "); scanf("%d", &dia); setDia(fecha_turno, dia);
    printf("Ingrese hora (8 a 12): "); scanf("%d", &hora); setHora(fecha_turno, hora);
    printf("Ingrese minuto (0 o 30): "); scanf("%d", &minuto); setMinuto(fecha_turno, minuto);
    setDisponible(fecha_turno, 0);
}

void solicitar_turno(ListaTurnoVO *lt, ListaPacientesE *lp, Fecha agenda[]) {
    int continuar_turno;
    do {
        int opcion_fecha = 1;
        do {
            printf("\n--- BUSQUEDA DE FECHAS LIBRES ---\n");
            buscar_fecha_libre(agenda);
            printf("\n¿Buscar otra fecha? (1.Si / 2.Continuar): "); scanf("%d", &opcion_fecha);
        } while (opcion_fecha == 1);

        if (opcion_fecha != 2) return;

        Fecha Tfec; long dni;
        printf("\nIngrese DNI del paciente: "); scanf("%ld", &dni);

        int existe = 0; reset_listaE(lp);
        while (!isOos_listaE(*lp)) {
            if (getDni(copy_listaE(*lp)) == dni) { existe = 1; break; }
            forward_listaE(lp);
        }
        if (!existe) cargar_paciente(lp);

        carga_fecha(&Tfec);

        int id_encontrado = -1, i;
        for (i = 0; i < Maxarr; i++) {
            if (getMes(agenda[i]) == getMes(Tfec) && getDia(agenda[i]) == getDia(Tfec) &&
                getHora(agenda[i]) == getHora(Tfec) && getMinuto(agenda[i]) == getMinuto(Tfec)) {
                if (getDisponible(agenda[i]) == 1) id_encontrado = i;
                break;
            }
        }

        if (id_encontrado != -1) {
            Turno nuevoTurno;
            setId(&nuevoTurno, id_encontrado);
            setPaciente(&nuevoTurno, dni);
            setFechaTurno(&nuevoTurno, getDia(Tfec), getMes(Tfec), 2026, getHora(Tfec), getMinuto(Tfec), 0);
            setEstado(&nuevoTurno, 1); 
            setMonto(&nuevoTurno, 0.0); setMutual(&nuevoTurno, 0);

            setDisponible(&agenda[id_encontrado], 0);
            insert_listaVO(lt, nuevoTurno);
            printf("\n¡Turno solicitado! ID Turno: %d\n", id_encontrado);
        } else {
            printf("Error: Fecha invalida o ya ocupada\n");
        }
        printf("\n¿Solicitar otro turno? (1=Si, 0=No): "); scanf("%d", &continuar_turno);
    } while (continuar_turno == 1);
}

int busca_turno_id(ListaTurnoVO *lt, int id_buscado) {
    if (isempty_listaVO(*lt)) return 0;
    reset_listaVO(lt);
    while (!isOos_listaVO(*lt)) {
        if (getId(copy_listaVO(*lt)) == id_buscado) return 1;
        forward_listaVO(lt);
    }
    return 0;
}

void elimina_turno_cancelado(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) return;
    FILE *arch = fopen("turno_cancelado.txt", "a");
    if (arch == NULL) return;

    reset_listaVO(lt);
    while (!isOos_listaVO(*lt)) {
        Turno t = copy_listaVO(*lt);
        if (getEstado(t) == 4) { 
            Fecha f = getFechaTurno(t);
            fprintf(arch, "ID: %d | DNI: %ld | Fecha: %02d/%02d/%d | Hora: %02d:%02d\n",
                    getId(t), getPaciente_dni(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f));
            supress_listaVO(lt);
        } else {
            forward_listaVO(lt);
        }
    }
    fclose(arch);
}

void muestra_turno_dni(ListaTurnoVO lt) {
    if (isempty_listaVO(lt)) { printf("No hay turnos.\n"); return; }
    long dni_buscado;
    printf("Ingrese el DNI: "); scanf("%ld", &dni_buscado);
    int encontrados = 0;
    reset_listaVO(&lt);
    while (!isOos_listaVO(lt)) {
        Turno t = copy_listaVO(lt);
        if (getPaciente_dni(t) == dni_buscado) {
            Fecha f = getFechaTurno(t);
            printf("ID Turno: %d | Fecha: %02d/%02d/%d - %02d:%02d | Estado: %s\n",
                   getId(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));
            encontrados++;
        }
        forward_listaVO(&lt);
    }
    if (!encontrados) printf("No hay turnos para este DNI.\n");
}

void muestra_turno_buscado(ListaTurnoVO lt) {
    if (isempty_listaVO(lt)) { printf("No hay turnos.\n"); return; }
    int id_buscado;
    printf("Ingrese ID del turno: "); scanf("%d", &id_buscado);
    if (busca_turno_id(&lt, id_buscado)) {
        Turno t = copy_listaVO(lt); Fecha f = getFechaTurno(t);
        printf("\nID: %d | DNI: %ld | %02d/%02d/%d - %02d:%02d | Estado: %s\n",
               getId(t), getPaciente_dni(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));
    } else {
        printf("Turno no existe.\n");
    }
}

void confirmar_turno(ListaTurnoVO *lt, ListaPacientesE lp) {
    if (isempty_listaVO(*lt)) { printf("No hay turnos.\n"); return; }
    int id_buscado;
    printf("Ingrese ID del turno a confirmar: "); scanf("%d", &id_buscado);

    if (busca_turno_id(lt, id_buscado)) {
        Turno t = copy_listaVO(*lt);
        if (getEstado(t) != 1) { printf("Error: Solo se confirman turnos 'Solicitados'.\n"); return; }
        
        long dni_p = getPaciente_dni(t); int tiene_os = 0;
        reset_listaE(&lp);
        while (!isOos_listaE(lp)) {
            if (getDni(copy_listaE(lp)) == dni_p) { tiene_os = getOsocial(copy_listaE(lp)); break; }
            forward_listaE(&lp);
        }

        if (tiene_os == 1) {
            int mutual;
            printf("Mutual (1.OSDE, 2.PAMI, 3.OCECAC, 4.DOSPU): "); scanf("%d", &mutual);
            setMutual(&t, mutual); setMonto(&t, 0.0);
        } else {
            float monto;
            printf("Monto cobrado: $"); scanf("%f", &monto);
            setMonto(&t, monto); setMutual(&t, 0);
        }

        setEstado(&t, 2); 
        supress_listaVO(lt); insert_listaVO(lt, t);
        printf("¡Turno CONFIRMADO!\n");
    } else {
        printf("Turno no encontrado.\n");
    }
}

void cancela_turno(ListaTurnoVO *lt, Fecha agenda[]) {
    if (isempty_listaVO(*lt)) { printf("No hay turnos.\n"); return; }
    int id_buscado;
    printf("Ingrese ID del turno a cancelar: "); scanf("%d", &id_buscado);

    if (busca_turno_id(lt, id_buscado)) {
        Turno t = copy_listaVO(*lt);
        if (getEstado(t) == 3) { printf("Error: Ya fue ATENDIDO.\n"); return; }

        setEstado(&t, 4); 
        supress_listaVO(lt); insert_listaVO(lt, t);
        setDisponible(&agenda[id_buscado], 1); 

        printf("Turno cancelado.\n");
        elimina_turno_cancelado(lt);
    } else {
        printf("Turno no existe.\n");
    }
}

void proximo_paciente(ListaTurnoVO *lt, Fecha agenda[]) {
    if (isempty_listaVO(*lt)) { printf("No hay turnos.\n"); return; }
    int mes, dia, continuar = 0;
    printf("Mes (11 o 12): "); scanf("%d", &mes);
    printf("Dia actual: "); scanf("%d", &dia);
    
    do {
        reset_listaVO(lt);
        int encontrado = 0;
        while (!isOos_listaVO(*lt)) {
            Turno t = copy_listaVO(*lt); Fecha f = getFechaTurno(t);
            if (getMes(f) == mes && getDia(f) == dia && getEstado(t) == 2) {
                encontrado = 1;
                printf("\n=== PROXIMO PACIENTE ===\nID: %d | DNI: %ld | %02d:%02d\n", getId(t), getPaciente_dni(t), getHora(f), getMinuto(f));
                int presencia;
                printf("¿Presente? (1=Si, 0=No): "); scanf("%d", &presencia);

                if (presencia == 1) {
                    setEstado(&t, 3); 
                    supress_listaVO(lt); insert_listaVO(lt, t);
                    printf("ATENDIDO.\n");
                } else {
                    setEstado(&t, 4); 
                    supress_listaVO(lt); insert_listaVO(lt, t);
                    setDisponible(&agenda[getId(t)], 1);
                    elimina_turno_cancelado(lt);
                }
                break; 
            }
            forward_listaVO(lt);
        }
        if (!encontrado) {
            printf("No hay mas confirmados hoy.\n"); break; 
        }
        printf("\n¿Llamar al siguiente? (1=Si, 0=No): "); scanf("%d", &continuar);
    } while (continuar == 1);
}

void muestra_turnos(ListaTurnoVO lt) {
    if (isempty_listaVO(lt)) { printf("No hay turnos.\n"); return; }
    reset_listaVO(&lt);
    int contador = 0, pagina = 1;
    printf("\n--- TURNOS (PAG %d) ---\n", pagina);
    while (!isOos_listaVO(lt)) {
        Turno t = copy_listaVO(lt); Fecha f = getFechaTurno(t);
        printf("ID: %3d | DNI: %ld | %02d/%02d/%d %02d:%02d | %s\n",
               getId(t), getPaciente_dni(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));
        contador++;
        if (contador % 5 == 0 && !isOos_listaVO(lt)) {
            printf("\nENTER para siguiente pagina..."); getchar(); getchar();
            printf("\n--- TURNOS (PAG %d) ---\n", ++pagina);
        }
        forward_listaVO(&lt);
    }
}

void mostrar_turnos_mes_rec(ListaTurnoVO *lt, int mes_buscado) {
    if (isOos_listaVO(*lt)) return;
    Turno t = copy_listaVO(*lt); Fecha f = getFechaTurno(t);
    // Filtro por Solicitado (1) o Confirmado (2)
    if (getMes(f) == mes_buscado && (getEstado(t) == 1 || getEstado(t) == 2)) {
        printf("ID: %d | %02d/%02d/%d %02d:%02d | %s\n",
               getId(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));
    }
    forward_listaVO(lt);
    mostrar_turnos_mes_rec(lt, mes_buscado);
}

void mostrar_turnos_mes(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) { printf("Vacio.\n"); return; }
    int mes; printf("Ingrese mes: "); scanf("%d", &mes);
    printf("\n--- ASIGNADOS MES %d ---\n", mes);
    reset_listaVO(lt);
    mostrar_turnos_mes_rec(lt, mes);
}

int contar_atendidos_rec(ListaTurnoVO *lt, int dia, int mes) {
    if (isOos_listaVO(*lt)) return 0;
    Turno t = copy_listaVO(*lt); Fecha f = getFechaTurno(t);
    int count = (getDia(f) == dia && getMes(f) == mes && getEstado(t) == 3) ? 1 : 0;
    forward_listaVO(lt);
    return count + contar_atendidos_rec(lt, dia, mes);
}

void contar_atendidos(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) return;
    int dia, mes;
    printf("Mes (11 o 12): "); scanf("%d", &mes);
    printf("Dia: "); scanf("%d", &dia);
    reset_listaVO(lt);
    printf("\nAtendidos el %02d/%02d: %d\n", dia, mes, contar_atendidos_rec(lt, dia, mes));
}

float calcular_monto_rec(ListaTurnoVO *lt, int mes) {
    if (isOos_listaVO(*lt)) return 0.0;
    Turno t = copy_listaVO(*lt); Fecha f = getFechaTurno(t);
    float val = (getMes(f) == mes && getEstado(t) == 3) ? getMonto(t) : 0.0;
    forward_listaVO(lt);
    return val + calcular_monto_rec(lt, mes);
}

void calcular_monto_mes(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) return;
    int mes; printf("Mes a facturar: "); scanf("%d", &mes);
    reset_listaVO(lt);
    printf("\nRecaudacion mes %d: $%.2f\n", mes, calcular_monto_rec(lt, mes));
}

void guardar_sistema(ListaPacientesE lp, ListaTurnoVO lt, Fecha agenda[]) {
    FILE *archP = fopen("pacientes.txt", "w");
    if (archP != NULL) {
        reset_listaE(&lp);
        while (!isOos_listaE(lp)) {
            Paciente p = copy_listaE(lp);
            char *nom = getNombre(p), *ape = getApellido(p);
            fprintf(archP, "%ld %s %s %d\n", getDni(p), nom, ape, getOsocial(p));
            free(nom); free(ape); forward_listaE(&lp);
        } fclose(archP);
    }

    FILE *archT = fopen("turnos.txt", "w");
    if (archT != NULL) {
        reset_listaVO(&lt);
        while (!isOos_listaVO(lt)) {
            Turno t = copy_listaVO(lt); Fecha f = getFechaTurno(t);
            fprintf(archT, "%d %ld %d %d %d %d %d %d %d %.2f\n", getId(t), getPaciente_dni(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), getEstado(t), getMutual(t), getMonto(t));
            forward_listaVO(&lt);
        } fclose(archT);
    }
    
    FILE *archA = fopen("agenda.txt", "w");
    if (archA != NULL) {
        for(int i = 0; i < Maxarr; i++) {
            fprintf(archA, "%d %d %d %d %d %d %d\n", i, getDia(agenda[i]), getMes(agenda[i]), getAnio(agenda[i]), getHora(agenda[i]), getMinuto(agenda[i]), getDisponible(agenda[i]));
        } fclose(archA);
    }
    printf("-> Backup de Pacientes, Turnos y Agenda completado.\n");
}

void cargar_sistema_manual(ListaPacientesE *lp, ListaTurnoVO *lt, Fecha agenda[]) {
    int cargados = 0;
    FILE *archA = fopen("agenda.txt", "r");
    if (archA != NULL) {
        int id, d, m, a, h, min, disp;
        while (fscanf(archA, "%d %d %d %d %d %d %d", &id, &d, &m, &a, &h, &min, &disp) == 7) {
            setFecha(&agenda[id], d, m, a, h, min, disp);
        } fclose(archA); cargados = 1; printf("-> Agenda cargada.\n");
    }

    FILE *archP = fopen("pacientes.txt", "r");
    if (archP != NULL) {
        Paciente p; long dni; char nom[20], ape[20]; int os;
        while (fscanf(archP, "%ld %s %s %d", &dni, nom, ape, &os) == 4) {
            setDni(&p, dni); setNombre(&p, nom); setApellido(&p, ape); setOsocial(&p, os); insert_listaE(lp, p);
        } fclose(archP); printf("-> Pacientes cargados.\n");
    }

    FILE *archT = fopen("turnos.txt", "r");
    if (archT != NULL) {
        Turno t; int id, dia, mes, anio, hora, min, est, mut; long dni; float monto;
        while (fscanf(archT, "%d %ld %d %d %d %d %d %d %d %f", &id, &dni, &dia, &mes, &anio, &hora, &min, &est, &mut, &monto) == 10) {
            setId(&t, id); setPaciente(&t, dni);
            setFechaTurno(&t, dia, mes, anio, hora, min, 0);
            setEstado(&t, est); setMutual(&t, mut); setMonto(&t, monto); insert_listaVO(lt, t);
        } fclose(archT); printf("-> Turnos cargados.\n");
    }
    if(!cargados) printf("No se encontro backup. Agenda por defecto activa.\n");
}

int main(){
    Fecha AGENDA[Maxarr];
    ListaPacientesE LPacientes;
    ListaTurnoVO LTurnoVO;

    inicializar_agenda(AGENDA);
    init_listaE(&LPacientes);
    init_listaVO(&LTurnoVO);
    
    // NOTA: Se elimino la carga automatica. Ahora se hace desde el Menu opcion 5 (Segun PDF).

    int op1, op2 = -1;
    do {
        printf("\n==== SISTEMA DE TURNOS MEDICOS 2026 ====\n");
        printf("(1). Gestion de Agenda \n(2). Gestion de Pacientes \n(3). Gestion de Turnos \n(4). Recursividad y Reportes \n(5). Recuperar Backup Manualmente\n(0). Salir\nSelecione una opcion: ");
        scanf("%d", &op1);

        switch(op1) {
            case 1: 
                do {
                    printf("\n== Gestion de Agenda == \n(1). Busca Fecha libre \n(2). Busca fecha ocupada \n(0). Salir \nElige: ");
                    scanf("%d", &op2);
                    switch(op2) { case 1: buscar_fecha_libre(AGENDA); break; case 2: buscar_fecha_Ocupada(AGENDA); break; }
                } while (op2 != 0); break;
            case 2: 
                do {
                    printf("\n== Gestion de Pacientes == \n(1). Cargar paciente \n(2). Eliminar paciente \n(3). Mostrar datos DNI \n(4). Mostrar todos\n(0). Salir \nElige: ");
                    scanf("%d", &op2);
                    switch(op2){ case 1: cargar_paciente(&LPacientes); break; case 2: eliminar_paciente(&LPacientes, &LTurnoVO); break; case 3: mostrar_paciente_dni(LPacientes); break; case 4: mostrar_todos_pacientes(LPacientes); break;}
                } while (op2 != 0); break;
            case 3: 
                do {
                    printf("\n== Gestion de Turnos == \n(1). Solicitar \n(2). Mostrar por DNI\n(3). Mostrar por ID\n(4). Confirmar \n(5). Cancelar\n(6). Proximo paciente\n(7). Mostrar todos\n(0). Salir \nElige: ");
                    scanf("%d", &op2);
                    switch(op2) { case 1: solicitar_turno(&LTurnoVO, &LPacientes, AGENDA); break; case 2: muestra_turno_dni(LTurnoVO); break; case 3: muestra_turno_buscado(LTurnoVO); break; case 4: confirmar_turno(&LTurnoVO, LPacientes); break; case 5: cancela_turno(&LTurnoVO, AGENDA); break; case 6: proximo_paciente(&LTurnoVO, AGENDA); break; case 7: muestra_turnos(LTurnoVO); break; }
                } while (op2 != 0); break;
            case 4: 
                do {
                    printf("\n== Recursividad y Reportes == \n(1). Turnos asignados por mes\n(2). Contar atendidos\n(3). Calcular monto mensual\n(0). Salir \nElige: ");
                    scanf("%d", &op2);
                    switch(op2){ case 1: mostrar_turnos_mes(&LTurnoVO); break; case 2: contar_atendidos(&LTurnoVO); break; case 3: calcular_monto_mes(&LTurnoVO); break;}
                } while (op2 != 0); break;
            case 5:
                printf("\n--- RECUPERACION DE BACKUP ---\n");
                cargar_sistema_manual(&LPacientes, &LTurnoVO, AGENDA);
                break;
            case 0:
                printf("\nGuardando informacion...\n");
                guardar_sistema(LPacientes, LTurnoVO, AGENDA);
                break;
        }
    } while (op1 != 0);
    return 0;
}

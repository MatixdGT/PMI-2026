#include <stdio.h>
#include <stdlib.h>
#include "fecha.h"
#include "paciente.h"
#include "turno.h"
#include "listaE_Paciente.h"
#include "listaVO_Turno.h"

#define Maxarr 390

// ====================================================================
// FUNCION AUXILIAR: Traductor de Estados
// Convierte el codigo numerico (1, 2, 3, 4) a texto legible para el usuario
// ====================================================================
const char* obtener_nombre_estado(int estado) {
    switch(estado) {
        case 1: return "Solicitado";
        case 2: return "Confirmado";
        case 3: return "Atendido";
        case 4: return "Cancelado";
        default: return "Desconocido";
    }
}

// ====================================================================
// FUNCIONALIDAD: Filtro de dias laborables (Excluye fines de semana y feriados)
// ====================================================================
int es_laborable(int dia, int mes) {
    int offset = (mes == 11) ? 0 : 2; // Nov 2026 arranca en Domingo. Dic arranca en Martes.
    int dia_semana = (dia - 1 + offset) % 7;

    if (dia_semana == 0 || dia_semana == 6) return 0; // Sábados y Domingos

    // Feriados y Asuetos de Argentina 2026
    if (mes == 11 && dia == 23) return 0;
    if (mes == 12 && dia == 8) return 0;
    if (mes == 12 && dia == 24) return 0;
    if (mes == 12 && dia == 25) return 0;
    if (mes == 12 && dia == 31) return 0;

    return 1; // Día laborable
}

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
    printf("-> [SISTEMA] Agenda inicializada con %d turnos laborables exactos.\n", i);
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

// ====================================================================
// FUNCIONALIDAD (d): Busca fecha Ocupada en agenda segun mes
// ====================================================================
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
            printf("================\n ID Turno: %d \n Horario: %02d:%02d hs\n Dia: %02d\n================\n\n",
                   i, getHora(agenda[i]), getMinuto(agenda[i]), getDia(agenda[i]));
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No hay turnos Ocupados para el mes seleccionado.\n");
    }
}

// ====================================================================
// FUNCIONALIDAD: Cargar paciente por teclado
// ====================================================================
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
    scanf(" %[^\n]", nombreax);
    getchar();
    printf("Ingrese el Apellido: ");
    scanf(" %[^\n]", apellidoax);
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

// ====================================================================
// FUNCIONALIDAD: Eliminar paciente por teclado validando que no tenga turnos
// ====================================================================
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

    int tiene_turnos = 0;
    reset_listaVO(lt);
    while (!isOos_listaVO(*lt)) {
        Turno t = copy_listaVO(*lt);
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

// ====================================================================
// FUNCIONALIDAD: Mostrar datos de un paciente segun DNI
// ====================================================================
void mostrar_paciente_dni(ListaPacientesE lp) {
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
            printf("===============\n DNI: %ld\n", getDni(p));
            printf(" Nombre: %s %s\n", nom, ape);
            printf(" Obra Social: %s\n===============\n\n", getOsocial(p) == 1 ? "SI (1)" : "NO (0)");

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

// ====================================================================
// FUNCIONALIDAD: Mostrar todos los pacientes registrados
// ====================================================================
void mostrar_todos_pacientes(ListaPacientesE lp) {
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

        printf("==================\n %d. DNI: %ld \n Nombre: %s %s \n Obra Social: %s\n==================\n\n",
               contador, getDni(p), nom, ape, getOsocial(p) == 1 ? "SI" : "NO");

        free(nom);
        free(ape);

        forward_listaE(&lp);
        contador++;
    }
}

// ====================================================================
// FUNCIONALIDAD AUXILIAR: Carga de fecha para solicitar turno
// ====================================================================
void carga_fecha(Fecha *fecha_turno) {
    int mes, dia, hora, minuto, disponible;
    initFecha(fecha_turno);

    printf("Ingrese la fecha del turno\n");
    do {
        printf("Ingrese mes (11 o 12)\n");
        scanf("%d", &mes);
    } while (mes != 11 && mes != 12);

    setMes(fecha_turno, mes);

    do {
        printf("Ingrese dia\n");
        scanf("%d", &dia);
        if ((mes == 11 && (dia >= 1 && dia <= 30)) || (mes == 12 && (dia >= 1 && dia <= 31))) {
            setDia(fecha_turno, dia);
        } else {
            printf("Noviembre tiene 30 dias y Diciembre tiene 31 dias\n");
        }
    } while (dia < 1 || dia > 31);

    do {
        printf("Ingrese hora (8 a 12)\n");
        scanf("%d", &hora);
        if (hora >= 8 && hora <= 12) {
            setHora(fecha_turno, hora);
        } else {
            printf("Los turnos se dan de 8:00hs a 12:30hs\n");
        }
    } while (hora < 8 || hora > 12);

    do {
        printf("Ingrese minuto (0 o 30)\n");
        scanf("%d", &minuto);
    } while (minuto != 0 && minuto != 30);

    setMinuto(fecha_turno, minuto);

    disponible = 0;
    setDisponible(fecha_turno, disponible);
}

// ====================================================================
// FUNCIONALIDAD: Solicitar un turno médico
// ====================================================================
void solicitar_turno(ListaTurnoVO *lt, ListaPacientesE *lp, Fecha agenda[]) {
    Fecha Tfec;
    long dni;

    printf("--- SOLICITAR TURNO ---\n");
    printf("Ingrese DNI del paciente: ");
    scanf("%ld", &dni);

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

    carga_fecha(&Tfec);

    int id_encontrado = -1;
    int i;
    for (i = 0; i < Maxarr; i++) {
        if ((getMes(agenda[i]) == getMes(Tfec)) &&
            (getDia(agenda[i]) == getDia(Tfec)) &&
            (getHora(agenda[i]) == getHora(Tfec)) &&
            (getMinuto(agenda[i]) == getMinuto(Tfec))) {

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

    Turno nuevoTurno;
    setId(&nuevoTurno, id_encontrado);
    setPaciente(&nuevoTurno, dni);
    setFechaTurno(&nuevoTurno, getDia(Tfec), getMes(Tfec), 2026, getHora(Tfec), getMinuto(Tfec), 0);
    setEstado(&nuevoTurno, 1); // 1 = Solicitado
    setMonto(&nuevoTurno, 0.0);
    setMutual(&nuevoTurno, 0);

    setDisponible(&agenda[id_encontrado], 0);

    insert_listaVO(lt, nuevoTurno);
    printf(" Turno solicitado con exito! Asignado ID Turno: %d\n", id_encontrado);
}

// ====================================================================
// FUNCIONALIDAD AUXILIAR: Buscar turno por ID
// ====================================================================
int busca_turno_id(ListaTurnoVO *lt, int id_buscado) {
    if (isempty_listaVO(*lt)) return 0;
    reset_listaVO(lt);

    while (!isOos_listaVO(*lt)) {
        Turno t = copy_listaVO(*lt);
        if (getId(t) == id_buscado) {
            return 1;
        }
        forward_listaVO(lt);
    }
    return 0;
}

// ====================================================================
// FUNCIONALIDAD: Eliminar turno cancelado y registrarlo en archivo auxiliar
// ====================================================================
void elimina_turno_cancelado(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) return;

    FILE *arch = fopen("turno_cancelado.txt", "a");
    if (arch == NULL) {
        printf("Error al abrir turno_cancelado.txt\n");
        return;
    }

    reset_listaVO(lt);
    while (!isOos_listaVO(*lt)) {
        Turno t = copy_listaVO(*lt);
        if (getEstado(t) == 4) { // 4 = Cancelado
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

// ====================================================================
// FUNCIONALIDAD: Mostrar turnos de un paciente por DNI
// ====================================================================
void muestra_turno_dni(ListaTurnoVO lt) {
    if (isempty_listaVO(lt)) {
        printf("No hay turnos registrados en el sistema.\n");
        return;
    }

    long dni_buscado;
    printf("Ingrese el DNI del paciente a consultar: ");
    scanf("%ld", &dni_buscado);

    printf("\n--- TURNOS REGISTRADOS PARA EL DNI %ld ---\n", dni_buscado);
    int encontrados = 0;
    reset_listaVO(&lt);

    while (!isOos_listaVO(lt)) {
        Turno t = copy_listaVO(lt);
        if (getPaciente_dni(t) == dni_buscado) {
            Fecha f = getFechaTurno(t);
            printf("===============\n ID Turno: %d \n Fecha: %02d/%02d/%d\n Hora: %02d:%02d hs \n Estado: %s\n===============\n\n",
                   getId(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));
            encontrados++;
        }
        forward_listaVO(&lt);
    }

    if (encontrados == 0) {
        printf("No se encontraron turnos asignados a este DNI.\n");
    }
}

// ====================================================================
// FUNCIONALIDAD: Mostrar datos del turno buscado por ID
// ====================================================================
void muestra_turno_buscado(ListaTurnoVO lt) {
    if (isempty_listaVO(lt)) {
        printf("No hay turnos registrados en el sistema.\n");
        return;
    }

    int id_buscado;
    printf("Ingrese el ID del turno a consultar: ");
    scanf("%d", &id_buscado);

    if (busca_turno_id(&lt, id_buscado)) {
        Turno t = copy_listaVO(lt);
        Fecha f = getFechaTurno(t);
        printf("\n--- DETALLE DEL TURNO ID: %d ---\n", getId(t));
        printf("DNI Paciente: %ld\n", getPaciente_dni(t));
        printf("Fecha y Hora: %02d/%02d/%d - %02d:%02d hs\n",
               getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f));
        printf("Estado: %s\n==================\n", obtener_nombre_estado(getEstado(t)));

        if (getMutual(t) > 0) printf("Obra Social (Codigo): %d\n", getMutual(t));
        if (getMonto(t) > 0) printf("Monto Cobrado: $%.2f\n", getMonto(t));
    } else {
        printf("El turno con ID %d no existe.\n", id_buscado);
    }
}

// ====================================================================
// FUNCIONALIDAD: Confirmar el turno solicitado
// ====================================================================
void confirmar_turno(ListaTurnoVO *lt, ListaPacientesE lp) {
    if (isempty_listaVO(*lt)) {
        printf("No hay turnos para confirmar.\n");
        return;
    }

    int id_buscado;
    printf("Ingrese ID del turno a confirmar: ");
    scanf("%d", &id_buscado);

    if (busca_turno_id(lt, id_buscado)) {
        Turno t = copy_listaVO(*lt);

        if (getEstado(t) != 1) {
            printf("Error: Solo se pueden confirmar turnos 'Solicitados'.\n");
            return;
        }

        long dni_p = getPaciente_dni(t);
        int tiene_os = 0;

        reset_listaE(&lp);
        while (!isOos_listaE(lp)) {
            Paciente p = copy_listaE(lp);
            if (getDni(p) == dni_p) {
                tiene_os = getOsocial(p);
                break;
            }
            forward_listaE(&lp);
        }

        if (tiene_os == 1) {
            int mutual;
            printf("Ingrese la Mutual (1.OSDE, 2.PAMI, 3.OCECAC, 4.DOSPU): ");
            scanf("%d", &mutual);
            setMutual(&t, mutual);
            setMonto(&t, 0.0);
        } else {
            float monto;
            printf("Ingrese el Monto cobrado por la consulta: $");
            scanf("%f", &monto);
            setMonto(&t, monto);
            setMutual(&t, 0);
        }

        setEstado(&t, 2); // 2 = Confirmado

        supress_listaVO(lt);
        insert_listaVO(lt, t);

        printf("¡Turno ID %d CONFIRMADO exitosamente!\n", id_buscado);
    } else {
        printf("Error: No se encontro el turno solicitado.\n");
    }
}

// ====================================================================
// FUNCIONALIDAD: Cancelar turno y liberar agenda
// ====================================================================
void cancela_turno(ListaTurnoVO *lt, Fecha agenda[]) {
    if (isempty_listaVO(*lt)) {
        printf("No hay turnos para cancelar.\n");
        return;
    }

    int id_buscado;
    printf("Ingrese ID del turno a cancelar: ");
    scanf("%d", &id_buscado);

    if (busca_turno_id(lt, id_buscado)) {
        Turno t = copy_listaVO(*lt);

        if (getEstado(t) == 3) { // 3 = Atendido
            printf("Error: No se puede cancelar un turno que ya fue ATENDIDO.\n");
            return;
        }

        setEstado(&t, 4); // 4 = Cancelado

        supress_listaVO(lt);
        insert_listaVO(lt, t);

        setDisponible(&agenda[id_buscado], 1); // 1 = Libre

        printf("Turno ID %d marcado como cancelado.\n", id_buscado);

        elimina_turno_cancelado(lt);
    } else {
        printf("Error: No existe el turno con ID %d.\n", id_buscado);
    }
}

// ====================================================================
// FUNCIONALIDAD: Proximo paciente a atender
// ====================================================================
void proximo_paciente(ListaTurnoVO *lt, Fecha agenda[]) {
    if (isempty_listaVO(*lt)) {
        printf("No hay turnos en el sistema.\n");
        return;
    }

    int mes, dia;
    printf("Ingrese el Mes actual (11 o 12): ");
    scanf("%d", &mes);
    printf("Ingrese el Dia actual: ");
    scanf("%d", &dia);

    reset_listaVO(lt);
    int encontrado = 0;

    while (!isOos_listaVO(*lt)) {
        Turno t = copy_listaVO(*lt);
        Fecha f = getFechaTurno(t);

        if (getMes(f) == mes && getDia(f) == dia && getEstado(t) == 2) {
            encontrado = 1;
            printf("\n=== PROXIMO PACIENTE A ATENDER ===\n");
            printf("===============\n ID Turno: %d \n DNI Paciente: %ld \n Hora: %02d:%02d hs\n===============\n\n",
                   getId(t), getPaciente_dni(t), getHora(f), getMinuto(f));

            int presencia;
            printf("¿El paciente esta en el consultorio? (1=Si, 0=No): ");
            scanf("%d", &presencia);

            if (presencia == 1) {
                setEstado(&t, 3); // 3 = Atendido

                supress_listaVO(lt);
                insert_listaVO(lt, t);

                printf("Turno actualizado a estado ATENDIDO.\n");
            } else {
                printf("Paciente ausente. Procediendo a cancelar el turno...\n");
                setEstado(&t, 4); // 4 = Cancelado

                supress_listaVO(lt);
                insert_listaVO(lt, t);

                setDisponible(&agenda[getId(t)], 1);
                elimina_turno_cancelado(lt);
            }
            break;
        }
        forward_listaVO(lt);
    }

    if (!encontrado) {
        printf("No hay pacientes con turno CONFIRMADO para la fecha %02d/%02d/2026.\n", dia, mes);
    }
}

// ====================================================================
// FUNCIONALIDAD: Mostrar todos los turnos registrados
// ====================================================================
void muestra_turnos(ListaTurnoVO lt) {
    if (isempty_listaVO(lt)) {
        printf("No hay turnos para mostrar.\n");
        return;
    }

    reset_listaVO(&lt);
    int contador = 0;
    int pagina = 1;

    printf("\n--- LISTA COMPLETA DE TURNOS (PAGINA %d) ---\n", pagina);

    while (!isOos_listaVO(lt)) {
        Turno t = copy_listaVO(lt);
        Fecha f = getFechaTurno(t);

        printf("==================\n ID: %3d \n DNI: %ld \n Fecha: %02d/%02d/%d \n Hora: %02d:%02d \n Estado: %s\n==================\n\n",
               getId(t), getPaciente_dni(t), getDia(f), getMes(f), getAnio(f),
               getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));

        contador++;

        if (contador % 5 == 0 && !isOos_listaVO(lt)) {
            printf("\nPresione ENTER para ver la siguiente pagina...");
            getchar();
            getchar();
            pagina++;
            printf("\n--- LISTA COMPLETA DE TURNOS (PAGINA %d) ---\n", pagina);
        }

        forward_listaVO(&lt);
    }
    printf("\n--- FIN DE LA LISTA (%d turnos en total) ---\n", contador);
}

// ====================================================================
// FUNCIONALIDAD RECURSIVA: Mostrar turnos asignados por mes (Real)
// ====================================================================
void mostrar_turnos_mes_rec(ListaTurnoVO *lt, int mes_buscado) {
    if (isOos_listaVO(*lt)) {
        return;
    }

    Turno t = copy_listaVO(*lt);
    Fecha f = getFechaTurno(t);

    if (getMes(f) == mes_buscado) {
        printf("===============\n ID Turno: %d \n Fecha: %02d/%02d/%d\n Hora: %02d:%02d hs \n Estado: %s\n===============\n\n",
               getId(t), getDia(f), getMes(f), getAnio(f), getHora(f), getMinuto(f), obtener_nombre_estado(getEstado(t)));
    }

    forward_listaVO(lt);
    mostrar_turnos_mes_rec(lt, mes_buscado);
}

// ====================================================================
// FUNCIONALIDAD RECURSIVA: Mostrar turnos asignados por mes (Wrapper)
// ====================================================================
void mostrar_turnos_mes(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) {
        printf("No hay turnos registrados en el sistema.\n");
        return;
    }

    int mes;
    printf("Ingrese el mes a consultar (11 o 12): ");
    scanf("%d", &mes);

    printf("\n--- TURNOS ASIGNADOS EN EL MES %d ---\n", mes);
    reset_listaVO(lt);
    mostrar_turnos_mes_rec(lt, mes);
}

// ====================================================================
// FUNCIONALIDAD RECURSIVA: Contar turnos atendidos por dia y mes
// ====================================================================
int contar_atendidos_rec(ListaTurnoVO *lt, int dia, int mes) {
    if (isOos_listaVO(*lt)) return 0;

    Turno t = copy_listaVO(*lt);
    Fecha f = getFechaTurno(t);
    int contador = 0;

    if (getDia(f) == dia && getMes(f) == mes && getEstado(t) == 3) {
        contador = 1;
    }

    forward_listaVO(lt);
    return contador + contar_atendidos_rec(lt, dia, mes);
}

void contar_atendidos(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) {
        printf("No hay turnos registrados.\n");
        return;
    }
    int dia, mes;
    printf("Ingrese el MES (11 o 12): "); scanf("%d", &mes);
    printf("Ingrese el DIA: "); scanf("%d", &dia);

    reset_listaVO(lt);
    int total = contar_atendidos_rec(lt, dia, mes);

    printf("\nTotal de pacientes atendidos el %02d/%02d: %d\n", dia, mes, total);
}

// ====================================================================
// FUNCIONALIDAD RECURSIVA: Calcular el monto ingresado en un mes dado
// ====================================================================
float calcular_monto_rec(ListaTurnoVO *lt, int mes) {
    if (isOos_listaVO(*lt)) return 0.0;

    Turno t = copy_listaVO(*lt);
    Fecha f = getFechaTurno(t);
    float acumulador = 0.0;

    if (getMes(f) == mes && getEstado(t) == 3) {
        acumulador = getMonto(t);
    }

    forward_listaVO(lt);
    return acumulador + calcular_monto_rec(lt, mes);
}

void calcular_monto_mes(ListaTurnoVO *lt) {
    if (isempty_listaVO(*lt)) {
        printf("No hay turnos registrados.\n");
        return;
    }
    int mes;
    printf("Ingrese el MES a facturar (11 o 12): "); scanf("%d", &mes);

    reset_listaVO(lt);
    float total = calcular_monto_rec(lt, mes);

    printf("\nRecaudacion total del mes %d: $%.2f\n", mes, total);
}

// ====================================================================
// PERSISTENCIA: Guardar pacientes y turnos activos en archivos .txt
// ====================================================================
void guardar_sistema(ListaPacientesE lp, ListaTurnoVO lt) {
    FILE *archP = fopen("pacientes.txt", "w");
    if (archP != NULL) {
        reset_listaE(&lp);
        while (!isOos_listaE(lp)) {
            Paciente p = copy_listaE(lp);
            char *nom = getNombre(p);
            char *ape = getApellido(p);
            fprintf(archP, "%ld %s %s %d\n", getDni(p), nom, ape, getOsocial(p));
            free(nom); free(ape);
            forward_listaE(&lp);
        }
        fclose(archP);
    }

    FILE *archT = fopen("turnos.txt", "w");
    if (archT != NULL) {
        reset_listaVO(&lt);
        while (!isOos_listaVO(lt)) {
            Turno t = copy_listaVO(lt);
            Fecha f = getFechaTurno(t);
            fprintf(archT, "%d %ld %d %d %d %d %d %d %d %.2f\n",
                    getId(t), getPaciente_dni(t), getDia(f), getMes(f), getAnio(f),
                    getHora(f), getMinuto(f), getEstado(t), getMutual(t), getMonto(t));
            forward_listaVO(&lt);
        }
        fclose(archT);
    }
    printf("-> [SISTEMA] Datos guardados exitosamente en archivos de texto.\n");
}

// ====================================================================
// PERSISTENCIA: Precargar datos desde archivos .txt al iniciar
// ====================================================================
void cargar_sistema(ListaPacientesE *lp, ListaTurnoVO *lt, Fecha agenda[]) {
    FILE *archP = fopen("pacientes.txt", "r");
    if (archP != NULL) {
        Paciente p;
        long dni; char nom[20], ape[20]; int os;
        int contP = 0;

        while (fscanf(archP, "%ld %s %s %d", &dni, nom, ape, &os) == 4) {
            setDni(&p, dni); setNombre(&p, nom); setApellido(&p, ape); setOsocial(&p, os);
            insert_listaE(lp, p);
            contP++;
        }
        fclose(archP);
        printf("-> [PRECARGA] %d Pacientes recuperados de pacientes.txt\n", contP);
    } else {
        printf("-> [PRECARGA] No se encontro pacientes.txt. Arranca vacio.\n");
    }

    FILE *archT = fopen("turnos.txt", "r");
    if (archT != NULL) {
        Turno t;
        int id, dia, mes, anio, hora, min, est, mut;
        long dni; float monto;
        int contT = 0;

        while (fscanf(archT, "%d %ld %d %d %d %d %d %d %d %f",
                      &id, &dni, &dia, &mes, &anio, &hora, &min, &est, &mut, &monto) == 10) {

            setId(&t, id); setPaciente(&t, dni);
            setFechaTurno(&t, dia, mes, anio, hora, min, 0);
            setEstado(&t, est); setMutual(&t, mut); setMonto(&t, monto);
            insert_listaVO(lt, t);

            setDisponible(&agenda[id], 0);
            contT++;
        }
        fclose(archT);
        printf("-> [PRECARGA] %d Turnos recuperados (y bloqueados en agenda) de turnos.txt\n", contT);
    }
}

int main(){
    Fecha AGENDA[Maxarr];
    ListaPacientesE LPacientes;
    ListaTurnoVO LTurnoVO;

    inicializar_agenda(AGENDA);
    init_listaE(&LPacientes);
    init_listaVO(&LTurnoVO);

    cargar_sistema(&LPacientes, &LTurnoVO, AGENDA);

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
                    printf("\n====== MENU ======\n== Gestion de Agenda == \n\n");
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
                    printf("\n====== MENU ======\n== Gestion de Pacientes == \n\n");
                    printf("(1). Cargar paciente por teclado \n");
                    printf("(2). Eliminar paciente por teclado \n");
                    printf("(3). Mostrar datos de un paciente segun DNI\n");
                    printf("(4). Mostrar todos los pacientes\n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2){
                        case 1: cargar_paciente(&LPacientes); break;
                        case 2: eliminar_paciente(&LPacientes, &LTurnoVO); break;
                        case 3: mostrar_paciente_dni(LPacientes); break;
                        case 4: mostrar_todos_pacientes(LPacientes); break;
                        case 0: break;
                    }
                } while (op2 != 0);
                break;
            }

            case 3: {
                do {
                    printf("\n====== MENU ======\n== Gestion de Turnos == \n\n");
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
                    case 1:
                        buscar_fecha_libre(AGENDA);
                        solicitar_turno(&LTurnoVO, &LPacientes, AGENDA);
                        break;
                    case 2:
                        muestra_turno_dni(LTurnoVO);
                        break;
                    case 3:
                        muestra_turno_buscado(LTurnoVO);
                        break;
                    case 4:
                        confirmar_turno(&LTurnoVO, LPacientes);
                        break;
                    case 5:
                        cancela_turno(&LTurnoVO, AGENDA);
                        break;
                    case 6:
                        proximo_paciente(&LTurnoVO, AGENDA);
                        break;
                    case 7:
                        muestra_turnos(LTurnoVO);
                        break;
                    case 0:
                        break;
                    default:
                        printf("Opcion invalida.\n");
                    }
                } while (op2 != 0);
                break;
            }

            case 4: {
                do {
                    printf("\n====== MENU ======\n== Recursividad y Reportes == \n\n");
                    printf("(1). Mostrar turnos asignados por mes\n");
                    printf("(2). Contar turnos atendidos por dia y mes\n");
                    printf("(3). Calcular el monto ingresado en un mes dado\n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2){
                        case 1:
                            mostrar_turnos_mes(&LTurnoVO);
                            break;
                        case 2:
                            contar_atendidos(&LTurnoVO);
                            break;
                        case 3:
                            calcular_monto_mes(&LTurnoVO);
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcion invalida.\n");
                    }
                } while (op2 != 0);
                break;
            }

            case 0:
                printf("\nGuardando informacion y saliendo...\n");
                guardar_sistema(LPacientes, LTurnoVO);
                break;

            default:
                printf("Opcion invalida.\n");
        }
    } while (op1 != 0);

    return 0;
}

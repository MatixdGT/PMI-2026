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
                    }
                }
            }
        }
    }
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

    for (int i = 0; i < Maxarr; i++) {
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

int main() {
    Fecha AGENDA[Maxarr];

    // Tareas OBLIGATORIAS al arrancar el programa
    inicializar_agenda(AGENDA);

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
                    printf("(1). Busca Fecha libre en la agenda (b)\n");
                    printf("(2). Busca fecha ocupada en agenda (d)\n");
                    printf("(0). Salir \n");
                    printf("Elige la Opcion: ");
                    scanf("%d", &op2);
                    printf(" \n");

                    switch(op2) {
                        case 1:
                            buscar_fecha_libre(AGENDA); // ¡Tu nueva funcion implementada!
                            break;
                        case 2:
                            // Aca ira la funcionalidad (d)
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
                        case 1: break;
                        case 2: break;
                        case 3: break;
                        case 4: break;
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
                        case 1: break;
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

    return 0;
}

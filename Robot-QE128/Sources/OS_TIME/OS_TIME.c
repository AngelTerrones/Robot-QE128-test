/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_TIME.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 13/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Implementación de los las funciones
 **                           relacionados con el manejo del timer del sistema
 **                           (TICK)
 **
 **    Revisiones:
 **    - 13/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/OS/OS.h"
#include "../Sources/OS_TICK/OS_TICK.h"
#include "../Sources/OS_LIST/OS_LIST.h"
#include "../Sources/OS_TIME/OS_TIME.h"

// *****************************************************************************
// Variables privadas del modulo
OS_TCB *            osTaskBlockDelayListHead = NULL;                            // lista de procesos bloqueados por tiempo
OS_TCB *            osTaskBlockDelayListTail = NULL;                            // lista de procesos bloqueados por tiempo

// *****************************************************************************
// Variables privadas de otros modulos
extern OS_TCB *     osTaskCurrent;                                              // Proceso actual -> OS.c

// *****************************************************************************
// Funciones privadas
void    OSInsertTaskBlockDelayList  (OS_TCB *tcb);                              //
void    OSRemoveTaskBlockDelayList  (OS_TCB *tcb);                              //
void    OSDelayResumeTask           (void);                                     //

// *****************************************************************************
// Funciones privadas definidas en otros modulos
extern void OSInsertTaskReadyList(OS_TCB *tcb, bool prioridad);                 // OS.c

// *****************************************************************************
// Implementacion de las funciones publicas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSDelayMS(int32u delay)
{
    OS_CPU_SR srCPU;                                                            // variable auxiliar para zona critica
    delay = (delay*OS_TICK_PER_SEC)/1000L;                                      // Calcular cuantos ticks son
    if(delay > 0){
        srCPU = OSEnterCriticalSection();                                       // prevenir interrupciones
            osTaskCurrent->OSTimeDelayMS = delay;                               // copiar el tiempo por el cual se bloquea
            OSInsertTaskBlockDelayList(osTaskCurrent);                          // insertar en la lista de bloqueados
            OSSyscall();                                                        // bloquear y sacar proximo proceso
        OSExitCriticalSection(srCPU);                                           // reestablecer estado anterior
    }
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSDelayS(int32u delay)
{
    OS_CPU_SR srCPU;                                                            // variable auxiliar para zona critica

    delay *= OS_TICK_PER_SEC;
    if(delay > 0){
        srCPU = OSEnterCriticalSection();                                       // prevenir interrupciones
            osTaskCurrent->OSTimeDelayMS = delay;                               // copiar el tiempo por el cual se bloquea
            OSInsertTaskBlockDelayList(osTaskCurrent);                          // insertar en la lista de bloqueados
            OSSyscall();                                                        // bloquear y sacar proximo proceso
        OSExitCriticalSection(srCPU);                                           // reestablecer estado anterior
    }
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSDelayHMS(int8u hour, int8u minutes, int8u seconds)
{
    int32u delay = 0;
    OS_CPU_SR srCPU;                                                            // variable auxiliar para zona critica
    if(hour == 0 && minutes == 0 && seconds == 0)
        return OS_ERROR_TIME_ZERO_DELAY;

    if(minutes > 59)
        return OS_ERROR_TIME_INVALID_MIN;
    if(seconds > 59)
        return OS_ERROR_TIME_INVALID_SEG;

    delay = ((int32u)hour*3600L + (int32u)minutes*60L + seconds)*OS_TICK_PER_SEC;
    srCPU = OSEnterCriticalSection();                                           // prevenir interrupciones
        osTaskCurrent->OSTimeDelayMS = delay;                                   // copiar el tiempo por el cual se bloquea
        OSInsertTaskBlockDelayList(osTaskCurrent);                              // insertar en la lista de bloqueados
        OSSyscall();                                                            // bloquear y sacar proximo proceso
    OSExitCriticalSection(srCPU);                                               // reestablecer estado anterior

    return OS_ERROR_NONE;
}

// *****************************************************************************
// Implementacion de las funciones privadas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSInsertTaskBlockDelayList(OS_TCB *tcb)
{
    OSInsertTCBLista(tcb, &osTaskBlockDelayListHead, &osTaskBlockDelayListTail);
    tcb->OSTaskStatus = OS_TASK_STATUS_BLOCK_DELAY;                             // bloqueado
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSRemoveTaskBlockDelayList(OS_TCB *tcb)
{
    OSRemoveTCBLista(tcb, &osTaskBlockDelayListHead, &osTaskBlockDelayListTail);
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSDelayResumeTask(void)
{
    OS_TCB *tcb      = osTaskBlockDelayListTail;                                // empezar desde el final de la lista
    OS_TCB *tcbSwap;

    // para todos los elementos de la lista
    while(tcb != NULL){
        if(!--(tcb->OSTimeDelayMS)){                                            // decrementar y determinar si es cero.
            tcbSwap = tcb->OSTCBPrev;                                           // guardar el proximo en la verificacion
            OSRemoveTaskBlockDelayList(tcb);                                    // remover de la lista
            OSInsertTaskReadyList(tcb, true);                                   // insertar en la lista de procesos listos con mayor prioridad
            tcb = tcbSwap;                                                      // proximo a verificar
        }
        else{
            tcb = tcb->OSTCBPrev;                                               // proximo elemento
        }
    }
}
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
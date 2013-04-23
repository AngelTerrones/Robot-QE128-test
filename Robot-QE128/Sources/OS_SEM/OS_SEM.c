/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_SEM.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 24/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Implementacion de las funciones asociadas al
 **                           manejo de los semaforos.
 **
 **    Revisiones:
 **    - 24/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/OS_LIST/OS_LIST.h"
#include "../Sources/OS_SEM/OS_SEM.h"
#include "../Sources/OS_MEM/OS_MEM.h"
#include "../Sources/OS_STRING/OS_STRING.h"

// *****************************************************************************
// Variables privadas de otros modulos
extern OS_TCB *     osTaskCurrent;                                              // Proceso actual

// *****************************************************************************
// Variables privadas del modulo
OS_SEM *  osSemCreatedListHead = NULL;                                          // Inicio de la lista de sem usados
OS_SEM *  osSemCreatedListTail = NULL;                                          // Fin de la lista de sem usados

// *****************************************************************************
// Funciones privadas definidas en otros modulos
extern void OSInsertTaskReadyList(OS_TCB *tcb, bool prioridad);                 // OS.c

// *****************************************************************************
// Funciones privadas del modulo
void        OSInsertSemCreatedList      (OS_SEM *sem);                          // Insertar el semaforo en la lista de sem. creados
void        OSRemoveSemCreatedList      (OS_SEM *sem);                          // Quitar un semaforo de la lista
void        OSInsertTaskBlockSemList    (OS_TCB *tcb);                          // Insertar un tcb en la lista de bloqueados
void        OSRemoveTaskBlockSemList    (OS_TCB *tcb);                          // Remover un tcb de la lista de bloqueados
void        OSSemResumeTasks            (OS_SEM *sem);                          // Alistar los procesos que puedan ser liberados

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
OS_SEM *OSSemAlloc(const int8u *name, int16u initialCount)
{
    OS_CPU_SR sr;
    OS_SEM *sem = OSAllocMem(sizeof(OS_SEM));

    if(sem == NULL)
        return NULL;

    sem->OSSemName          = name;                                             // Colocar el nuevo nombre
    sem->OSSemCount         = initialCount;                                     // Numero de cuentas iniciales del semaforo
    sem->OSBlockListHead    = NULL;                                             // Inicio de la lista de tareas bloqueadas
    sem->OSBlockListTail    = NULL;                                             // Fin
    sem->OSSemNext          = NULL;                                             // Proximo semaforo
    sem->OSSemPrev          = NULL;                                             // Semaforo previo
    sr = OSEnterCriticalSection();
        OSInsertSemCreatedList(sem);                                            // insertar el nuevo semaforo
    OSExitCriticalSection(sr);
    return sem;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSSemFree(OS_SEM *sem)
{
    OS_CPU_SR sr;
    if(sem->OSBlockListHead != NULL)                                            // y no tiene procesos bloqueados
        return OS_ERROR_SEM_HAVE_BLOCKED_TASK;
    sr = OSEnterCriticalSection();
        OSRemoveSemCreatedList(sem);                                            // Sacar de la lista de semaforos creados
    OSExitCriticalSection(sr);
    OSFreeMem(sem);                                                             // y liberar la memoria
    return OS_ERROR_NONE;
}
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSSemWait(OS_SEM *sem, int16u number)
{
    OS_CPU_SR sr;

    if(sem == NULL)                                                             // Null pointer? -> GTFO
        return OS_ERROR_SEM_NULL;
    if(number == 0)                                                             // Salir y ya
        return OS_ERROR_NONE;

    sr = OSEnterCriticalSection();
        if(sem->OSSemCount >= number){                                          // Si hay suficientes cuentas, restarlas y salir
            sem->OSSemCount -= number;
            OSExitCriticalSection(sr);
            return OS_ERROR_NONE;
        }
        osTaskCurrent->OSSemBlocking = sem;                                     // indicar que el proceso esta bloqueado
        osTaskCurrent->OSSemCountBlocking = number;                             // indicar de cuanto es la cuenta
        OSInsertTaskBlockSemList(osTaskCurrent);                                // e insertar en la lista de bloqueados
        OSSyscall();
    OSExitCriticalSection(sr);
    return OS_ERROR_NONE;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSSemSignal(OS_SEM *sem, int16u number)
{
    OS_CPU_SR sr;
    if(sem == NULL)                                                             // Null pointer? -> GTFO
        return OS_ERROR_SEM_NULL;
    if(number == 0)                                                             // Salir y ya
        return OS_ERROR_NONE;

    sr = OSEnterCriticalSection();
        if(sem->OSSemCount + number <= 65535u)                                   // Verificar que no haga overflow
            sem->OSSemCount += number;
        else
            sem->OSSemCount = 65535u;
        if(sem->OSBlockListHead != NULL)                                        // Si hay procesos bloqueados, sacar
            OSSemResumeTasks(sem);                                              // pero ho hacer cambio de contexto
    OSExitCriticalSection(sr);
    return OS_ERROR_NONE;
}
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_SEM *OSSemGetByName(const int8u *name)
{
    OS_CPU_SR sr;
    OS_SEM * sem;

    sr = OSEnterCriticalSection();                                              // manejo de la lista. Proteger el acceso
        sem = osSemCreatedListHead;
        while(sem != NULL){                                                     // buscar en toda la lista, por nombre.
            if(OSStrCmp(sem->OSSemName, name)){
                OSExitCriticalSection(sr);
                return sem;
            }
            sem = sem->OSSemNext;
        }
    OSExitCriticalSection(sr);

    return NULL;
}

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
void OSInsertSemCreatedList(OS_SEM *sem)
{
    if(sem == NULL)
        return;
    if(osSemCreatedListHead == NULL){                                           // Lista vacia
        osSemCreatedListHead = sem;                                             // Primer elemento a insertar
        sem->OSSemPrev = NULL;                                                  // El anterior no existe
    }
    else{
        osSemCreatedListTail->OSSemNext = sem;                                  // insertar al final
        sem->OSSemPrev = osSemCreatedListTail;                                  // apuntar al anterior
    }
    osSemCreatedListTail = sem;                                                 // actualizar el apuntador final
    sem->OSSemNext = NULL;                                                      // fin de la lista
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSRemoveSemCreatedList(OS_SEM *sem)
{
    if(sem == NULL)
        return;

    if(sem->OSSemPrev != NULL)
        (sem->OSSemPrev)->OSSemNext = sem->OSSemNext;                           // elemento previo existe
    else
        osSemCreatedListHead = sem->OSSemNext;                                  // primer elemento

    if(sem->OSSemNext != NULL)
        (sem->OSSemNext)->OSSemPrev = sem->OSSemPrev;                           // proximo elemento existe
    else
        osSemCreatedListTail = sem->OSSemPrev;                                  // ultimo elemento

    sem->OSSemPrev = sem->OSSemNext = NULL;                                     // por seguridad...
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSInsertTaskBlockSemList(OS_TCB *tcb)
{
    OS_SEM *sem = tcb->OSSemBlocking;                                           // obtener el semaforo que hace el bloqueo
    OSInsertTCBLista(tcb, &(sem->OSBlockListHead), &(sem->OSBlockListTail));    // Insertar en la lista de procesos bloqueados
    tcb->OSTaskStatus = OS_TASK_STATUS_BLOCK_SEMAPHORE;                         // marcar como bloqueado
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSRemoveTaskBlockSemList(OS_TCB *tcb)
{
    OS_SEM *sem = tcb->OSSemBlocking;                                           // Obtener el semaforo bloqueante
    tcb->OSSemCountBlocking = 0;                                                // marcar que nada bloquea a la tarea
    tcb->OSSemBlocking = NULL;
    OSRemoveTCBLista(tcb, &(sem->OSBlockListHead), &(sem->OSBlockListTail));    // Remover el proceso de la lista de bloqueados
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSSemResumeTasks(OS_SEM *sem)
{
    OS_CPU_SR sr;
    OS_TCB *tcb;
    OS_TCB *swap;
    sr = OSEnterCriticalSection();
        tcb = sem->OSBlockListHead;                                             // inicio de la lista
        while(tcb != NULL){                                                     // recorrer la lista
            if(sem->OSSemCount >= tcb->OSSemCountBlocking){                     // Si hay suficientes contadores para agarrar
                sem->OSSemCount -= tcb->OSSemCountBlocking;                     // decrementar, colocar la cuenta de espera en cero
                swap = tcb->OSTCBNext;
                OSRemoveTaskBlockSemList(tcb);                                  // sacar de la lista de bloqueados y colocarlo en la de listos
                OSInsertTaskReadyList(tcb, false);                              // Si prioridad: al final de la cola de listos
                tcb = swap;
            }
            else{
                tcb = tcb->OSTCBNext;
            }
        }
    OSExitCriticalSection(sr);
}
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
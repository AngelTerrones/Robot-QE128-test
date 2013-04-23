/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_MUTEX.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 15/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	:
 **
 **    Revisiones:
 **    - 15/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/OS_LIST/OS_LIST.h"
#include "../Sources/OS_MEM/OS_MEM.h"
#include "../Sources/OS_MUTEX/OS_MUTEX.h"
#include "../Sources/OS_STRING/OS_STRING.h"

// *****************************************************************************
// Variables privadas de otros modulos
extern OS_TCB *     osTaskCurrent;                  // Proceso actual

// *****************************************************************************
// Variables privadas del modulo
OS_MUTEX *  osMutexCreatedListHead = NULL;                                      // Inicio de la lista de mutex usados
OS_MUTEX *  osMutexCreatedListTail = NULL;                                      // Fin de la lista de mutex usados

// *****************************************************************************
// Funciones privadas definidas en otros modulos
extern void OSInsertTaskReadyList(OS_TCB *tcb, bool prioridad);                 // OS.c

// *****************************************************************************
// Funciones privadas del modulo
void        OSInsertMutexCreatedList    (OS_MUTEX *mutex);
void        OSRemoveMutexCreatedList    (OS_MUTEX *mutex);
void        OSInsertTaskBlockMutexList  (OS_TCB *tcb);
OS_TCB *    OSRemoveTaskBlockMutexList  (OS_MUTEX *mutex);
void        OSMutexResumeTask           (OS_MUTEX *mutex);

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
OS_MUTEX *OSMutexAlloc(const int8u *name)
{
    OS_CPU_SR sr;                                                               // zona critica
    OS_MUTEX *mutex;

    sr = OSEnterCriticalSection();                                              // entrar en la zona critica
        mutex = OSAllocMem(sizeof(OS_MUTEX));                                   //
        if(mutex == NULL){
            OSExitCriticalSection(sr);                                          // salir de la zona critica
            return NULL;
        }
        // configurar
        mutex->OSMutexName = name;                                              // copiar el nombre
        mutex->OSMutexLocked = false;                                           // set false
        mutex->OSMutexTaskParent = osTaskCurrent;                               // indicar quien es el creador del mutex
        mutex->OSMutexCurrentOwner = NULL;
        mutex->OSBlockListHead = NULL;
        mutex->OSBlockListTail = NULL;
        mutex->OSMutexNext = NULL;
        mutex->OSMutexPrev = NULL;
        OSInsertMutexCreatedList(mutex);                                        // insertar en la lista de mutex usados...
    OSExitCriticalSection(sr);                                                  // salir de la zona critica
    return mutex;                                                               // devolver el mutex
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSMutexFree(OS_MUTEX *mutex)
{
    OS_CPU_SR sr;

    sr = OSEnterCriticalSection();                                              // enter critical section
        if(mutex->OSMutexTaskParent != osTaskCurrent){                          // verificar si es el "padre"
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_NOT_PARENT;                                   // no es el "padre"
        }
        if(mutex->OSMutexLocked){                                               // mutex locked?
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_LOCKED;                                       // no se puede eliminar!
        }
        if(mutex->OSBlockListHead != NULL){                                     // Verificar si tiene procesos bloqueados
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_HAVE_BLOCKED_TASKS;                           // Tiene procesos bloqueados.
        }
        OSRemoveMutexCreatedList(mutex);                                        // sacar de la lista de mutex creados.
        OSFreeMem(mutex);                                                       // devolver a la lista de mutex libres
    OSExitCriticalSection(sr);                                                  // exit critical section

    return OS_ERROR_NONE;                                                       // no hubo problemas
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSMutexLock(OS_MUTEX *mutex)
{
    OS_CPU_SR sr;

    sr = OSEnterCriticalSection();                                              // enter critical section
        if(mutex == NULL){
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_NULL;                                         // en caso de vector nulo
        }
        if(!mutex->OSMutexLocked){
            mutex->OSMutexLocked = true;                                        // locked
            mutex->OSMutexCurrentOwner = osTaskCurrent;                         // indicar quien tiene el mutex
            osTaskCurrent->OSMutexBlocking = mutex;                             // indicar cual mutex posee
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_NONE;
        }

        // No se pudo hacer lock. Por lo tanto, bloquear.
        // NOTA: la llamada del sistema no depende de ninguna
        // condicion extra en los registros del CPU.
        // Por lo tanto, se asegura que el proceso se BLOQUEARA
        // antes de cualquier intento de borrar el mutex por
        // algun otro proceso.
        // Adicionalmente, la interrupcion estara bloqueada solo
        // para este proceso, no para el resto (una vez finalizada
        // la ejecucion del scheduler).
        osTaskCurrent->OSMutexBlocking = mutex;
        OSInsertTaskBlockMutexList(osTaskCurrent);                              // insertal el proceso en la lista de bloqueados
        OSSyscall();                                                            // bloquear el proceso.
        mutex->OSMutexLocked = true;                                            // locked
        mutex->OSMutexCurrentOwner = osTaskCurrent;                             // indicar quien tiene el mutex
    OSExitCriticalSection(sr);                                                  // exit critical section
    return OS_ERROR_NONE;                                                       // sin problemas.
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSMutexTryLock(OS_MUTEX *mutex)
{
    OS_CPU_SR sr;

    sr = OSEnterCriticalSection();                                              // enter critical section
        if(mutex == NULL){
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_NULL;                                         // en caso de vector nulo
        }
        if(!mutex->OSMutexLocked){
            mutex->OSMutexLocked = true;                                        // locked
            mutex->OSMutexCurrentOwner = osTaskCurrent;                         // indicar quien tiene el mutex
            osTaskCurrent->OSMutexBlocking = mutex;                             // indicar cual mutex posee
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_NONE;
        }
    OSExitCriticalSection(sr);                                                  // exit critical section
    return OS_ERROR_MUTEX_LOCKED;                                               // No se pudo hacer lock al mutex
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSMutexUnlock(OS_MUTEX *mutex)
{
    OS_CPU_SR sr;

    sr = OSEnterCriticalSection();                                              // enter critical section
        if(mutex == NULL){
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_NULL;                                         // en caso de vector nulo
        }
        if(mutex->OSMutexCurrentOwner != osTaskCurrent){                        // intentar hacer unlock cuando no se posee el mutex
            OSExitCriticalSection(sr);                                          // exit critical section
            return OS_ERROR_MUTEX_NOT_OWNER;
        }
        // El mutex existe y soy el dueño: hacer release.
        mutex->OSMutexLocked = false;                                           // unlocked
        mutex->OSMutexCurrentOwner = NULL;                                      // indicar quien tiene el mutex
        osTaskCurrent->OSMutexBlocking = NULL;                                  // el proceso no posee mutex. OJO
        OSMutexResumeTask(mutex);                                               // resumir procesos en espera por el mutex
        OSSyscall();                                                            // llamar
    OSExitCriticalSection(sr);                                                  // exit critical section
    return OS_ERROR_NONE;                                                       // sin problemas.
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_MUTEX *OSMutexGetByName(const int8u *name)
{
    OS_CPU_SR sr;
    OS_MUTEX *mutex = osMutexCreatedListHead;

    sr = OSEnterCriticalSection();                                              // enter critical section
        while(mutex != NULL){                                                   // mientras existan elementos
            if(OSStrCmp(mutex->OSMutexName, name)){                             // comparar el nombre
                OSExitCriticalSection(sr);                                      // exit critical section
                return mutex;                                                   // match
            }
            mutex = mutex->OSMutexNext;                                         // proximo en la lista
        }
    OSExitCriticalSection(sr);                                                  // exit critical section
    return NULL;                                                                // mutex no existe con ese nombre.
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
void OSInsertMutexCreatedList(OS_MUTEX *mutex)
{
    if(mutex == NULL)
        return;

    if(osMutexCreatedListHead == NULL){                                         // Lista vacia
        osMutexCreatedListHead = mutex;                                         // Primer elemento a insertar
        mutex->OSMutexPrev = NULL;                                              // El anterior no existe
    }
    else{
        osMutexCreatedListTail->OSMutexNext = mutex;                            // insertar al final
        mutex->OSMutexPrev = osMutexCreatedListTail;                            // apuntar al anterior
    }

    osMutexCreatedListTail = mutex;                                             // actualizar el apuntador final
    mutex->OSMutexNext = NULL;                                                  // fin de la lista
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSRemoveMutexCreatedList(OS_MUTEX *mutex)
{
    if(mutex == NULL)
        return;

    if(mutex->OSMutexPrev != NULL)
        (mutex->OSMutexPrev)->OSMutexNext = mutex->OSMutexNext;                 // elemento previo existe
    else
        osMutexCreatedListHead = mutex->OSMutexNext;                            // primer elemento

    if(mutex->OSMutexNext != NULL)
        (mutex->OSMutexNext)->OSMutexPrev = mutex->OSMutexPrev;                 // proximo elemento existe
    else
        osMutexCreatedListTail = mutex->OSMutexPrev;                            // ultimo elemento

    mutex->OSMutexPrev = mutex->OSMutexNext = NULL;                             // por seguridad...
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSInsertTaskBlockMutexList(OS_TCB *tcb)
{
    OS_MUTEX *mutex = tcb->OSMutexBlocking;                                     // obtener el mutex que hace el bloqueo
    OSInsertTCBLista(tcb, &(mutex->OSBlockListHead), &(mutex->OSBlockListTail));
    tcb->OSTaskStatus = OS_TASK_STATUS_BLOCK_MUTEX;                             // marcar como bloqueado
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_TCB *OSRemoveTaskBlockMutexList(OS_MUTEX *mutex)
{
    OS_TCB *tcb;

    if(mutex->OSBlockListHead != NULL){                                         // si existe algun proceso
        tcb = mutex->OSBlockListHead;                                           // sacar el primero
        OSRemoveTCBLista(tcb, &(mutex->OSBlockListHead), &(mutex->OSBlockListTail));
        return tcb;                                                             // devolver
    }
    return NULL;                                                                // lista vacia.
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSMutexResumeTask(OS_MUTEX *mutex)
{
    // NOTA: Solo desbloquea el primer proceso en la lista, ya que si se desbloquean
    // Todos al mismo tiempo, no se asegura la exclusividad en el acceso al mutex
    // después del "unlock"
    OS_TCB *tcb = OSRemoveTaskBlockMutexList(mutex);
    if(tcb != NULL)
        OSInsertTaskReadyList(tcb, true);                                       // insertar en la lista de procesos listos con mayor prioridad   
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
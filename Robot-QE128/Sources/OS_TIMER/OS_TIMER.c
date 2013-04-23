/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_TIMER.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 23/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Implementacion del modulo de timers del SO
 **
 **    Revisiones:
 **    - 23/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/OS_TIMER/OS_TIMER.h"
#include "../Sources/OS_MEM/OS_MEM.h"
#include "../Sources/OS_SEM/OS_SEM.h"
#include "../Sources/OS_TICK/OS_TICK.h"

// *****************************************************************************
// Constantes privadas del modulo
#define OS_TICK_PER_SEC_TIMER           50                                      // se desea una resolucion de 1/10 segundos
#define OS_TICK_MAX_COUNTER             (OS_TICK_PER_SEC/OS_TICK_PER_SEC_TIMER) // numero de veces que debe hacer tick para lograr la resolucion

// *****************************************************************************
// Variables privadas del modulo
int8u           counterTimers   = 0;
OS_TIMER *      osTimerListHead = NULL;                                         // Inicio de la lista de timers activos
OS_TIMER *      osTimerListTail = NULL;                                         // Final de la lista de timers activos
OS_SEM *        osSemTimer;                                                     // Semaforo para hacer signal al timer task
const int8u     osSemName[]     = "Timer Sem";                                  // Nombre del timer

// *****************************************************************************
// Funciones privadas del modulo
void    OSInsertTimerList   (OS_TIMER *timer);                                  // Insertar el timer en la lista
void    OSRemoveTimerList   (OS_TIMER *timer);                                  // Remover timer de la lista
void    OSDecrTimerCounters (void);                                             // Decrementar el contador de los timers
void    OSSignalTimers      (void);                                             // Incrementar el contador interno y hacer signal a los timers
void    OSTimerTask         (void *arg);                                        // Task para el manejo de los timers

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
void __OSInitTimer(void)
{
    osSemTimer = OSSemAlloc(osSemName, 0);                                      // Crear el semaforo.
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_TIMER *OSTimerCreate(int16u period, bool periodic, OS_TIMER_CALLBACK callback, OS_ERROR *error)
{
    OS_TIMER *timer;
    if(error == NULL)                                                           // Si no tengo donde hacer registo del error, salir
        return NULL;
    period /= OS_TICK_MAX_COUNTER;                                              // Normalizar
    if(period == 0){                                                            // El timer no puede ser instantaneo
        *error = OS_ERROR_TIMER_ZERO_PERIOD;
        return NULL;
    }
    if(callback == NULL){                                                       // Funcion valida?
        *error = OS_ERROR_TIMER_CALLBACK_NULL;
        return NULL;
    }
    timer = OSAllocMem(sizeof(OS_TIMER));
    if(timer == NULL){                                                          // Verificar que se reservo la memoria
        *error = OS_ERROR_TIMER_NO_MEMORY;
        return NULL;
    }
    timer->OSTimerPeriod    = period;                                           // Inicializar campos
    timer->OSTimerCounter   = period;                                           //
    timer->OSPeriodic       = periodic;                                         //
    timer->OSTimerCallback  = callback;                                         //
    timer->OSEnabled        = false;                                            //
    timer->OSTimerNext      = NULL;                                             //
    timer->OSTimerPrev      = NULL;                                             //
    *error = OS_ERROR_NONE;
    return timer;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSTimerDelete(OS_TIMER *timer)
{
    if(timer == NULL)                                                           // Verificar que el apuntador es valido
        return OS_ERROR_TIMER_NULL;
    if(timer->OSEnabled)                                                        // Abortar si todavia está habilitado
        return OS_ERROR_TIMER_NO_STOPPED;

    timer->OSTimerPeriod    = 0;                                                // Colocar campos a valores por defecto
    timer->OSTimerCounter   = 0;
    timer->OSPeriodic       = false;
    timer->OSTimerCallback  = NULL;
    timer->OSTimerNext      = NULL;
    timer->OSTimerPrev      = NULL;
    OSFreeMem(timer);                                                           // "liberar memoria"

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
OS_ERROR OSTimerStart(OS_TIMER *timer)
{
    if(timer == NULL)
        return OS_ERROR_TIMER_NULL;

    if(timer->OSEnabled){                                                       // Si es un timer activo
        timer->OSTimerCounter = timer->OSTimerPeriod;                           // Reiniciar la cuenta
    }else{
        OSInsertTimerList(timer);                                               // Insertar en la lista
        timer->OSEnabled = true;                                                // Marcar como activo
    }

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
OS_ERROR OSTimerStop(OS_TIMER *timer)
{
    if(timer == NULL)
        return OS_ERROR_TIMER_NULL;

    OSRemoveTimerList(timer);                                                   // sacar de la lista
    timer->OSEnabled = false;                                                   // marcar como inactivo
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
OS_ERROR OSTimerSetPeriod(OS_TIMER *timer, int16u period)
{
    if(timer == NULL)
        return OS_ERROR_TIMER_NULL;
    if(timer->OSEnabled)
        return OS_ERROR_TIMER_NO_STOPPED;
    period /= OS_TICK_MAX_COUNTER;                                              // Normalizar
    if(period == 0)                                                             // El timer no puede ser instantaneo
        return OS_ERROR_TIMER_ZERO_PERIOD;
    timer->OSTimerPeriod = period;
    timer->OSTimerCounter = period;
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
void OSInsertTimerList(OS_TIMER *timer)
{
    OS_CPU_SR sr;

    if(timer == NULL)
        return;
    sr = OSEnterCriticalSection();                                              //
        if(osTimerListHead == NULL){                                            // Lista vacia
            osTimerListHead = timer;
            timer->OSTimerPrev = NULL;
        }
        else{                                                                   // Insertar al final de la lista
            osTimerListTail->OSTimerNext = timer;
            timer->OSTimerPrev = osTimerListTail;
        }
        osTimerListTail = timer;
    OSExitCriticalSection(sr);                                                  //
    timer->OSTimerNext = NULL;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSRemoveTimerList(OS_TIMER *timer)
{
    OS_CPU_SR sr;
    sr = OSEnterCriticalSection();                                              //
        if(timer->OSTimerPrev != NULL)                                          // enlace previo
            timer->OSTimerPrev->OSTimerNext = timer->OSTimerNext;
        else
            osTimerListHead = timer->OSTimerNext;
        if(timer->OSTimerNext != NULL)                                          // enlace posterior
            timer->OSTimerNext->OSTimerPrev = timer->OSTimerPrev;
        else
            osTimerListTail = timer->OSTimerPrev;
    OSExitCriticalSection(sr);                                                  //
    timer->OSTimerNext      = NULL;                                             // reiniciar apuntadores
    timer->OSTimerPrev      = NULL;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSDecrTimerCounters(void)
{
    OS_TIMER *temp;
    OS_TIMER *swap;
    OS_CPU_SR sr;

    sr = OSEnterCriticalSection();                                              // Manejo de lista: seccion critica
        temp = osTimerListHead;
        if(temp == NULL){
            OSExitCriticalSection(sr);
            return;
        }
        while(temp != NULL){                                                    // revisar toda la lista de timers
            temp->OSTimerCounter--;                                             // decrementar contador
            if(temp->OSTimerCounter == 0){                                      // Ejecutar el callback si la cuenta llegó a cero
                (temp->OSTimerCallback)();
                if(temp->OSPeriodic){                                           // Si el timer es periodico, reiniciar el contador
                    temp->OSTimerCounter = temp->OSTimerPeriod;
                }else{                                                          // en caso contrario
                    swap = temp->OSTimerPrev;
                    OSRemoveTimerList(temp);                                    // sacar de la lista
                    temp = swap;
                }
            }
            temp = temp->OSTimerNext;                                           // proximo de la lista
        }
    OSExitCriticalSection(sr);                                                  //
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSSignalTimers(void)
{
    counterTimers++;
    if(counterTimers == OS_TICK_MAX_COUNTER){                                   // Si se alcanzo la cuenta
        counterTimers = 0;                                                      // reiniciar contador
        OSSemSignal(osSemTimer, 1);                                             // Avisar al timer task
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
void OSTimerTask(void *arg)
{
    (void)arg;
    for(;;){
        OSSemWait(osSemTimer, 1);                                               // Esperar por el semaforo
        OSDecrTimerCounters();                                                  // Decrementar contadores, y ejecutar callbacks
    }
}
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
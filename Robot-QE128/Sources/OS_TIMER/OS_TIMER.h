/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_TIMER.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 23/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	:
 **
 **    Revisiones:
 **    - 23/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_TIMER_H_
#define OS_TIMER_H_

#include "../Sources/OS_MUTEX/OS_MUTEX.h"

// *****************************************************************************
// Constantes: codigos de error
#define OS_ERROR_TIMER_NULL             1u                                      // Null pointer
#define OS_ERROR_TIMER_ZERO_PERIOD      2u                                      // Periodo zero
#define OS_ERROR_TIMER_NO_PERIODIC      3u                                      // Timer no periodico
#define OS_ERROR_TIMER_NO_STOPPED       4u                                      // Timer en ejecución
#define OS_ERROR_TIMER_CALLBACK_NULL    5u                                      // No se ha indicado callback
#define OS_ERROR_TIMER_NO_MEMORY        6u                                      // No hay memoria para crear el timer

// *****************************************************************************
// Tipo de datos
// callback
typedef void (*OS_TIMER_CALLBACK)(void);                                        // Funciones a llamar cuando el timer expire

// Estructura del timer
typedef struct os_timer_struct{
    int16u                      OSTimerPeriod;                                  // Periodo del timer
    int16u                      OSTimerCounter;                                 // Contador del timer
    OS_TIMER_CALLBACK           OSTimerCallback;                                // funcion a ejecutar al expirar el timer
    bool                        OSPeriodic;                                     // Para indicar si el timer es periodico, o de un solo disparo
    bool                        OSEnabled;                                      // Indicar si esta activo
    struct os_timer_struct *    OSTimerNext;                                    // siguiente timer en la lista
    struct os_timer_struct *    OSTimerPrev;                                    // timer previo en la lista
}OS_TIMER;

// *****************************************************************************
// Funciones publicas
void        __OSInitTimer       (void);                                         // Inicializar el semaforo del timer
OS_TIMER *  OSTimerCreate       (int16u period,                                 // Crear un timer
                                 bool periodic,
                                 OS_TIMER_CALLBACK callback,
                                 OS_ERROR *error);
OS_ERROR    OSTimerDelete       (OS_TIMER *timer);                              // Borrar el timer
OS_ERROR    OSTimerStart        (OS_TIMER *timer);                              // Iniciar el timer
OS_ERROR    OSTimerStop         (OS_TIMER *timer);                              // Detener el timer
OS_ERROR    OSTimerSetPeriod    (OS_TIMER *timer, int16u period);               // Modificar el periodo del timer

#endif /* OS_TIMER_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */

/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_MUTEX.h
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

#ifndef OS_MUTEX_H_
#define OS_MUTEX_H_

#include "../Sources/OS/OS.h"

// *****************************************************************************
// Constantes: codigos de error
#define OS_ERROR_MUTEX_NULL                 1u                                  // mutex = null pointer
#define OS_ERROR_MUTEX_NOT_EXIST            2u                                  // mutex no existe (En algun momento lo uso?)
#define OS_ERROR_MUTEX_NOT_OWNER            3u                                  // intento de hacer release de un mutex que no posee
#define OS_ERROR_MUTEX_NOT_PARENT           4u                                  // intento de hacer delete de un mutex que no ha creado
#define OS_ERROR_MUTEX_TIMEOUT              5u                                  // Timeout al hacer lock de un mutex
#define OS_ERROR_MUTEX_LOCKED               6u                                  // El mutex esta en uso (locked)
#define OS_ERROR_MUTEX_HAVE_BLOCKED_TASKS   7u                                  // El mutex tiene procesos bloqueados

// *****************************************************************************
// Tipo de datos
// Estructura del mutex. Cada mutex es el encargado del manejo de la lista de
// procesos bloqueados POR UN MUTEX.
typedef struct os_mutex_struct{
    const int8u *               OSMutexName;                                    // Nombre del mutex.
    struct os_tcb_struct *      OSMutexCurrentOwner;                            // dueño del mutex actualmente
    struct os_tcb_struct *      OSMutexTaskParent;                              // Proceso que ha creado el mutex
    struct os_tcb_struct *      OSBlockListHead;                                // Inicio de la lista de procesos bloqueados
    struct os_tcb_struct *      OSBlockListTail;                                // Fin de la lista de procesos bloqueados
    struct os_mutex_struct *    OSMutexNext;                                    // apuntador al siguiente en la lista de mutex
    struct os_mutex_struct *    OSMutexPrev;                                    // apuntador al anterior en la lista de mutex
    bool                        OSMutexLocked;                                  // Estado del mutex binario.
}OS_MUTEX;

// *****************************************************************************
// Funciones publicas
OS_MUTEX *   OSMutexAlloc       (const int8u *name);                            //
OS_ERROR     OSMutexFree        (OS_MUTEX *mutex);                              //
OS_ERROR     OSMutexLock        (OS_MUTEX *mutex);                              //
OS_ERROR     OSMutexTryLock     (OS_MUTEX *mutex);                              //
OS_ERROR     OSMutexUnlock      (OS_MUTEX *mutex);                              //
OS_MUTEX *   OSMutexGetByName   (const int8u *name);                            //

#endif /* OS_MUTEX_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
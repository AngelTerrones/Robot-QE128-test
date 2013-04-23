/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_SEM.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 23/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Declaracion de la estructura que representa a un
 **                           semaforo.
 **
 **    Revisiones:
 **    - 23/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_SEM_H_
#define OS_SEM_H_

#include "../Sources/OS/OS.h"

// *****************************************************************************
// Constantes: codigos de error
#define OS_ERROR_SEM_NULL               1u
#define OS_ERROR_SEM_NOT_EXIST          2u
#define OS_ERROR_SEM_TIMEOUT            3u
#define OS_ERROR_SEM_HAVE_BLOCKED_TASK  4u

// *****************************************************************************
// Tipo de datos
// estuctura del semaforo.
typedef struct os_sem_struct{
    const int8u *           OSSemName;                                          // Nombre del semaforo
    int16u                  OSSemCount;                                         // Numero de cuentas  del semaforo
    struct os_tcb_struct *  OSBlockListHead;                                    // Inicio de la lista de procesos bloqueados
    struct os_tcb_struct *  OSBlockListTail;                                    // Fin de la lista de procesos bloqueados
    struct os_sem_struct *  OSSemNext;                                          // Proximo semaforo en la lista
    struct os_sem_struct *  OSSemPrev;                                          // Semaforo previo en la lista
}OS_SEM;

// *****************************************************************************
// Funciones publicas
OS_SEM *    OSSemAlloc      (const int8u *name, int16u initialCount);           // Crear un semaforo
OS_ERROR    OSSemFree       (OS_SEM *sem);                                      // Destruir un semaforo
OS_ERROR    OSSemWait       (OS_SEM *sem, int16u number);                       // Esperar por un semaforo
OS_ERROR    OSSemSignal     (OS_SEM *sem, int16u number);                       // Liberar un semaforo
OS_SEM *    OSSemGetByName  (const int8u *name);                                // Buscar y obtener un semaforo por el nombre

#endif /* OS_SEM_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
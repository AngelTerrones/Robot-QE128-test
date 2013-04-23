/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 10/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Definicion de los tipos de datos y funciones
 **                           propias del OS.
 **
 **    Revisiones:
 **    - 10/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_H_
#define OS_H_

#include "../Sources/CPU/OS_CPU.h"
#include "../Sources/OS_TICK/OS_TICK.h"

// *****************************************************************************
// Constantes
#define OS_VERSION                      "13.03.10"                              // versión del Sistema
                                                                                // (fecha de compilacion)
#define OS_MAX_TASK                     32u                                     // Numero maximo de procesos
                                                                                // Dependiente del tamaño
                                                                                // de la RAM
// Estado de procesos
#define OS_TASK_STATUS_SUSP             0x00u                                   // No ejecutando
#define OS_TASK_STATUS_READY            0x01u                                   // Esperando por CPU
#define OS_TASK_STATUS_RUN              0x02u                                   // En ejecucion (pero es un poco obvia)
#define OS_TASK_STATUS_BLOCK_DELAY      0x03u                                   // Bloqueado por delay
#define OS_TASK_STATUS_BLOCK_MUTEX      0x04u                                   // Bloqueado por mutex
#define OS_TASK_STATUS_BLOCK_SEMAPHORE  0x05u                                   // Bloqueado por semaforo
#define OS_TASK_STATUS_BLOCK_IO         0x06u                                   // Bloqueado por IO

// Codigos de Error:
#define OS_ERROR_NONE                   0x00u                                   // No error
#define OS_ERROR_TASK_NOT_EXIST         0x01u                                   // Proceso no existe
#define OS_ERROR_TASK_NAME_NULL         0x02u                                   // Nombre del proceso vacio
#define OS_ERROR_TASK_MAX_NUM           0x03u                                   // Maximo numero de procesos alcanzado
#define OS_ERROR_TASK_NO_MEMORY         0x04u                                   // No se pudo reservar memoria
#define OS_ERROR_VECTOR_NUMBER_INVALID  0x05u                                   // fuera de rango o no permitido

// *****************************************************************************
// Tipo de datos
typedef int8u   OS_ERROR;                                                       // Para devolver los codigos de error
typedef void    (*OS_TASK_PTR)(void *arg);                                      // apuntador a las funciones de los procesos

typedef struct os_til_struct{                                                   // TIL: Task Install List. Sirve para indicar los procesos a ejecutar
    OS_TASK_PTR                     OSTaskPtr;                                  // apuntador a la funcion
    const int8u *                   OSTaskName;                                 // apuntador a string con el nombre del proceso
    int16u                          OSStackSize;                                // Tamaño del stack asignado
    bool                            autostart;                                  // indicar si debe crearse al inicio
}OS_TIL;

typedef struct os_tcb_struct{                                                   // TCB: Task Control Block. Calculo aprox: 41 bytes. Necesito alinear a 4 bytes
    OS_SP *                         OSTaskSP;                                   // valor del SP al momento del switch
    OS_SP *                         OSTaskStackBottom;                          // Inicio del Stack
    OS_SP *                         OSTaskStackTop;                             // Final del Stack
    int16u                          OSStackSize;                                // Tamaño del stack (en bytes)
    int16u                          OSStackFreeSize;                            // Bytes sin usar del stack (en bytes)
    OS_TASK_PTR                     OSTaskPtr;                                  // Apuntador al inicio del proceso
    const int8u *                   OSTaskName;                                 // Apuntador al nombre del proceso.
    int32u                          OSTimeDelayMS;                              // Delay en milisegundos (49.7102 dias @ tick de 1 ms)
    struct os_mutex_struct *        OSMutexBlocking;                            // Apuntador al mutex que bloquea al proceso
    struct os_sem_struct *          OSSemBlocking;                              // Apuntador al semaforo que bloquea al proceso
    int16u                          OSSemCountBlocking;                         // Numero de cuentas por las cuales se bloquea la tarea
    struct io_device_file_struct *  IOFile;                                     // Apuntador a los archivos abiertos por el proceso
    struct os_tcb_struct *          OSTCBNext;                                  // Apuntador al proximo elemento.
    struct os_tcb_struct *          OSTCBPrev;                                  // Apuntador al anterior elemento.

    int16u                          OSTaskID;                                   // ID del task (2^16 - 1)
    int8u                           OSTaskStatus;                               // Estado del proceso.
}OS_TCB;


// *****************************************************************************
// Funciones publicas
void        __OSInit        (void);                                             // Inicializar el SO
void        __OSStart       (void);                                             // Iniciar la ejecucion del OS
OS_ERROR    OSCreateTask    (const int8u *name);                                // Crear proceso por nombre
OS_ERROR    OSDeleteTask    (const int8u *name);                                // Crear proceso por nombre

#endif /* OS_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
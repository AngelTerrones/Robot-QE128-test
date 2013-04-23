/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_CPU.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 09/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Declaración de tipos de datos, funciones y macros
 **                           dependientes del procesador utilizado.
 **
 **    Revisiones:
 **    - 09/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_CPU_H_
#define OS_CPU_H_

#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "../Sources/CPU/OS_CPU_DATA.h"                                         // Definir los tipos de datos,
                                                                                // dependientes del procesador
// *****************************************************************************
// Propiedades/Configuración del CPU
#define N_CPU_INTERRUPTS    103                                                 // Número de interrupciones del uC
                                                                                // Incluye los 64 vectores usados
                                                                                // para excepciones del CPU
#define BUS_FREQUENCY       25000000L                                           // fbus = 25 MHz. Usado para el
                                                                                // calculo del f_timer.
#define TASK_STACK_SIZE_MIN 50                                                  // 50 entradas en el stack = 200 bytes
#define HEAP_SIZE           (6*1024)                                            // 7 KB de espacio para el heap
#define BYTE_ALIGMENT       4                                                   // Procesador de 32 bits
#define BYTE_ALIGMENT_MASK  0x0003                                              // Mascara para verificar el alineamiento

// *****************************************************************************
// Macros
#define OSEnableInterrupts()    asm { move.w SR,D0; andi.l #0xF8FF,D0; move.w D0,SR; }       // habilitar interrupciones
#define OSDisableInterrupts()   asm { move.w SR,D0; ori.l #0x0700,D0; move.w D0,SR; }        // deshabilitar interrupciones
#define OSWait()                asm ( stop #0x2000; )                           // Colocar el CPU es espera
#define OSKernelPanic()         asm { stop #0x2000; }                           // Colocar el CPU es espera
#define OSSyscall()             asm {trap #14;}                                 // Fuck

// *****************************************************************************
// Funciones publicas
void        OSInitCPU               (void);                                     // Inicialización del CPU
OS_CPU_SR   OSEnterCriticalSection  (void);                                     // Entar en sección crítica
void        OSExitCriticalSection   (OS_CPU_SR cpuSR);                          // Salir de la zona crítica
OS_SP *     OSInitTaskStack         (void (*task)(void *arg),                   // Inicializar el stack de la tarea
                                     void *arg, 
                                     OS_SP *sp);             

#endif /* OS_CPU_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_CPU_DATA.h
 **    Procesador         	:
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 11/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	:
 **
 **    Revisiones:
 **    - 11/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_CPU_DATA_H_
#define OS_CPU_DATA_H_

#include "PE_Types.h"

// *****************************************************************************
// Definición de los tipos de datos dependientes del procesador
//typedef unsigned char   bool;                                                   // boolean
typedef unsigned char   int8u;                                                  // unsigned 8-bits integer
typedef signed   char   int8s;                                                  // signed 8-bits integer
typedef unsigned short  int16u;                                                 // unsigned 16-bits integer
typedef signed   short  int16s;                                                 // signed 16-bits integer
typedef unsigned long   int32u;                                                 // unsigned 32-bits integer
typedef signed   long   int32s;                                                 // signed 32-bits integer
typedef unsigned long   OS_STACK;                                               // Tamaño de una entrada en el stack
typedef signed   short  OS_CPU_SR;                                              // Valor del Status Register
typedef unsigned long   OS_SP;                                                  // Valor del Stack Pointer
typedef void            (*ISR_PTR)(void);                                       // Apuntador a las funciones de interrupcion

typedef struct isr_struct{                                                      // Para el manejo de las ISR
    ISR_PTR     isrFunction;                                                    // Apuntador al ISR (driver)
    void*       arg;                                                            // Argumento opcional del ISR
}ISR;

// *****************************************************************************
// Definición de constantes
#ifndef false
    #define false 0x00
#endif

#ifndef true
    #define true  0x01
#endif

#ifndef NULL
    #define NULL    (void *)0
#endif

#endif /* OS_CPU_DATA_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
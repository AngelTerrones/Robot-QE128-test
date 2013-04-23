/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_MEM.h
 **    Procesador         	: MCF51CN128
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

#ifndef OS_MEM_H_
#define OS_MEM_H_

#include "../Sources/OS/OS.h"
#include "../Sources/CPU/OS_CPU_DATA.h"

// *****************************************************************************
// Declaracion de funciones publicas
void        __OSMemInit     (void);                                             // Inicializar los bloques de memoria
int32u      __OSFreeMemSize (void);                                             // Cantidad de memoria libre (heap)
int32u      __OSUsedMemSize (void);                                             // Cantidad de memoria usada (heap)
void        OSMemClear      (int8u *pMem, int16u size);                         // Inicializar a cero un bloque de memoria
void        OSMemCopy       (int8u *pSource, int8u *pDest, int32u size);        // Copiar memoria
void *      OSAllocMem      (int32u size);                                      // Hacer el alloc de la memoria
void        OSFreeMem       (void *ptrMem);                                     // Liberar memoria

#endif /* OS_MEM_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_LIST.h
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

#ifndef OS_LIST_H_
#define OS_LIST_H_

#include "../Sources/OS/OS.h"

// *****************************************************************************
// Funciones publicas
void    OSInsertTCBLista    (OS_TCB *tcb, OS_TCB **head, OS_TCB **tail);
void    OSRemoveTCBLista    (OS_TCB *tcb, OS_TCB **head, OS_TCB **tail);

#endif /* OS_LIST_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
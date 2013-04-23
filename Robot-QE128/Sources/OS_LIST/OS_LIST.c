/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_LIST.c
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

#include "../Sources/OS_LIST/OS_LIST.h"

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
void OSInsertTCBLista(OS_TCB *tcb, OS_TCB **head, OS_TCB **tail)
{
    if(*head == NULL){
        *head = tcb;                                                            // primer elemento a insertar
        tcb->OSTCBPrev = NULL;                                                  // El anterior no existe
    }
    else{
        (*tail)->OSTCBNext = tcb;                                               // insertar al final
        tcb->OSTCBPrev = *tail;                                                 // Apuntar al anterior
    }
    
    *tail = tcb;                                                                // actualizar la cola
    tcb->OSTCBNext = NULL;                                                      // cerrar el eslabon   
}

/*
 * ###################################################################
 *  Función     : OSExitCriticalSection
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSRemoveTCBLista(OS_TCB *tcb, OS_TCB **head, OS_TCB **tail)
{
    if(tcb->OSTCBPrev != NULL)
        (tcb->OSTCBPrev)->OSTCBNext = tcb->OSTCBNext;                           // elemento interno
    else
        *head = tcb->OSTCBNext;                                                 // primer elemento
    
    if(tcb->OSTCBNext != NULL)
        (tcb->OSTCBNext)->OSTCBPrev = tcb->OSTCBPrev;                           // elemento interno
    else
        *tail = tcb->OSTCBPrev;                                                 // ultimo elemento
        
    tcb->OSTCBPrev = tcb->OSTCBNext = NULL;                                     // nada
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
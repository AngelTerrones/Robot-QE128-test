/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_SYSCALL.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 14/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com) 
 **    Función            	: Modulo que se encarga de definir la interrupcion
 **                           por software encargada de realizar un cambio de
 **                           contexto por bloqueos
 **
 **    Revisiones:
 **    - 14/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/CPU/OS_CPU.h"                      // NPI

// *****************************************************************************
// Variables privadas de otros modulos
extern OS_SP        *spTemp;                            // OS_TICK.c

// *****************************************************************************
// Funciones privadas
void OSSyscallISR(void);    

// *****************************************************************************
// Funciones privadas definidas en otros modulos
extern void OSScheduler(void);                          //  OS.c

// *****************************************************************************
// Implementacion de las funciones privadas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     : OSSyscallISR
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
ISR(OSSyscallISR)
{
    asm{
        LEA         -60(A7),A7              // Save processor registers onto stack        
        MOVEM.L     D0-D7/A0-A6,(A7)
        
        MOVE        A7,spTemp               // mover el SP a la variable temporal
        JSR         OSScheduler             // llamar al resto de las funciones que corresponden
        MOVE        spTemp,A7               // cargar el nuevo SP
        
        MOVEM.L     (A7),D0-D7/A0-A6        // Restore processor registers from stack     
        LEA         60(A7),A7
    }
}
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
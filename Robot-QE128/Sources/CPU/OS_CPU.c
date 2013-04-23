/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_CPU.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 09/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Implementacion de las funciones dependientes del
 **                           CPU.
 **
 **    Revisiones:
 **    - 09/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/CPU/OS_CPU.h"                                              // NPI

// *****************************************************************************
// Implementacion de las funciones publicas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     : OSInitCPU
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSInitCPU(void)
{
    OSDisableInterrupts();
}

/*
 * ###################################################################
 *  Función     : OSEnterCriticalSection
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_CPU_SR OSEnterCriticalSection(void)
{
    OS_CPU_SR cpuSR;
    asm{                                                                        // Uso de ensamblador para el manejo del SR
        MOVE.W   SR,D0                                                          // SR -> D0
        MOVE.L   D0,-(A7)                                                       // guardar en el stack
        ORI.L    #0x0700,D0                                                     // Deshabilitar las interrupciones
        MOVE.W   D0,SR                                                          // Guardar en SR
        MOVE.L   (A7)+,D0                                                       // recuperar el valor original
        MOVE.W   D0,cpuSR                                                       // guardar en la variable
    }

    return cpuSR;
}

/*
 * ###################################################################
 *  Función     : OSExitCriticalSection
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSExitCriticalSection(OS_CPU_SR cpuSR)
{
    asm
    {
        MOVE.W   cpuSR,D0                                                       // Cargar en D0
        MOVE.W   D0,SR                                                          // Mover a SR
    }
}

/*
 * ###################################################################
 *  Función     : OSExitCriticalSection
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_SP *OSInitTaskStack(void (*task)(void *arg), void *arg, OS_SP *sp)
{
    int32u originalA5;
    asm     (MOVE.L  A5, originalA5)                                            // A5 es reservado por el *#~%&& compilador.
    *sp   = 0x12345678L;                                                        // ------- SIMULATE CALL TO FUNCTION WITH ARGUMENT --------
    //*--sp = (OS_SP)arg;                                                       //    p_arg
    //*--sp = (OS_SP)task;                                                      //    Task return address
                                                                                // ------- SIMULATE INTERRUPT STACK FRAME -----------------
    *--sp = (OS_SP)task;                                                        //    Task return address
    *--sp = (0x40000000 | 0x2000);                                              //    format and status register
                                                                                // ------- SAVE ALL PROCESSOR REGISTERS -------------------
    *--sp = 0x00A600A6L;                                                        // Register A6
    *--sp = originalA5;                                                         // Register A5, Global mem ptr, linker initialized symbol
    *--sp = 0x00A400A4L;                                                        // Register A4
    *--sp = 0x00A300A3L;                                                        // Register A3
    *--sp = 0x00A200A2L;                                                        // Register A2
    *--sp = 0x00A100A1L;                                                        // Register A1
    *--sp = (OS_SP)arg;                                                         // Register A0
    *--sp = 0x00D700D7L;                                                        // Register D7
    *--sp = 0x00D600D6L;                                                        // Register D6
    *--sp = 0x00D500D5L;                                                        // Register D5
    *--sp = 0x00D400D4L;                                                        // Register D4
    *--sp = 0x00D300D3L;                                                        // Register D3
    *--sp = 0x00D200D2L;                                                        // Register D2
    *--sp = 0x00D100D1L;                                                        // Register D1
    *--sp = (OS_SP)arg;                                                         // Register D0
    return sp;                                                                  // Return pointer to new top-of-stack
}
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
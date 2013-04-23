/*
 ** ############################################################################
 **    Nombre del Proyecto  : OS
 **    Nombre de Archivo    : OS_TICK.c
 **    Procesador           : MCF51CN128
 **    Version              : v0.1
 **    Compilador           : CodeWarrior compiler 10.2
 **    Fecha                : 10/03/2013
 **    Autor(es)/email      : Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función              : Implementación de los las funciones
 **                           relacionados con el manejo del timer del sistema
 **                           (TICK)
 **
 **    Revisiones:
 **    - 10/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/OS_TICK/OS_TICK.h"

// *****************************************************************************
// Variables privadas del modulo
int16u   osTickMaxCounter = 0;                                                  // Valor maximo del contador
OS_SP   *spTemp;                                                                // temporar para guardar el sp de los procesos

// *****************************************************************************
// Funciones privadas definidas en otros modulos
extern void OSScheduler         (void);                                         // OS.c
extern void OSDelayResumeTask   (void);                                         // OS_TIME.c
extern void OSCheckStacks       (void);                                         // OS.c
extern void OSSignalTimers      (void);                                         // OS_TIMER.c

// *****************************************************************************
// Funciones privadas
void    OSTickHandler   (void);                                                 // Limpieza del timer
void    OSTimerTickISR  (void);                                                 // ISR para el manejo del TICK

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
void OSInitTick(void)
{
    // Utilizar el timer con mayor prioridad: TPM1 Channel 0, Interrupt 4
    // OJO: Se espera una configuracion del CPU de 50.33 MHz, y 
    // Fbus = 25.165 MHZ
    // A futuro: calculo de la frecuencia del bus.
    
    // calcular cuanto es el valor de la cuenta del timer para un tiempo de 1/OS_TICKS_PER_SEC
    // OJO: Supone el uso de un prescaler igual a 8
    osTickMaxCounter = BUS_FREQUENCY/(8*OS_TICK_PER_SEC);
    
    SCGC1       |=  SCGC1_TPM1_MASK;            // habilitar el clock del modulo
    TPM1SC       =  0x00;                       // Stop and reset module
    TPM1MOD      =  0x0000;                     // H:L. Free running. 
    TPM1C0SC     =  0x50;                       // Interrupt Disable. Output Compare. No output.
    TPM1C0V      =  osTickMaxCounter;           // Valor de la cuenta para fTick
    TPM1CNT      =  0x0000;                     // Reset counter
    TPM1SC       =  0x03;                       // prescale factor 8. Modulo deshabilitado
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSEnableTick(void)
{
    TPM1SC |= 0x08;                                                             // Habilitar la interrupcion del modulo
}

// *****************************************************************************
// Implementacion de las funciones privadas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSTickHandler(void)
{
    TPM1C0SC_CH0F = 0;                                                          // limpiar la interrupcion
    TPM1C0V += osTickMaxCounter;                                                // Proximo valor de interrupcion
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
ISR(OSTimerTickISR)                                                             // #$%& Codewarrior...
{ 
    asm{
        LEA         -60(A7),A7                                                  // Save processor registers onto stack
        MOVEM.L     D0-D7/A0-A6,(A7)

        MOVE        A7,spTemp                                                   // mover el SP a la variable temporal
        JSR         OSTickHandler                                               // limpiar flag
        JSR         OSSignalTimers                                              // timers
        JSR         OSDelayResumeTask                                           // Determinar si hay procesos desbloqueados por tiempo
        JSR         OSCheckStacks                                               // Verificar los stacks
        JSR         OSScheduler                                                 // Scheduling
        MOVE        spTemp,A7                                                   // cargar el nuevo SP

        MOVEM.L     (A7),D0-D7/A0-A6                                            // Restore processor registers from stack
        LEA         60(A7),A7
    }
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
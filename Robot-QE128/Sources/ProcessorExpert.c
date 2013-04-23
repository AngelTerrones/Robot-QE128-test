/** ###################################################################
**     Filename    : ProcessorExpert.c
**     Project     : ProcessorExpert
**     Processor   : MCF51QE128CLH
**     Version     : Driver 01.00
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2013-04-20, 17:00, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/* MODULE ProcessorExpert */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "IV1.h"
#include "IV2.h"
#include "MotorD1.h"
#include "MotorD2.h"
#include "MotorI1.h"
#include "MotorI2.h"
#include "Terminal.h"
#include "Encoders.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

// Operative system
#include "../Sources/OS/OS.h"

/**
 * ###################################################################
 * Funci�n      : Main
 * Descripci�n  : Funci�n principal de la aplicaci�n. Esta funci�n es
 *                ejecutada despues de la funci�n de inicializaci�n
 *                _Startup()
 * Par�metros   : Ninguno.
 * Retorna      : Nada (Jam�s debe salir de ac�)
 * ###################################################################
 */ 
void main(void)
{
    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    PE_low_level_init();
	PE_low_level_init();
    __OSInit();                                                                 // Inicializar OS.
    __OSStart();                                                                // Iniciar ejecuci�n del OS
    /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
    for(;;){}
    /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/
/*
** ###################################################################
**
**     This file was created by Processor Expert 5.3 [05.01]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/

/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : Cpu.h
**     Project     : ProcessorExpert
**     Processor   : MCF51QE128CLH
**     Component   : MCF51QE128_64
**     Version     : Component 01.010, Driver 01.12, CPU db: 3.00.080
**     Datasheet   : MCF51QE128RM, Rev. 3, 9/2007
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2013-04-23, 06:47, # CodeGen: 66
**     Abstract    :
**         This component "MCF51QE128_64" contains initialization of the
**         CPU and provides basic methods and events for CPU core
**         settings.
**     Settings    :
**
**     Contents    :
**         EnableInt  - void Cpu_EnableInt(void);
**         DisableInt - void Cpu_DisableInt(void);
**
**     Copyright : 1997 - 2012 Freescale, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __Cpu
#define __Cpu

/* MODULE Cpu. */
/* pragma to disable "possibly unassigned ISR handler" message generated by compiler on definition of ISR without vector number */
#pragma warn_absolute off

/* Active configuration define symbol */
#define PEcfg_Release_51QE128CLH 1U

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"



#define CPU_BUS_CLK_HZ              0x01800000UL /* Initial value of the bus clock frequency in Hz */

#define CPU_INSTR_CLK_HZ            0x01800000UL /* Initial value of the instruction clock frequency in Hz */

#define CPU_INT_CLK_HZ              0x8000UL /* Value of the internal oscillator clock frequency in Hz */

#define CPU_TICK_NS                 0x7736U /* CPU tick is a unit derived from the frequency of external clock source. If no external clock is enabled or available it is derived from the value of internal clock source. The value of this constant represents period of the clock source in ns. */

#define CPU_CORE_ColdFireV1            /* Specification of the core type of the selected cpu */
#define CPU_DERIVATIVE_MCF51QE128      /* Name of the selected cpu derivative */
#define CPU_PARTNUM_MCF51QE128CLH      /* Part number of the selected cpu */

/* Global variables */
extern volatile far word SR_reg;       /* Current CCR reegister */
extern volatile byte SR_lock;




__interrupt void Cpu_Interrupt(void);
/*
** ===================================================================
**     Method      :  Cpu_Cpu_Interrupt (component MCF51QE128_64)
**
**     Description :
**         This ISR services an unused interrupt/exception vector.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void __initialize_hardware(void);
/*
** ===================================================================
**     Method      :  __initialize_hardware (component MCF51QE128_64)
**
**     Description :
**         Configure the basic system functions (timing, etc.).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#define   Cpu_DisableInt() asm {move.w SR,D0; ori.l #0x0700,D0; move.w D0,SR;} /* Disable interrupts */
/*
** ===================================================================
**     Method      :  Cpu_DisableInt (component MCF51QE128_64)
**
**     Description :
**         Disables maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define   Cpu_EnableInt()  asm {move.w SR,D0; andi.l #0xF8FF,D0; move.w D0,SR;} /* Enable interrupts */
/*
** ===================================================================
**     Method      :  Cpu_EnableInt (component MCF51QE128_64)
**
**     Description :
**         Enables maskable interrupts
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void PE_low_level_init(void);
/*
** ===================================================================
**     Method      :  PE_low_level_init (component MCF51QE128_64)
**
**     Description :
**         Initializes components and provides common register 
**         initialization. The method is called automatically as a part 
**         of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

/* END Cpu. */

#endif
/* ifndef __Cpu */
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/
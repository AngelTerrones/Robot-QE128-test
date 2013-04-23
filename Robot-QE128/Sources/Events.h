/** ###################################################################
**     Filename    : Events.h
**     Project     : ProcessorExpert
**     Processor   : MCF51QE128CLH
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2013-04-20, 17:00, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "IV1.h"
#include "IV2.h"
#include "MotorD1.h"
#include "MotorD2.h"
#include "MotorI1.h"
#include "MotorI2.h"
#include "Terminal.h"
#include "Encoders.h"


void Tick_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  Tick_OnInterrupt (module Events)
**
**     Component   :  Tick [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnTrapEXP(byte ExceptionType);
/*
** ===================================================================
**     Event       :  Cpu_OnTrapEXP (module Events)
**
**     Component   :  Cpu [MCF51QE128_64]
**     Description :
**         This event is called when one of the trap exceptions occurs.
**         This event is automatically enabled when at least one of the
**         <Trap exceptions> is enabled. Disabling this event also
**         disables all the <Trap exceptions>.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ExceptionType   - This parameter
**                           specifies which trap exception caused this
**                           event. Possible values are:
**                           - TRAP_0 - Trap exception #0 
**                           - TRAP_1 - Trap exception #1 
**                           - ... 
**                           - TRAP_15 - Trap exception #15
**     Returns     : Nothing
** ===================================================================
*/

void Encoder_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  Encoder_OnInterrupt (module Events)
**
**     Component   :  Encoder [KBI]
**     Description :
**         This event is called when the active signal edge/level
**         occurs. This event is enabled only if <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Terminal_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  Terminal_OnFreeTxBuf (module Events)
**
**     Component   :  Terminal [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Terminal_OnRxChar(void);
/*
** ===================================================================
**     Event       :  Terminal_OnRxChar (module Events)
**
**     Component   :  Terminal [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 5.3 [05.01]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/

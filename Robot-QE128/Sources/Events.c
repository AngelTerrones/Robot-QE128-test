/** ###################################################################
**     Filename    : Events.c
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
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "OS/OS.h"

extern int8u contadorEncoderMD;
extern int8u contadorEncoderMI;

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
**     Returns     : Nothings
** ===================================================================
*/
void Encoder_OnInterrupt(void)
{
	/*
    int8u encoders = Encoder_GetVal();
    if(!(encoders & EncoderMotorD))
        contadorEncoderMD++;
    if(!(encoders & EncoderMotorI))
        contadorEncoderMI++;
    //Cpu_Delay100US(2);
    (void)Encoder_GetStatus();                                                  // por si acaso
    */
}
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
void  Terminal_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

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
void  Terminal_OnRxChar(void)
{
  /* Write your code here ... */
}

/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : MotorD2.h
**     Project     : ProcessorExpert
**     Processor   : MCF51QE128CLH
**     Component   : BitIO
**     Version     : Component 02.086, Driver 03.14, CPU db: 3.00.080
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2013-04-22, 15:48, # CodeGen: 52
**     Abstract    :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings    :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       25            |  PTC1_RGPIO9_TPM3CH1
**             ----------------------------------------------------
**
**         Port name                   : PTC
**
**         Bit number (in port)        : 1
**         Bit mask of the port        : 0x0002
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : up
**
**         Port data register          : PTCD      [0xFFFF8004]
**         Port control register       : PTCDD     [0xFFFF8005]
**
**         Optimization for            : speed
**     Contents    :
**         GetVal - bool MotorD2_GetVal(void);
**         PutVal - void MotorD2_PutVal(bool Val);
**         ClrVal - void MotorD2_ClrVal(void);
**         SetVal - void MotorD2_SetVal(void);
**
**     Copyright : 1997 - 2012 Freescale, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef MotorD2_H_
#define MotorD2_H_

/* MODULE MotorD2. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  MotorD2_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
#define MotorD2_GetVal() ( \
    (bool)((getReg8(PTCD) & 0x02U))    /* Return port data */ \
  )

/*
** ===================================================================
**     Method      :  MotorD2_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void MotorD2_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  MotorD2_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define MotorD2_ClrVal() ( \
    (void)setReg8(PTCCLR, 0x02U)       /* PTCCLR1=0x01U */ \
  )

/*
** ===================================================================
**     Method      :  MotorD2_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define MotorD2_SetVal() ( \
    (void)setReg8(PTCSET, 0x02U)       /* PTCSET1=0x01U */ \
  )



/* END MotorD2. */
#endif /* #ifndef __MotorD2_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/

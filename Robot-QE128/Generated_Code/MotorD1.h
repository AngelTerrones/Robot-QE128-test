/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : MotorD1.h
**     Project     : ProcessorExpert
**     Processor   : MCF51QE128CLH
**     Component   : PWM
**     Version     : Component 02.238, Driver 01.28, CPU db: 3.00.080
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2013-04-22, 16:40, # CodeGen: 54
**     Abstract    :
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
**     Settings    :
**         Used output pin             : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       26            |  PTC0_RGPIO8_TPM3CH0
**             ----------------------------------------------------
**
**         Timer name                  : TPM3 [16-bit]
**         Counter                     : TPM3CNT   [0xFFFF8061]
**         Mode register               : TPM3SC    [0xFFFF8060]
**         Run register                : TPM3SC    [0xFFFF8060]
**         Prescaler                   : TPM3SC    [0xFFFF8060]
**         Compare register            : TPM3C0V   [0xFFFF8066]
**         Flip-flop register          : TPM3C0SC  [0xFFFF8065]
**
**         User handling procedure     : not specified
**
**         Port name                   : PTC
**         Bit number (in port)        : 0
**         Bit mask of the port        : 0x0001
**         Port data register          : PTCD      [0xFFFF8004]
**         Port control register       : PTCDD     [0xFFFF8005]
**
**         Initialization:
**              Output level           : high
**              Timer                  : Disabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 25165824 Hz
**           Initial value of            period     pulse width
**             Xtal ticks              : 33         0
**             microseconds            : 1000       0
**             milliseconds            : 1          0
**             seconds (real)          : 0.001000006994 0.0
**
**     Contents    :
**         Enable     - byte MotorD1_Enable(void);
**         Disable    - byte MotorD1_Disable(void);
**         SetRatio8  - byte MotorD1_SetRatio8(byte Ratio);
**         SetRatio16 - byte MotorD1_SetRatio16(word Ratio);
**         SetDutyUS  - byte MotorD1_SetDutyUS(word Time);
**         SetDutyMS  - byte MotorD1_SetDutyMS(word Time);
**         SetValue   - byte MotorD1_SetValue(void);
**         ClrValue   - byte MotorD1_ClrValue(void);
**
**     Copyright : 1997 - 2012 Freescale, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __MotorD1
#define __MotorD1

/* MODULE MotorD1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"

#define MotorD1_PERIOD_VALUE              0x624DU /* Initial period value in ticks of the timer in high speed mode */
#define MotorD1_PERIOD_VALUE_HIGH         0x624DU /* Period value in ticks of the timer in high speed mode */


byte MotorD1_Enable(void);
/*
** ===================================================================
**     Method      :  MotorD1_Enable (component PWM)
**
**     Description :
**         This method enables the component - it starts the signal
**         generation. Events may be generated (<DisableEvent>
**         /<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte MotorD1_Disable(void);
/*
** ===================================================================
**     Method      :  MotorD1_Disable (component PWM)
**
**     Description :
**         This method disables the component - it stops the signal
**         generation and events calling. When the timer is disabled,
**         it is possible to call <ClrValue> and <SetValue> methods.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte MotorD1_SetRatio8(byte Ratio);
/*
** ===================================================================
**     Method      :  MotorD1_SetRatio8 (component PWM)
**
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as an 8-bit unsigned integer number. 0 - FF value is
**         proportional to ratio 0 - 100%. The method is available
**         only if it is not selected list of predefined values in
**         <Starting pulse width> property. 
**         Note: Calculated duty depends on the timer capabilities and
**         on the selected period.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 255 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte MotorD1_SetRatio16(word Ratio);
/*
** ===================================================================
**     Method      :  MotorD1_SetRatio16 (component PWM)
**
**     Description :
**         This method sets a new duty-cycle ratio. Ratio is expressed
**         as a 16-bit unsigned integer number. 0 - FFFF value is
**         proportional to ratio 0 - 100%. The method is available
**         only if it is not selected list of predefined values in
**         <Starting pulse width> property. 
**         Note: Calculated duty depends on the timer possibilities and
**         on the selected period.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ratio           - Ratio to set. 0 - 65535 value is
**                           proportional to ratio 0 - 100%
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/

byte MotorD1_SetDutyUS(word Time);
/*
** ===================================================================
**     Method      :  MotorD1_SetDutyUS (component PWM)
**
**     Description :
**         This method sets the new duty value of the output signal.
**         The duty is expressed in microseconds as a 16-bit
**         unsigned integer number.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Time            - Duty to set [in microseconds]
**                      (0 to 1000 us in high speed mode)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/

byte MotorD1_SetDutyMS(word Time);
/*
** ===================================================================
**     Method      :  MotorD1_SetDutyMS (component PWM)
**
**     Description :
**         This method sets the new duty value of the output signal.
**         The duty is expressed in milliseconds as a 16-bit
**         unsigned integer number.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Time            - Duty to set [in milliseconds]
**                      (0 to 1 ms in high speed mode)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/

byte MotorD1_SetValue(void);
/*
** ===================================================================
**     Method      :  MotorD1_SetValue (component PWM)
**
**     Description :
**         This method sets (set to "1" = "High") timer flip-flop
**         output signal level. It allows to the user to directly set
**         the output pin value (=flip-flop state), and can set the
**         signal polarity. This method only works when the timer is
**         disabled (<Disable> method) otherwise it returns the error
**         code. <ClrValue> and <SetValue> methods are used for setting
**         the initial state.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_ENABLED - Component is enabled.
**                           Component must be disabled (see "Disable
**                           method")
** ===================================================================
*/

byte MotorD1_ClrValue(void);
/*
** ===================================================================
**     Method      :  MotorD1_ClrValue (component PWM)
**
**     Description :
**         This method clears (set to "0" = "Low") timer flip-flop
**         output signal level. It allows to the user to directly set
**         the output pin value (=flip-flop state), and can set the
**         signal polarity. This method only works when the timer is
**         disabled (<Disable> otherwise it returns the error code.
**         <ClrValue> and <SetValue> methods are used for setting the
**         initial state.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_ENABLED - Component is enabled.
**                           Component must be disabled (see "Disable
**                           method")
** ===================================================================
*/

void MotorD1_Init(void);
/*
** ===================================================================
**     Method      :  MotorD1_Init (component PWM)
**
**     Description :
**         Initializes the associated peripheral(s) and the components 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END MotorD1. */

#endif /* ifndef __MotorD1 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/

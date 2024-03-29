/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : MotorI1.h
**     Project     : ProcessorExpert
**     Processor   : MCF51QE128CLH
**     Component   : PWM
**     Version     : Component 02.238, Driver 01.28, CPU db: 3.00.080
**     Compiler    : CodeWarrior ColdFireV1 C Compiler
**     Date/Time   : 2013-04-22, 16:42, # CodeGen: 55
**     Abstract    :
**         This component implements a pulse-width modulation generator
**         that generates signal with variable duty and fixed cycle. 
**     Settings    :
**         Used output pin             : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       21            |  PTC2_RGPIO10_TPM3CH2
**             ----------------------------------------------------
**
**         Timer name                  : TPM3 [16-bit]
**         Counter                     : TPM3CNT   [0xFFFF8061]
**         Mode register               : TPM3SC    [0xFFFF8060]
**         Run register                : TPM3SC    [0xFFFF8060]
**         Prescaler                   : TPM3SC    [0xFFFF8060]
**         Compare register            : TPM3C2V   [0xFFFF806C]
**         Flip-flop register          : TPM3C2SC  [0xFFFF806B]
**
**         User handling procedure     : not specified
**
**         Port name                   : PTC
**         Bit number (in port)        : 2
**         Bit mask of the port        : 0x0004
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
**         Enable         - byte MotorI1_Enable(void);
**         Disable        - byte MotorI1_Disable(void);
**         SetRatio8      - byte MotorI1_SetRatio8(byte Ratio);
**         SetRatio16     - byte MotorI1_SetRatio16(word Ratio);
**         SetDutyTicks16 - byte MotorI1_SetDutyTicks16(word Ticks);
**         SetDutyUS      - byte MotorI1_SetDutyUS(word Time);
**         SetDutyMS      - byte MotorI1_SetDutyMS(word Time);
**         SetValue       - byte MotorI1_SetValue(void);
**         ClrValue       - byte MotorI1_ClrValue(void);
**
**     Copyright : 1997 - 2012 Freescale, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __MotorI1
#define __MotorI1

/* MODULE MotorI1. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "Cpu.h"

#define MotorI1_PERIOD_VALUE              0x624DU /* Initial period value in ticks of the timer in high speed mode */
#define MotorI1_PERIOD_VALUE_HIGH         0x624DU /* Period value in ticks of the timer in high speed mode */


byte MotorI1_Enable(void);
/*
** ===================================================================
**     Method      :  MotorI1_Enable (component PWM)
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

byte MotorI1_Disable(void);
/*
** ===================================================================
**     Method      :  MotorI1_Disable (component PWM)
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

byte MotorI1_SetRatio8(byte Ratio);
/*
** ===================================================================
**     Method      :  MotorI1_SetRatio8 (component PWM)
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

byte MotorI1_SetRatio16(word Ratio);
/*
** ===================================================================
**     Method      :  MotorI1_SetRatio16 (component PWM)
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

byte MotorI1_SetDutyTicks16(word Ticks);
/*
** ===================================================================
**     Method      :  MotorI1_SetDutyTicks16 (component PWM)
**
**     Description :
**         The duty is expressed in main oscillator (usually
**         External osc., internal osc. if exits and external osc.
**         is disabled) ticks as a 16-bit unsigned integer number.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Ticks           - Duty to set in Xtal ticks
**                      (0 to 33 ticks in high speed mode)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_MATH - Overflow during evaluation
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/

byte MotorI1_SetDutyUS(word Time);
/*
** ===================================================================
**     Method      :  MotorI1_SetDutyUS (component PWM)
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

byte MotorI1_SetDutyMS(word Time);
/*
** ===================================================================
**     Method      :  MotorI1_SetDutyMS (component PWM)
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

byte MotorI1_SetValue(void);
/*
** ===================================================================
**     Method      :  MotorI1_SetValue (component PWM)
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

byte MotorI1_ClrValue(void);
/*
** ===================================================================
**     Method      :  MotorI1_ClrValue (component PWM)
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

void MotorI1_Init(void);
/*
** ===================================================================
**     Method      :  MotorI1_Init (component PWM)
**
**     Description :
**         Initializes the associated peripheral(s) and the components 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END MotorI1. */

#endif /* ifndef __MotorI1 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale ColdFireV1 series of microcontrollers.
**
** ###################################################################
*/

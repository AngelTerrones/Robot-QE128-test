/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: Application.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 11/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Funcion            	: Implementacion de los procesos de usuario.
 **
 **    Revisiones:
 **    - 11/03/2013, v0.1
 **         - Creacion del archivo.
 **    - 17/03/2013, v0.1
 **         - Modificada la lista de tareas a ejecutar: Agregado el tamaño del
 **           stack.
 **    - 24/03/2013, v0.1
 **         - Eliminadas las tareas 3 y 4.
 **         - Creados los timers 1 y 2, para sustituir las tareas eliminadas.
 ** ############################################################################
 */

#include "../Sources/OS/OS.h"
#include "../Sources/OS_TIMER/OS_TIMER.h"
#include "../Sources/OS_TIME/OS_TIME.h"

// Drivers
#include "MotorD1.h"
#include "MotorD2.h"
#include "MotorI1.h"
#include "MotorI2.h"
#include "Encoders.h"
#include "Terminal.h"

// ****************************************************************************
// Declaracion de los procesos
// ****************************************************************************
void    MainTask        (void *arg);
void    TimerPIDMotor   (void);

// ****************************************************************************
// Creacion de la lista de procesos a instalar
// NOTA: EN ESTA LISTA NO PUEDEN EXISTIR DOS PROCESOS CON EL MISMO NOMBRE!!!!
// ****************************************************************************
// Es un poco triste el que tenga que hacer la conversion en forma explicita...
const OS_TIL osTaskInstallList[] =
{
    {MainTask, (const int8u *)"MainTask", TASK_STACK_SIZE_MIN, true},           // Tarea principal
    {NULL    , NULL      , false}                                               // fin de la lista
};

// *****************************************************************************
// Variables globales
volatile int8u contadorEncoderMD = 0;        // Valor de la cuenta de los encoders
volatile int8u contadorEncoderMI = 0;        //

int8u	errorMD = 0;
int8u	errorMI = 0;

#define	EncoderD	0x01
#define	EncoderI	0x02

// *****************************************************************************
// Procesos de usuario
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void MainTask(void *arg)
{
    OS_ERROR err;
    OS_TIMER *timerPIDMotor = OSTimerCreate(500, true, TimerPIDMotor, &err);
    int8u	estadoAnterior = Encoders_GetVal();
    int8u	estadoActual = 0;

    (void)arg;
    MotorD1_Enable();                                                           // Habilitar las salidas de PWM
    MotorI1_Enable();
    Terminal_EnableEvent();
    Terminal_Enable();
    OSTimerStart(timerPIDMotor);                                               	// Ejecutar el timer para el control de motores

    for(;;){
        OSDelayMS(5);                                                    
        estadoActual = Encoders_GetVal();
        
        if(!(estadoAnterior & EncoderD) && (estadoActual & EncoderD))
        	contadorEncoderMD++;
        if(!(estadoAnterior & EncoderI) && (estadoActual & EncoderI))
			contadorEncoderMI++;
        estadoAnterior = estadoActual;
    }
}

// *****************************************************************************
// Callbacks de los timers
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void TimerPIDMotor(void)
{
    // frecuencia de operacion: 10 Hz. Limitacion por la resolucion del timer: 10 Hz.
    // Si de desea aumentar, modificar la resolución en OS_TIMER.c
    int16u count = 0;
    int16u dutyMD = 0;
    int16u dutyMI = 0;
    int8u  ref = 11;                                        // 12 cuentas x rev
    int16u  Kp = 10;
    int16u  Ki = 2;
    int8u  freq = 2;
    
    if(contadorEncoderMD*freq > ref){
        dutyMD = (int16u)((contadorEncoderMD*freq - ref)*Kp);
    }
    else{
        dutyMD = (int16u)((ref - contadorEncoderMD*freq)*Kp);        
    }
    errorMD += ref - contadorEncoderMD*freq;
    
    if(contadorEncoderMI*freq > ref){
        dutyMI = (int16u)((contadorEncoderMI*freq - ref)*Kp);
    }
    else{ 
        dutyMI = (int16u)((ref - contadorEncoderMI*freq)*Kp);
    }
    errorMI += ref - contadorEncoderMI*freq;
    
    dutyMD += errorMD*Ki;
    dutyMI += errorMI*Ki;
    
    if(dutyMD > 0xAA)
        dutyMD = 0xAA;
    if(dutyMI > 0xAA)
        dutyMI = 0xAA;
    MotorD1_SetRatio8((int8u)dutyMD);
    MotorI1_SetRatio8((int8u)dutyMI);    
    //Terminal_SendBlock((const int8u *)&contadorEncoderMD, 1, &count);
    //Terminal_SendBlock((const int8u *)&contadorEncoderMI, 1, &count);
    
    contadorEncoderMD = 0;
    contadorEncoderMI = 0; 
}
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */

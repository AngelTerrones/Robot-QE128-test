/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_TIME.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 13/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com)
 **    Función            	: Declaración de los tipos de datos y
 **                           funciones relacionados con el manejo del timer del
 **                           sistema (TICK)
 **
 **    Revisiones:
 **    - 13/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_TIME_H_
#define OS_TIME_H_

#include "../Sources/OS/OS.h"
#include "../Sources/CPU/OS_CPU.h"

// *****************************************************************************
// Constantes: Códigos de error
#define OS_ERROR_TIME_INVALID_MIN   0x01u                                       // Minutos fuera del rango
#define OS_ERROR_TIME_INVALID_SEG   0x02u                                       // Segundos fuera del rango
#define OS_ERROR_TIME_ZERO_DELAY    0x03u                                       // No tiempo configurado

// *****************************************************************************
// Funciones publicas
void        OSDelayMS   (int32u delay);                                         // delay milisegundos
void        OSDelayS    (int32u delay);                                         // delay segundos
OS_ERROR    OSDelayHMS  (int8u hour, int8u minutes, int8u seconds);             // Delay HH:MM:SS

#endif /* OS_TIME_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
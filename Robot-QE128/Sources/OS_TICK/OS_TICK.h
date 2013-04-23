/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_TICK.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 10/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com) 
 **    Función            	: Defincion de tipos de datos y funciones asociados
 **                           con el manejo del TICK del sistema.
 **
 **    Revisiones:
 **    - 10/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_TICK_H_
#define OS_TICK_H_

#include "../Sources/CPU/OS_CPU.h"

// *****************************************************************************
// Constantes
#define OS_TICK_PER_SEC             1000u                                       // Definir timer de 1 ms
#define OS_TICK_PRESCALER           128                                         // Valor del prescaler utilizado
#define OS_MAX_TICK_COUNTER         BUS_FREQUENCY/(OS_TICK_PRESCALER*OS_TICK_PER_SEC)

// *****************************************************************************
// Funciones dependientes de la arquitectura
void    OSInitTick      (void);                                                 // Inicializacion del timer
void    OSEnableTick    (void);                                                 // Habilitar el tick

#endif /* OS_TICK_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_STRING.h
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 11/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com) 
 **    Función            	:
 **
 **    Revisiones:
 **    - 11/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#ifndef OS_STRING_H_
#define OS_STRING_H_

#include "../Sources/CPU/OS_CPU_DATA.h"

// *****************************************************************************
// Funciones publicas
bool     OSStrCmp   (const int8u *str1, const int8u *str2);                     // Comparar dos null terminated strings
int8u    OSStrLen   (const int8u *str);                                         // Determinar la la longitud del string

#endif /* OS_STRING_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
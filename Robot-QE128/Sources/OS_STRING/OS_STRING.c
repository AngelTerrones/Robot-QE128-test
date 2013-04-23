/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_STRING.c
 **    Procesador         	: 
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

#include "../Sources/OS_STRING/OS_STRING.h"

// *****************************************************************************
// Implementacion de las funciones publicas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     : OSStrCmp
 *  Descripción : 
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
bool OSStrCmp(const int8u *str1, const int8u *str2)
{
    // Determinar si uno de los dos es un string nulo
    if(str1  == NULL || str2 == NULL)
        return false;   
    // Si las longitudes son diferentes, obviamente no son iguales.
    // Despues de la verificacion del null pointer, para evitar
    // bug por str1 = str2 = null;
    // Creo que las dos lineas de codigo mas abajo no son necesarias.
    //if(OSStrLen(str1) != OSStrLen(str2))
    //    return false;   
    // hasta que se encuentre el final del string
    while(*str1 != 0 && *str2 != 0){
        if(*str1 != *str2)  // si son diferentes, devolver false
            return false;
        str1++;             // proximo caracter
        str2++;
    }  
    return true;            // ambos strings son iguales.
}

/*
 * ###################################################################
 *  Función     : 
 *  Descripción : 
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
int8u OSStrLen(const int8u *str)
{
    int8u size = 0;
    
    if(str == NULL)             // Null pointer
        return 0;                       
    while(*(str++) != 0)        // Mientras no se termine el string
        size++;        
    return size;                // Obviamente, devolver el tamaño. DA!
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
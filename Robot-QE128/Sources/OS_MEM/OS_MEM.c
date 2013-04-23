/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_MEM.c
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
#include "../Sources/OS/OS.h"
#include "../Sources/OS_MEM/OS_MEM.h"

// *****************************************************************************
// Variables privadas del modulo.
int8u   heap[HEAP_SIZE];                                                        // Heap   
int32u  heapBytesUsed;                                                          // Numero de bytes usados

// *****************************************************************************
// Implementacion de las funciones publicas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void __OSMemInit(void)
{
    heapBytesUsed = 0;                                                          // Las primeras posiciones de memoria
}

/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
int32u __OSFreeMemSize(void)
{
    return HEAP_SIZE - heapBytesUsed;
}
/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
int32u __OSUsedMemSize(void)
{
    return heapBytesUsed;
}

/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSMemClear(int8u *pMem, int16u size)
{
    while(size > 0){                                                            // Mientras el tamaño sea mayor a cero
        *pMem++ = 0;                                                            // Colocar a cero e incrementar la direccion
        size--;                                                                 // Disminuir la cuenta de bytes que hay que borrar
    }
}

/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSMemCopy(int8u *pSource, int8u *pDest, int32u size)
{
    while(size > 0){
        *pDest++ = *pSource++;
        size--;
    }
}

/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void *OSAllocMem(int32u size)
{
    OS_CPU_SR sr;                                                               // zona critica
    void *ptrMem = NULL;
    
    if(size & BYTE_ALIGMENT_MASK)
        size += BYTE_ALIGMENT - (size & BYTE_ALIGMENT_MASK);                    // Ajustar el tamaño
    sr = OSEnterCriticalSection();                                              // Entrar en la zona critica
        if(heapBytesUsed + size < HEAP_SIZE){                                   // Queda espacio
            ptrMem = heap + heapBytesUsed;                                      // Devolver esta posicion de memoria
            heapBytesUsed += size;                                              // Incrementar la cuenta de bytes usados
        }
    OSExitCriticalSection(sr);                                                  // salir de la zona critica
    return ptrMem;
}
/*
 * ###################################################################
 *  Función     : 
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSFreeMem(void *ptrMem)
{
    (void)ptrMem;                                                               // No hace nada.
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
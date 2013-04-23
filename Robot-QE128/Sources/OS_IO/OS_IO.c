/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_IO.c
 **    Procesador         	: MCF51CN128
 **    Version            	: v0.1
 **    Compilador         	: CodeWarrior compiler 10.2
 **    Fecha              	: 15/03/2013
 **    Autor(es)/email    	: Angel Terrones (aterrones@usb.ve,
 **                                           angelterrones@gmail.com) 
 **    Función            	:
 **
 **    Revisiones:
 **    - 15/03/2013, v0.1
 **         - Creación del archivo.
 ** ############################################################################
 */

#include "../Sources/OS_IO/OS_IO.h"
#include "../Sources/OS_MEM/OS_MEM.h"
#include "../Sources/OS_MUTEX/OS_MUTEX.h"
#include "../Sources/OS_LIST/OS_LIST.h"

// *****************************************************************************
// Variables privadas de otros modulos
extern const    IO_DEVICE_INSTALL ioDeviceInstallList[];                        // Lista de drivers que se deben instalar
extern OS_TCB * osTaskCurrent;                                                  // Proceso actual

// *****************************************************************************
// Variables privadas del modulo
IO_DEVICE * ioDeviceListHead;                                                   // Inicio de la lista de dispositivos instalados
IO_DEVICE * ioDeviceListTail;                                                   // Final  de la lista de dispositivos instalados
IO_FILE *   ioFileListHead;                                                     // Inicio de la lista de archivos creados
IO_FILE *   ioFileListTail;                                                     // Final  de la lista de archivos creados
OS_MUTEX *  mutexIO;                                                            // Mutex para proteger el acceso a la lista de dispositivos

// *****************************************************************************
// Funciones privadas definidas en otros modulos
extern void OSInsertTaskReadyList(OS_TCB *tcb, bool prioridad);                 // OS.c

// *****************************************************************************
// Funciones privadas del modulo
void        IOInsertTaskBlockIOList (OS_TCB *tcb);
void        IORemoveTaskBlockIOList (OS_TCB *tcb);
void        IOInsertDevice          (IO_DEVICE *device);
void        IORemoveDevice          (IO_DEVICE *device);
void        IOInsertFile            (IO_FILE *file);
void        IORemoveFile            (IO_FILE *file);

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
void __IODeviceBlockTask(void)
{
    OS_CPU_SR sr  = OSEnterCriticalSection();                                   // enter critical section   
        IOInsertTaskBlockIOList(osTaskCurrent);
        OSSyscall();  
    OSExitCriticalSection(sr);                                                  // exit critical section    
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void __IODeviceUnblockTask(IO_FILE *fileBlock)
{
    OS_TCB *tcb = fileBlock->IOBlockListHead;   
    
    if(tcb != NULL){
        IORemoveTaskBlockIOList(tcb);                                           // sacar de la lista de procesos bloqueados
        OSInsertTaskReadyList(tcb, true);                                       // insertar en lista de ready
        tcb->IOFile = NULL;                                                     // sin archivo por el cual esperar
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
void __IOInitDevices()
{
    int8u index = 0;
    IO_DEV_INSTALL_FUNCTION_PTR ioInstall;
    IO_ERROR error;

    // iterar por toda la lista y ejecutar la funcion de instalacion
    while(ioDeviceInstallList[index].IODeviceName != NULL){
        ioInstall = ioDeviceInstallList[index].IODeviceInstall;
        error = (*ioInstall)(ioDeviceInstallList[index].IODeviceName);        
        if(ioInstall == NULL || error)
            OSKernelPanic();       
        index++;
    }    
    mutexIO = OSMutexAlloc((const int8u *)"mutexIO");                           // WHY?? No deberia hacer el casting...
    if(mutexIO == NULL)
        OSKernelPanic();
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
IO_ERROR IODeviceInstall(int8u *                    name,
                         IO_DEV_OPEN_FUNCTION_PTR   ioOpen,
                         IO_DEV_CLOSE_FUNCTION_PTR  ioClose,
                         IO_DEV_READ_FUNCTION_PTR   ioRead, 
                         IO_DEV_WRITE_FUNCTION_PTR  ioWrite,
                         IO_DEV_IOCTL_FUNCTION_PTR  ioIoctl)
{
    IO_DEVICE *device;
    
    // verificar si alguno de los parametros es nulo
    if(name == NULL)
        return IO_ERROR_NULL_NAME;
    if(ioOpen == NULL)
        return IO_ERROR_NULL_OPEN_FUNCTION;
    if(ioClose == NULL)
        return IO_ERROR_NULL_CLOSE_FUNCTION;
    if(ioRead == NULL)
        return IO_ERROR_NULL_READ_FUNCTION;
    if(ioWrite == NULL)
        return IO_ERROR_NULL_WRITE_FUNCTION;
    if(ioIoctl == NULL)
        return IO_ERROR_NULL_IOCTL_FUNCTION;
    
    // pedir un bloque para el dispositivo
    device = (IO_DEVICE *)OSAllocMem(sizeof(IO_DEVICE));
    if(device == NULL)
        return IO_ERROR_NO_ALLOC_DEVICE;
    
    // inicializarlo    
    device->IODeviceName = name;
    device->IODeviceOpen = ioOpen;
    device->IODeviceClose = ioClose;
    device->IODeviceRead = ioRead;
    device->IODeviceWrite = ioWrite;
    device->IODeviceIoctl = ioIoctl;  
    IOInsertDevice(device);                                                     // insertar el driver en la lista de instalados.        
    return IO_ERROR_DEVICE_NONE;    
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
IO_FILE *IODeviceOpen(int8u *name, IO_DEV_FLAGS flags)
{
    OS_CPU_SR   sr;
    IO_FILE    *file;
    IO_DEVICE  *device = ioDeviceListHead;                                      // inicio de la tabla a buscar
    int8u      *nameDev;
    int8u      *nameTmp;
    IO_ERROR    error;

    if(name == NULL)
        return NULL;
        
    if(OSMutexLock(mutexIO) != OS_ERROR_NONE)
        return NULL;
        
    // buscar el dispositivo por nombre. El formato es XXXX:YY, donde 
    // XXXX -> Nombre del driver
    // YY   -> Canal a utilizar
    while(device != NULL){
        nameDev = device->IODeviceName;                                         // nombre a comparar
        nameTmp = name;                                                         // nombre a buscar      
        // comparar hasta que alguno llegue a "\0" o sean diferentes            
        while(*nameDev && *nameTmp && (*nameDev == *nameTmp)){
            nameDev++;                                                          // incrementar el apuntador
            nameTmp++;
        }
        if(!(*nameDev))                                                         // Coincide el nombre del dispositivo, resta
            break;                                                              // el canal en el apuntador nameTmp        
        device = device->IODeviceNext;                                          // siguiente a buscar
    }
    (void)OSMutexUnlock(mutexIO);    
    
    if(device == NULL)
        return NULL;                                                            // no se encontro en la lista de dispositivos
    
    file = (IO_FILE *)OSAllocMem(sizeof(IO_FILE));                              // solicitar archivo
    
    if(file == NULL)
        return NULL;
   
    // NOTA: EL ACCESO AL HW DEBE SER PROTEGIDO
    //       En teoria, esto debería hacerlo el mismo driver.
    sr = OSEnterCriticalSection(); 
        file->IODevice = device;
        error = (*device->IODeviceOpen)(file, nameTmp, flags);                  // OJO: VERIFICAR
    OSExitCriticalSection(sr);
    
    if(error != IO_ERROR_DEVICE_NONE)
    {
        OSFreeMem(file);
        return NULL;
    }
    
    return file;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
IO_ERROR IODeviceClose(IO_FILE *file)
{
    OS_CPU_SR   sr;
    IO_DEVICE *device;
    IO_ERROR   error;
    
    if(file == NULL)                                                            // null pointer?  
        return IO_ERROR_FILE_NULL_POINTER;
    
    device = file->IODevice;
    if(device == NULL)
        return IO_ERROR_DEVICE_NULL_POINTER;
    
    sr = OSEnterCriticalSection(); 
        error = (*device->IODeviceClose)(file);
    OSExitCriticalSection(sr);    
    
    if(error == IO_ERROR_DEVICE_NONE)
        OSFreeMem(file);
    
    return error;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
IO_ERROR IODeviceRead(IO_FILE *file, int8u *buffer, int16u size)
{
    IO_DEVICE *device;
    
    if(file == NULL)                                                            // null pointer?  
        return IO_ERROR_FILE_NULL_POINTER;
    
    if(buffer == NULL)
        return IO_ERROR_BUFFER_NULL_POINTER;
    
    if(size < 1)
        return IO_ERROR_INVALID_SIZE;
    
    device = file->IODevice;
    if(device == NULL)
        return IO_ERROR_DEVICE_NULL_POINTER;
    
    osTaskCurrent->IOFile = file;                                               // posible bloqueo?
    return (*device->IODeviceRead)(file, buffer, size);
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
IO_ERROR IODeviceWrite(IO_FILE *file, const int8u *buffer, int16u size)
{
    IO_DEVICE *device;
        
    if(file == NULL)                                                            // null pointer?  
        return IO_ERROR_FILE_NULL_POINTER;
    
    if(buffer == NULL)
        return IO_ERROR_BUFFER_NULL_POINTER;
    
    if(size < 1)
        return IO_ERROR_INVALID_SIZE;
        
    device = file->IODevice;
    if(device == NULL)
        return IO_ERROR_DEVICE_NULL_POINTER; 
    
    osTaskCurrent->IOFile = file;                                               // posible bloqueo?
    return (*device->IODeviceWrite)(file, buffer, size);
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
IO_ERROR IODeviceIoctl(IO_FILE *file, IO_IOCTL_CMD cmd, void *arg)
{
    IO_DEVICE *device;
    IO_ERROR   error = IO_ERROR_DEVICE_NONE;
    
    if(file == NULL)                                                            // null pointer?  
        return IO_ERROR_FILE_NULL_POINTER;
    
    device = file->IODevice;
    if(device == NULL)
        return IO_ERROR_DEVICE_NULL_POINTER;
    
    switch(cmd){
        case IO_IOCTL_GET_FLAGS:
        *(IO_DEV_FLAGS *)arg = file->IODeviceFlags;
            break;
//        case IO_IOCTL_SET_FLAGS:    
//            break;
        default:
            error = (*device->IODeviceIoctl)(file, cmd, arg);
            break;
    }
    
    return error;
}

// *****************************************************************************
// Implementacion de las funciones privadas
// *****************************************************************************
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void IOInsertTaskBlockIOList(OS_TCB *tcb)
{
    IO_FILE *file = tcb->IOFile;
    OSInsertTCBLista(tcb, &(file->IOBlockListHead), &(file->IOBlockListTail));
    tcb->OSTaskStatus = OS_TASK_STATUS_BLOCK_IO;
}
    
/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void IORemoveTaskBlockIOList(OS_TCB *tcb)
{
    IO_FILE *file = tcb->IOFile;
    OSRemoveTCBLista(tcb, &(file->IOBlockListHead), &(file->IOBlockListTail));    
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void IOInsertDevice(IO_DEVICE *device)
{
    if(ioDeviceListHead)                                                        // insertar el dispositivo creado
        ioDeviceListTail->IODeviceNext = device;                                // en la lista
    else
        ioDeviceListHead = device;
    
    ioDeviceListTail = device;
    device->IODeviceNext = NULL;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void IORemoveDevice(IO_DEVICE *device)
{
    // NO IMPLEMENTADA.
    // La eliminacion en "runtime" del driver NO ESTA IMPLEMENTADA
    // Por lo tanto, esta funcion es totalmente inutil.
    (void)device;       
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void IOInsertFile(IO_FILE *file)
{
    if(file == NULL)
        return;
    
    if(ioFileListHead != NULL){
        ioFileListTail->IOFileNext = file;                                      // insertar al final
        file->IOFilePrev = ioFileListTail;
    }
    else{                                                                       // lista vacia
        ioFileListHead = file;                                                  // insertar primero
        file->IOFilePrev = NULL;
    }
    
    ioFileListTail = file;                                                      // actualizar apuntador
    file->IOFileNext = NULL;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void IORemoveFile(IO_FILE *file)
{
    if(file == NULL)
        return;
    
    if(file->IOFilePrev != NULL)
        (file->IOFilePrev)->IOFileNext = file->IOFileNext;
    else
        ioFileListHead = file->IOFileNext;
    
    if(file->IOFileNext != NULL)
        (file->IOFileNext)->IOFilePrev = file->IOFilePrev;
    else
        ioFileListTail = file->IOFilePrev;
    
    file->IOFilePrev = file->IOFileNext = NULL;
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
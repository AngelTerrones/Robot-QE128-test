/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS_IO.h
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

#ifndef OS_IO_H_
#define OS_IO_H_

#include "../Sources/OS/OS.h"

// *****************************************************************************
// Constantes: codigos de error
#define IO_ERROR_DEVICE_NONE                0u                                  // OK
#define IO_ERROR_DEVICE_NO_EXIST            1u                                  // Driver no existe
#define IO_ERROR_INSTALL                    2u                                  // no se pudo instalar un dispositivo
#define IO_ERROR_NULL_NAME                  3u
#define IO_ERROR_NULL_OPEN_FUNCTION         4u
#define IO_ERROR_NULL_CLOSE_FUNCTION        5u
#define IO_ERROR_NULL_READ_FUNCTION         6u
#define IO_ERROR_NULL_WRITE_FUNCTION        7u
#define IO_ERROR_NULL_IOCTL_FUNCTION        8u
#define IO_ERROR_NO_ALLOC_DEVICE            9u
#define IO_ERROR_DEVICE_NULL_POINTER       10u
#define IO_ERROR_FILE_NULL_POINTER         11u
#define IO_ERROR_INVALID_IOCTL_CMD         12u
#define IO_ERROR_BUFFER_NULL_POINTER       13u
#define IO_ERROR_INVALID_SIZE              14u
#define IO_ERROR_LOCK_DEVICE_LIST          15u
#define IO_ERROR_DEVICE_ALREADY_OPEN       16u
#define IO_ERROR_DEVICE_READ_ERROR         17u
#define IO_ERROR_DEVICE_WRITE_ERROR        18u
#define IO_ERROR_CORRUPT_FILE              19u
#define IO_ERROR_INVALID_FLAG              20u
#define IO_ERROR_WRITE_BUSY                21u
#define IO_ERROR_BUFFER_OVERFLOW           22u
#define IO_ERROR_NULL_ARGUMENT             23u

// flags basicos comunes
#define IO_READ_ONLY                        1u                                  // read only
#define IO_WRITE_ONLY                       2u                                  // write only
#define IO_READ_WRITE                       3u                                  // R/W

// comandos ioctl comunes
#define IO_IOCTL_GET_FLAGS                  1u
#define IO_IOCTL_SET_FLAGS                  2u

// *****************************************************************************
// Tipo de datos
// tipos de datos basicos
typedef int8u   IO_ERROR;
typedef int16u  IO_DEV_FLAGS;
typedef int8u   IO_IOCTL_CMD;
// -----------------------------------------------------------------------------
// estructura que representa el "archivo" para el manejo del dispositivo: 24 bytes
typedef struct io_device_file_struct{
    IO_DEV_FLAGS                     IODeviceFlags;                             // Flags de configuracion
    struct io_device_struct         *IODevice;                                  // Apuntador al dispositivo
    struct os_tcb_struct            *IOBlockListHead;                           // Inicio de la cola de procesos bloqueados por I/O
    struct os_tcb_struct            *IOBlockListTail;                           // Final  de la cola de procesos bloqueados por I/O
    struct io_device_file_struct    *IOFileNext;                                // apuntador al siguiente de la lista
    struct io_device_file_struct    *IOFilePrev;                                // apuntador al anterior de la lista
}IO_FILE;

// -----------------------------------------------------------------------------
// funciones que deben implementar los diferentes modulos/drivers
typedef IO_ERROR (*IO_DEV_INSTALL_FUNCTION_PTR) (int8u *name);
typedef IO_ERROR (*IO_DEV_OPEN_FUNCTION_PTR)    (IO_FILE *file, int8u *name, IO_DEV_FLAGS flags);
typedef IO_ERROR (*IO_DEV_CLOSE_FUNCTION_PTR)   (IO_FILE *file);
typedef IO_ERROR (*IO_DEV_READ_FUNCTION_PTR)    (IO_FILE *file, int8u *buffer, int16u size);
typedef IO_ERROR (*IO_DEV_WRITE_FUNCTION_PTR)   (IO_FILE *file, const int8u *buffer, int16u size);
typedef IO_ERROR (*IO_DEV_IOCTL_FUNCTION_PTR)   (IO_FILE *file, IO_IOCTL_CMD cmd, void* arg);

// -----------------------------------------------------------------------------
// estructura de datos que representa al dispositivo: 28 bytes
typedef struct io_device_struct{
    int8u *                          IODeviceName;                              // nombre del dispositivo
    IO_DEV_OPEN_FUNCTION_PTR         IODeviceOpen;                              // funcion para abrir      el dispositivo
    IO_DEV_CLOSE_FUNCTION_PTR        IODeviceClose;                             //              cerrar
    IO_DEV_READ_FUNCTION_PTR         IODeviceRead;                              //              leer
    IO_DEV_WRITE_FUNCTION_PTR        IODeviceWrite;                             //              escribir
    IO_DEV_IOCTL_FUNCTION_PTR        IODeviceIoctl;                             //              configurar
    struct io_device_struct *        IODeviceNext;                              // Apuntador al siguiente de la lista
}IO_DEVICE;

// -----------------------------------------------------------------------------------------------------------
// Estructura utilizada para la instalacion de los drivers: 8 bytes
typedef struct io_device_install_struct{
    int8u *                          IODeviceName;                              // Nombre del dispositivo
    IO_DEV_INSTALL_FUNCTION_PTR      IODeviceInstall;                           // Apuntador a la funcion de instalacion
}IO_DEVICE_INSTALL;

// *****************************************************************************
// Funciones publicas
void        __IODeviceBlockTask     (void);                                     //
void        __IODeviceUnblockTask   (IO_FILE *fileBlock);                       //
//void        __IOInitDeviceFreeTable (void);                                     //
//void        __IOInitFileFreeTable   (void);                                     //
void        __IOInitDevices         (void);                                     //

// funciones basicas disponibles al usuario
IO_ERROR    IODeviceInstall         (int8u *                    name,           // Funcion encargada de instalar el driver
                                     IO_DEV_OPEN_FUNCTION_PTR   ioOpen,
                                     IO_DEV_CLOSE_FUNCTION_PTR  ioClose,
                                     IO_DEV_READ_FUNCTION_PTR   ioRead, 
                                     IO_DEV_WRITE_FUNCTION_PTR  ioWrite,
                                     IO_DEV_IOCTL_FUNCTION_PTR  ioIoctl);
IO_FILE *   IODeviceOpen            (int8u *        name,                       // Abrir el dispositivo
                                     IO_DEV_FLAGS   flags);
IO_ERROR    IODeviceClose           (IO_FILE *      file);                      // Cerrar el dispositivo 
IO_ERROR    IODeviceRead            (IO_FILE *      file,                       // Leer el dispositivo
                                     int8u *        buffer, 
                                     int16u         size);
IO_ERROR    IODeviceWrite           (IO_FILE *      file,                       // Escribir en el archivo
                                     const int8u *  buffer, 
                                     int16u         size);
IO_ERROR    IODeviceIoctl           (IO_FILE *      file,                       // Indicar comandos de control al dispositivo
                                     IO_IOCTL_CMD   cmd, 
                                     void*          arg);

#endif /* OS_IO_H_ */
/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
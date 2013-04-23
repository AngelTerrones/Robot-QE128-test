/*
 ** ############################################################################
 **    Nombre del Proyecto	: OS
 **    Nombre de Archivo  	: OS.c
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
 **    - 16/03/2013, v0.1
 **         - Modificacion de las funciones encargadas de crear los procesos
 **           para que usar la nueva funcion OSAllocMem().
**     - 17/03/2013, v0.1
**          - Arreglado un bug con la creacion de los stacks de los procesos.
**          - Funciones para crear ahora exigen el tamaño del stack a asignar
**            a cada proceso.
**          - Arreglado un bug en el alloc de memoria del proceso de estadistica
**          - La creacion de los procesos de usuario se mueve para la tarea de
**            estatistica. A futuro: cambiarle el nombre por uno mas generico,
**            ya que si no, dicha funcion no queda clara.
 ** ############################################################################
 */

#include "../Sources/OS/OS.h"
#include "../Sources/OS_IO/OS_IO.h"
#include "../Sources/OS_MEM/OS_MEM.h"
#include "../Sources/OS_LIST/OS_LIST.h"
#include "../Sources/OS_STRING/OS_STRING.h"
#include "../Sources/OS_TIME/OS_TIME.h"
#include "../Sources/OS_TIMER/OS_TIMER.h"
#include "../Sources/OS_SEM/OS_SEM.h"

// *****************************************************************************
// Variables privadas del sistema
OS_TCB *        osTaskReadyListHead = NULL;                                     // Inicio de la lista enlazada
OS_TCB *        osTaskReadyListTail = NULL;                                     // Final de la lista enlazada
OS_TCB *        osTaskCurrent       = NULL;                                     // Proceso en Actual ejecucion
OS_TCB *        osTaskOld           = NULL;                                     // Proceso en Actual ejecucion
int8u           osNumberTask        = 0;                                        // Numero de procesos en ejecucion
int16u          osCurrentPID        = 0;                                        // Empezar por el proceso 1
const int8u     osIdleTaskName[]    = "Idle Task";                              // Nombre del proceso ocioso
const int8u     osStatTaskName[]    = "Stat & Init Task";                       // Nombre del proceso de estadisticas
const int8u     osTimerTaskName[]   = "Timer Task";                             // Nombre del proceso encargado del manejo del timer
volatile int32u osIdleCounter       = 0;                                        // Contador del idle task
int32u          osIdleCounterMax    = 0;                                        // Valor del contador maximo para determinar el uso del CPU
int32u          osIdleCounterNow    = 0;                                        // Valor del contador despues del tiempo de calculo
int8u           osCPUUsage          = 0;                                        // Valor del uso del CPU

// *****************************************************************************
// Variables privadas en otros modulos
extern OS_SP *          spTemp;                                                 // Definida en el modulo OS_TICK.c
extern const OS_TIL     osTaskInstallList[];                                    // Lista con las tareas a instalar
                                                                                // OJO: el dato debe estar declarado en el archivo
                                                                                // del usuario
// *****************************************************************************
// Funciones privadas
void        OSInitHW                (void);                                     // Inicializar el hardware
void        OSInitAutostartTask     (void);                                     // Crear los procesos marcados como autostart
OS_ERROR    OSInitTask              (const int8u *  osTaskName,                 // Inicializar una tarea
                                     OS_TASK_PTR    osTaskPtr,
                                     int16u         osStackSize);
int16u      OSGetNewID              (void);                                     // Obtener un valor para el ID del proceso
void        OSInsertTaskReadyList   (OS_TCB *tcb, bool prioridad);              // Insertar el proceso en la lista enlazada
OS_TCB *    OSGetTaskReadyList      (void);                                     // Obtener un proceso de la lista enlazada
void        OSScheduler             (void);                                     // Despachador
void        OSCheckStacks           (void);                                     // Verificar que los stacks de procesos no hagan overflow
void        OSIdleTask              (void *arg);                                // Task ocioso
void        OSStatAndInitTask       (void *arg);                                // Task para el calculo de las estadisticas de ejecución

// *****************************************************************************
// Funciones privadas declaradas en otros modulos
extern void OSInitMutexList (void);                                             //
extern void OSTimerTask     (void *arg);                                        //

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
void __OSInit(void)
{
    OSInitHW();                                                                 // Inicializar el hardware.
    __OSMemInit();                                                              // Inicializar pool de memoria
    __IOInitDevices();                                                          // Instalar drivers
    __OSInitTimer();
    OSInitTask(osIdleTaskName, OSIdleTask, TASK_STACK_SIZE_MIN);                // Inicializar el proceso inicial del sistema
    OSInitTask(osStatTaskName, OSStatAndInitTask, TASK_STACK_SIZE_MIN);         // Inicialiar el proceso de estadistica y otros procesos
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void __OSStart(void)
{
    OSEnableTick();                                                             // Habilitar el modulo del tick
    OSEnableInterrupts();                                                       // habilitar las Interrupciones
    //OSWait();                                                                   // Esperar hasta la ejecucion del IdleTask
    for(;;){                                                                    // Por si acaso...
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
OS_ERROR OSCreateTask(const int8u *name)
{
    int8u taskIndex = 0;
    const int8u *taskName;
    if(name == NULL)                                                            // Determinar si el nombre es un null pointer
        return OS_ERROR_TASK_NAME_NULL;
    while(osTaskInstallList[taskIndex].OSTaskPtr != NULL){                      // Determinar si existe el proceso. Iterar hasta que se acabe la lista
        taskName = osTaskInstallList[taskIndex].OSTaskName;                     // buscar por nombre.
        if(OSStrCmp(name, taskName))
            return OSInitTask(taskName, osTaskInstallList[taskIndex].OSTaskPtr, osTaskInstallList[taskIndex].OSStackSize);
        taskIndex++;                                                            // buscar el siguiente
    }
    return OS_ERROR_TASK_NOT_EXIST;                                             // No se encontro el proceso.
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_ERROR OSDeleteTask(const int8u *name)
{
    // Por ahora esta funcion no hace nada. Se supone que en ningun momento se eliminan tareas
    (void)name;
    return OS_ERROR_NONE;
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
void OSInitHW(void)
{
    OSInitCPU();                                                                // Inicializar el CPU
    OSInitTick();                                                               // Inicializar el Timer del OS
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSInitAutostartTask(void)
{
    int8u taskIndex = 0;

    while(osTaskInstallList[taskIndex].OSTaskPtr != NULL){
        if(osTaskInstallList[taskIndex].OSTaskName != NULL && osTaskInstallList[taskIndex].autostart)
            (void)OSInitTask(osTaskInstallList[taskIndex].OSTaskName, osTaskInstallList[taskIndex].OSTaskPtr, osTaskInstallList[taskIndex].OSStackSize);
        taskIndex++;
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
OS_ERROR OSInitTask(const int8u *osTaskName, OS_TASK_PTR osTaskPtr, int16u osStackSize)
{
    OS_CPU_SR cpuSR;
    OS_TCB *tcb;                                                                // tcb temporal
    OS_STACK *stack;                                                            // stack de la tarea

    // allocate one tcb
    tcb = (OS_TCB *)OSAllocMem(sizeof(OS_TCB));                                 // Allocate memory
    if(tcb == NULL)
        return OS_ERROR_TASK_MAX_NUM;                                           // Si no hay espacio, se alcanzo el limite.

    // proceder a inicializar el TCB del proceso
    tcb->OSTaskName = osTaskName;                                               // copiar el nombre
    tcb->OSTaskPtr = osTaskPtr;                                                 // copiar la direccion del proceso

    // Solicitar un bloque de memoria para el proceso
    stack = (OS_STACK *)OSAllocMem(osStackSize * sizeof(OS_STACK));
    if(stack == NULL){
        OSFreeMem(tcb);                                                         // Hacer free (OJO: Posible memory leak, segun la implementación del Free)
        return OS_ERROR_TASK_NO_MEMORY;                                         // Indicar el error.
    }

    // configurar los los parametros
    tcb->OSTaskStackBottom  = stack + osStackSize - 1;                          // Ultima posicion de memoria del stack
    tcb->OSTaskStackTop     = stack;                                            // primera posicion de la memoria
    tcb->OSStackSize        = osStackSize * sizeof(OS_STACK);                   // Tamaño del stack en bytes
    tcb->OSStackFreeSize    = osStackSize * sizeof(OS_STACK);                   // En teoria, el stack es libre completamente
    tcb->OSTaskSP = OSInitTaskStack(tcb->OSTaskPtr, NULL, tcb->OSTaskStackBottom);
    tcb->OSTaskID = OSGetNewID();                                               // Asignar ID al proceso
    tcb->IOFile = NULL;                                                         // no posee ningun archivo

    OSInsertTaskReadyList(tcb, false);                                          // insertar en la lista de procesos listos para ejecutar
    cpuSR = OSEnterCriticalSection();                                           // La variable es global, por lo tanto, se debe proteger el acceso.
        osNumberTask++;
    OSExitCriticalSection(cpuSR);

    return OS_ERROR_NONE;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
int16u OSGetNewID(void)
{
    return ++osCurrentPID;                                                      // devolver el valor.
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSInsertTaskReadyList(OS_TCB *tcb, bool prioridad)
{
    if(prioridad){
        tcb->OSTCBNext = osTaskReadyListHead;                                   // insertar primero en la lista
        osTaskReadyListHead = tcb;
        if(tcb->OSTCBNext == NULL)                                              // si es el unico, actualizar el apuntador al ultimo de la lista
            osTaskReadyListTail = tcb;
    }
    else{
        if(osTaskReadyListHead == NULL)                                         // si es el primer proceso en insertar
            osTaskReadyListHead = tcb;                                          // crear cabeza
        else
            osTaskReadyListTail->OSTCBNext = tcb;                               // insertar en la cola
        osTaskReadyListTail = tcb;  // y la cola.
    }

    osTaskReadyListTail->OSTCBNext = NULL;
    tcb->OSTaskStatus = OS_TASK_STATUS_READY;                                   // Marcar como Listo para ejecucion
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
OS_TCB  *OSGetTaskReadyList(void)
{
    OS_TCB *temp = osTaskReadyListHead;
    osTaskReadyListHead = osTaskReadyListHead->OSTCBNext;                       // apuntar al proximo elemento
    if(osTaskReadyListHead == NULL)
        osTaskReadyListTail = NULL;
    return temp;
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSScheduler(void)
{
    if(osTaskCurrent != NULL){                                                  // determinar si es la primera ejecucion
        osTaskCurrent->OSTaskSP = spTemp;                                       // guardar el SP
        // verificar si el proceso no esta marcado como bloqueado!
        if(osTaskCurrent->OSTaskStatus == OS_TASK_STATUS_RUN){
            osTaskCurrent->OSTaskStatus = OS_TASK_STATUS_READY;                 // marcar como listo
            OSInsertTaskReadyList(osTaskCurrent, false);                        // insertar en la lista
        }
    }
    osTaskCurrent = OSGetTaskReadyList();                                       // obtener el siguiente
    osTaskCurrent->OSTaskStatus = OS_TASK_STATUS_RUN;                           // cambiar el estado del proceso actual
    spTemp = osTaskCurrent->OSTaskSP;                                           // obtener el SP del nuevo proceso
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSCheckStacks(void)
{
    OS_STACK *ptrStack;
    int16u  freeSize = 0;

    if(osTaskCurrent == NULL)
        return;
    ptrStack = osTaskCurrent->OSTaskStackTop;                                   // Apuntador al inicio del tope.
    while(*(ptrStack++) == (OS_STACK)0)                                                   // Verificar cuantas entradas son iguales a cero
        freeSize++;
    osTaskCurrent->OSStackFreeSize = freeSize * sizeof(OS_STACK);               // Guardar el valor dentro del TCB
}

/*
 * ###################################################################
 *  Función     :
 *  Descripción :
 *  Parmámetros :
 *  Retorna     :
 * ###################################################################
 */
void OSIdleTask(void *arg)
{
    OS_CPU_SR cpuSR;
    (void)arg;
    for(;;){
        cpuSR = OSEnterCriticalSection();                                       // La variable es global, por lo tanto, se debe proteger el acceso.
            osIdleCounter++;
        OSExitCriticalSection(cpuSR);
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
void OSStatAndInitTask(void *arg)
{
    OS_CPU_SR cpuSR;
    (void)arg;

    OSDelayMS(10);                                                              // Esperar 10 ms
    cpuSR = OSEnterCriticalSection();
        osIdleCounter = 0;                                                      // Reiniciar el contador del idle task.
    OSExitCriticalSection(cpuSR);
    OSDelayMS(100);                                                             // Esperar 100 ms, para el calculo del valor maximo del contador
    cpuSR = OSEnterCriticalSection();
        osIdleCounterMax = osIdleCounter;                                       // Guardar la cuenta maxima
    OSExitCriticalSection(cpuSR);

    OSInitTask(osTimerTaskName, OSTimerTask, TASK_STACK_SIZE_MIN);              // Inicialiar el proceso del timer
    OSInitAutostartTask();                                                      // Iniciar los procesos de usuario configurados en inicio automatico.

    for(;;){
        cpuSR = OSEnterCriticalSection();                                       // La variable es global, por lo tanto, se debe proteger el acceso.
            osIdleCounterNow = osIdleCounter;                                   // Obtener la cuenta actual del uso de CPU
            osIdleCounter = 0;                                                  // Reiniciar la cuenta a cero
        OSExitCriticalSection(cpuSR);
        osCPUUsage = 100u - (int8u)((osIdleCounterNow*100)/osIdleCounterMax);   // Calcular el porcentaje
        OSDelayMS(100);                                                         // Dormir por otros 100 ms
    }
}

/*
 ** ############################################################################
 ** Fin del archivo
 ** ############################################################################
 */
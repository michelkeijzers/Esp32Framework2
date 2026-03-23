#ifdef ESP_PLATFORM

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#else

#include <cstdint>

#define portDOUBLE double
#define portLONG int32_t
#define portSHORT int16_t
#define portSTACK_TYPE uint8_t
#define portBASE_TYPE int

#define configSTACK_DEPTH_TYPE    StackType_t
 
typedef portSTACK_TYPE StackType_t;
typedef portBASE_TYPE BaseType_t;
typedef unsigned portBASE_TYPE UBaseType_t;

typedef void (* TaskFunction_t)( void * arg );
 
struct tskTaskControlBlock; /* The old naming convention is used to prevent breaking kernel aware debuggers. */
typedef struct tskTaskControlBlock * TaskHandle_t;

#define pdFALSE ( ( BaseType_t ) 0 )
#define pdTRUE  ( ( BaseType_t ) 1 )

#define pdPASS  ( pdTRUE )
#define pdFAIL  ( pdFALSE )

inline BaseType_t xTaskCreate(TaskFunction_t pvTaskCode, const char* pcName, configSTACK_DEPTH_TYPE uxStackDepth, void* pvParametesr, UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask)
{
    return pdPASS;
}

#endif

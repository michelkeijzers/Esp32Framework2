#pragma once

#ifdef ESP_PLATFORM

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

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

// Queue types (no function stubs — use RtosQueueStub.cpp in test builds)
typedef uint32_t TickType_t;
#define CONFIG_FREERTOS_HZ 100
#define configTICK_RATE_HZ CONFIG_FREERTOS_HZ
#define pdMS_TO_TICKS( xTimeInMs ) \
    ( ( TickType_t ) ( ( ( uint64_t ) ( xTimeInMs ) * ( uint64_t ) configTICK_RATE_HZ ) / ( uint64_t ) 1000U ) )

struct QueueDefinition; /* Using old naming convention so as not to break kernel aware debuggers. */
typedef struct QueueDefinition   * QueueHandle_t;
typedef struct QueueDefinition   * QueueSetHandle_t;
typedef struct QueueDefinition   * QueueSetMemberHandle_t;

#endif

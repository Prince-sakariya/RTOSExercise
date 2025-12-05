#pragma once

// Override Trace hook macros
#ifndef __ASSEMBLER__
    #ifdef configUSE_TRACE_FACILITY
        #define configUSE_TRACE_FACILITY        1
        #define configUSE_POSIX_ERRNO           1
    #endif
    
    #define traceQUEUE_SET_SEND(pxQueueSet) \
    do { \
        const char* taskName = pcTaskGetName(NULL); \
    } while(0)
    
    #define TRACE_IF_ENABLED(code) \
    do { \
        extern volatile TickType_t xLoggingEnabled; \
        if (xLoggingEnabled) { code; } } while(0)



    #define traceQUEUE_SEND( pxQueue )                                  \
        TRACE_IF_ENABLED(                         \
            TickType_t waitTicks = xTicksToWait;                    \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueSendTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );   \
            vQueueSendTrace( pxQueue, tickCount, waitTicks, taskName );                             \
        )

    #define traceQUEUE_SEND_FAILED( pxQueue )                           \
        TRACE_IF_ENABLED(                         \
            TickType_t waitTicks = xTicksToWait;                    \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueSendFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );  \
            vQueueSendFailedTrace( pxQueue, tickCount, waitTicks, taskName );                       \
        )

    #define traceQUEUE_SEND_FROM_ISR( pxQueue )                         \
        TRACE_IF_ENABLED(                             \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueSendFromISRTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );       \
            vQueueSendFromISRTrace( pxQueue, tickCount,  0, taskName );                                 \
        )

    #define traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue )                  \
        TRACE_IF_ENABLED(                         \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueSendFromISRFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );  \
            vQueueSendFromISRFailedTrace( pxQueue, tickCount, 0, taskName );                       \
        )

    #define traceQUEUE_RECEIVE( pxQueue )                               \
        TRACE_IF_ENABLED(                         \
            TickType_t waitTicks = xTicksToWait;                    \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueReceiveTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );  \
            vQueueReceiveTrace( pxQueue, tickCount, waitTicks, taskName );                          \
        )

    #define traceQUEUE_RECEIVE_FAILED( pxQueue )                        \
        TRACE_IF_ENABLED(                         \
            TickType_t waitTicks = xTicksToWait;                    \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueReceiveFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );  \
            vQueueReceiveFailedTrace( pxQueue, tickCount, waitTicks, taskName );                    \
        )
    
    #define traceQUEUE_RECEIVE_FROM_ISR( pxQueue )                      \
        TRACE_IF_ENABLED(                             \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueReceiveFromISRTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );       \
            vQueueReceiveFromISRTrace( pxQueue, tickCount, 0, taskName );                                 \
        )

    #define traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue )               \
        TRACE_IF_ENABLED(                         \
            TickType_t tickCount = xTaskGetTickCount();             \
            const char* taskName = pcTaskGetName( NULL );           \
            extern void vQueueReceiveFromISRFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName );  \
            vQueueReceiveFromISRFailedTrace( pxQueue, tickCount, 0, taskName );                       \
        )

#endif /* def __ASSEMBLER__ */
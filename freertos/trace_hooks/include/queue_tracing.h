#pragma once

// Override Trace hook macros
#ifndef __ASSEMBLER__
    #ifdef configUSE_TRACE_FACILITY
        #define configUSE_TRACE_FACILITY        1
    #endif
    
    #define traceQUEUE_SET_SEND(pxQueueSet) \
    do { \
        const char* taskName = pcTaskGetName(NULL); \
    } while(0)

    #define traceQUEUE_SEND( pxQueue )                                  \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                         \
                TickType_t waitTicks = xTicksToWait;                    \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueSendTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks);   \
                vQueueSendTrace( pxQueue, tickCount, waitTicks );                             \
            }                                                           \
        } while( 0 )

    #define traceQUEUE_SEND_FAILED( pxQueue )                           \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                         \
                TickType_t waitTicks = xTicksToWait;                    \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueSendFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );  \
                vQueueSendFailedTrace( pxQueue, tickCount, waitTicks );                       \
            }                                                           \
        } while ( 0 )
    
    #define traceQUEUE_SEND_FROM_ISR( pxQueue )                         \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                             \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueSendFromISRTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );       \
                vQueueSendFromISRTrace( pxQueue, tickCount,  0 );                                 \
            }                                                               \
        } while( 0 )

    #define traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue )                  \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                         \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueSendFromISRFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );  \
                vQueueSendFromISRFailedTrace( pxQueue, tickCount, 0 );                       \
            }                                                           \
        } while ( 0 )
    
    #define traceQUEUE_RECEIVE( pxQueue )                               \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                         \
                TickType_t waitTicks = xTicksToWait;                    \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueReceiveTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );  \
                vQueueReceiveTrace( pxQueue, tickCount, waitTicks );                          \
            }                                                           \
        } while( 0 )

    #define traceQUEUE_RECEIVE_FAILED( pxQueue )                        \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                         \
                TickType_t waitTicks = xTicksToWait;                    \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueReceiveFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );  \
                vQueueReceiveFailedTrace( pxQueue, tickCount, waitTicks );                    \
            }                                                           \
        } while( 0 )
    
    #define traceQUEUE_RECEIVE_FROM_ISR( pxQueue )                      \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                             \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueReceiveFromISRTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );       \
                vQueueReceiveFromISRTrace( pxQueue, tickCount, 0 );                                 \
            }                                                               \
        } while( 0 )

    #define traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue )               \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                          \
            if ( xLoggingEnabled ) {                         \
                TickType_t tickCount = xTaskGetTickCount();             \
                extern void vQueueReceiveFromISRFailedTrace( QueueHandle_t pxQueue, TickType_t tickCount, TickType_t waitTicks );  \
                vQueueReceiveFromISRFailedTrace( pxQueue, tickCount, 0 );                       \
            }                                                           \
        } while ( 0 )    

#endif /* def __ASSEMBLER__ */
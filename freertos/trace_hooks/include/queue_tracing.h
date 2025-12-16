#pragma once

// Override Trace hook macros
#ifndef __ASSEMBLER__
    #ifdef configUSE_TRACE_FACILITY
        #define configUSE_TRACE_FACILITY        1
        #define configUSE_POSIX_ERRNO           1
    #endif
    
    #define traceQUEUE_SET_SEND(pxQueueSet)         \
    do {                                            \
        const char* taskName = pcTaskGetName(NULL); \
    } while(0)
    
    #define TRACE_IF_ENABLED(code)                  \
    do {                                            \
        extern volatile TickType_t xLoggingEnabled; \
        if (xLoggingEnabled) { code; } } while(0)

    #define traceQUEUE_SEND( pxQueue )                                                                      \
        TRACE_IF_ENABLED(                                                                                   \
            TickType_t waitTicks = xTicksToWait;                                                            \
            TickType_t tickCount = xTaskGetTickCount();                                                     \
            const char* taskName = pcTaskGetName( NULL );                                                   \
            static const char *QUEUE_SEND = "traceQUEUE_SEND";                                              \
            extern void LogEvent( const char *event,                                                        \
                TickType_t tickCount,                                                                       \
                QueueHandle_t queue,                                                                        \
                TickType_t waitTicks,                                                                       \
                const char* taskName,                                                                       \
                int coreID                                                                                  \
            );                                                                                              \
            LogEvent( QUEUE_SEND, tickCount, pxQueue, waitTicks, taskName, portGET_CORE_ID() );             \
        )

    #define traceQUEUE_SEND_FAILED( pxQueue )                                                               \
        TRACE_IF_ENABLED(                                                                                   \
            TickType_t waitTicks = xTicksToWait;                                                            \
            TickType_t tickCount = xTaskGetTickCount();                                                     \
            const char* taskName = pcTaskGetName( NULL );                                                   \
            static const char *QUEUE_SEND_FAILED = "traceQUEUE_SEND_FAILED";                                \
            extern void LogEvent( const char *event,                                                        \
                TickType_t tickCount,                                                                       \
                QueueHandle_t queue,                                                                        \
                TickType_t waitTicks,                                                                       \
                const char* taskName,                                                                       \
                int coreID                                                                                  \
            );                                                                                              \
            LogEvent( QUEUE_SEND_FAILED, tickCount, pxQueue, waitTicks, taskName, portGET_CORE_ID()  );     \
        )

    #define traceQUEUE_SEND_FROM_ISR( pxQueue )                                                             \
        TRACE_IF_ENABLED(                                                                                   \
            TickType_t tickCount = xTaskGetTickCountFromISR();                                              \
            static const char *QUEUE_SEND_FROM_ISR = "traceQUEUE_SEND_FROM_ISR";                            \
            extern void LogEvent( const char *event,                                                        \
                TickType_t tickCount,                                                                       \
                QueueHandle_t queue,                                                                        \
                TickType_t waitTicks,                                                                       \
                const char* taskName,                                                                       \
                int coreID                                                                                  \
            );                                                                                              \
            LogEvent( QUEUE_SEND_FROM_ISR, tickCount, pxQueue, 0, "ISR", portGET_CORE_ID() );               \
        )

    #define traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue )                                                      \
        TRACE_IF_ENABLED(                                                                                   \
            TickType_t tickCount = xTaskGetTickCountFromISR();                                              \
            const char* taskName = pcTaskGetName( NULL );                                                   \
            static const char *QUEUE_SEND_FROM_ISR_FAILED = "traceQUEUE_SEND_FROM_ISR_FAILED";              \
            extern void LogEvent( const char *event,                                                        \
                TickType_t tickCount,                                                                       \
                QueueHandle_t queue,                                                                        \
                TickType_t waitTicks,                                                                       \
                const char* taskName,                                                                       \
                int coreID                                                                                  \
            );                                                                                              \
            LogEvent( QUEUE_SEND_FROM_ISR_FAILED, tickCount, pxQueue, 0, "ISR", portGET_CORE_ID() );        \
        )

    #define traceQUEUE_RECEIVE( pxQueue )                                                                   \
        TRACE_IF_ENABLED(                                                                                   \
            TickType_t waitTicks = xTicksToWait;                                                            \
            TickType_t tickCount = xTaskGetTickCount();                                                     \
            const char* taskName = pcTaskGetName( NULL );                                                   \
            static const char *QUEUE_RECEIVE = "traceQUEUE_RECEIVE";                                        \
            extern void LogEvent( const char *event,                                                        \
                TickType_t tickCount,                                                                       \
                QueueHandle_t queue,                                                                        \
                TickType_t waitTicks,                                                                       \
                const char* taskName,                                                                       \
                int coreID                                                                                  \
            );                                                                                              \
            LogEvent( QUEUE_RECEIVE, tickCount, pxQueue, waitTicks, taskName, portGET_CORE_ID() );          \
        )

    #define traceQUEUE_RECEIVE_FAILED( pxQueue )                                                            \
        TRACE_IF_ENABLED(                                                                                   \
            TickType_t waitTicks = xTicksToWait;                                                            \
            TickType_t tickCount = xTaskGetTickCount();                                                     \
            const char* taskName = pcTaskGetName( NULL );                                                   \
            static const char *QUEUE_RECEIVE_FAILED = "traceQUEUE_RECEIVE_FAILED";                          \
            extern void LogEvent( const char *event,                                                        \
                TickType_t tickCount,                                                                       \
                QueueHandle_t queue,                                                                        \
                TickType_t waitTicks,                                                                       \
                const char* taskName,                                                                       \
                int coreID                                                                                  \
            );                                                                                              \
            LogEvent( QUEUE_RECEIVE_FAILED, tickCount, pxQueue, waitTicks, taskName, portGET_CORE_ID() );   \
        )
    
    #define traceQUEUE_RECEIVE_FROM_ISR( pxQueue )                                                      \
        TRACE_IF_ENABLED(                                                                               \
            TickType_t tickCount = xTaskGetTickCountFromISR();                                          \
            const char* taskName = pcTaskGetName( NULL );                                               \
            static const char *QUEUE_RECEIVE_FROM_ISR = "traceQUEUE_RECEIVE_FROM_ISR";                  \
            extern void LogEvent( const char *event,                                                    \
                TickType_t tickCount,                                                                   \
                QueueHandle_t queue,                                                                    \
                TickType_t waitTicks,                                                                   \
                const char* taskName,                                                                   \
                int coreID                                                                              \
            );                                                                                          \
            LogEvent( QUEUE_RECEIVE_FROM_ISR, tickCount, pxQueue, 0, "ISR", portGET_CORE_ID() );        \
        )

    #define traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue )                                               \
        TRACE_IF_ENABLED(                                                                               \
            TickType_t tickCount = xTaskGetTickCountFromISR();                                          \
            const char* taskName = pcTaskGetName( NULL );                                               \
            static const char *QUEUE_RECEIVE_FROM_ISR_FAILED = "traceQUEUE_RECEIVE_FROM_ISR_FAILED";    \
            extern void LogEvent( const char *event,                                                    \
                TickType_t tickCount,                                                                   \
                QueueHandle_t queue,                                                                    \
                TickType_t waitTicks,                                                                   \
                const char* taskName,                                                                   \
                int coreID                                                                              \
            );                                                                                          \
            LogEvent( QUEUE_RECEIVE_FROM_ISR_FAILED, tickCount, pxQueue, 0, "ISR", portGET_CORE_ID() ); \
        )

#endif /* def __ASSEMBLER__ */
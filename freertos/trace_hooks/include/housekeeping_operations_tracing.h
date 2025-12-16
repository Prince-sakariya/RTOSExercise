#pragma once

// Override Trace hook macros
#ifndef __ASSEMBLER__

    #define TRACE_IF_ENABLED(code) \
    do { \
        extern volatile TickType_t xLoggingEnabled; \
        if (xLoggingEnabled) { code; } } while(0)


    // ----------------------Task Increment Macro ----------------------------------------------
    #define traceTASK_INCREMENT_TICK( xTickCount )                      \
        TRACE_IF_ENABLED( \
            const char* taskName = pcTaskGetName( NULL );            \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID ); \
            LogEvent( "traceTASK_INCREMENT_TICK", xTickCount, NULL, 0, taskName, portGET_CORE_ID() );                             \
        )
    
    // ----------------------Task Creation/Deletion Macro -------------------------------------
    
    #define traceTASK_CREATE( xTask )                                   \
        TRACE_IF_ENABLED( \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID ); \
            LogEvent( "traceTASK_CREATE", xTaskGetTickCount(), NULL, 0, pcTaskGetName( xTask ), portGET_CORE_ID() );                    \
        )
    
    #define traceTASK_CREATE_FAILED( pxNewTCB ) \
        TRACE_IF_ENABLED( \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID ); \
            LogEvent( "traceTASK_CREATE_FAILED", xTaskGetTickCount(), NULL, 0, "Unknown", portGET_CORE_ID() ); \
        )

    #define traceTASK_DELETE( xTask ) \
        TRACE_IF_ENABLED( \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID ); \
            LogEvent( "traceTASK_DELETE", xTaskGetTickCount(), NULL, 0, pcTaskGetName( xTask ), portGET_CORE_ID() ); \
        )

    // ----------------------Task Delay Macro -------------------------------------
    #define traceTASK_DELAY( )                                   \
        TRACE_IF_ENABLED( \
            const char* taskName = pcTaskGetName( NULL );        \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID ); \
            LogEvent( "traceTASK_DELAY", xTaskGetTickCount(), NULL, 0, taskName, portGET_CORE_ID() );                    \
        )
    
    #define traceTASK_DELAY_UNTIL( xTimeToWake )                                   \
        TRACE_IF_ENABLED( \
            const char* taskName = pcTaskGetName( NULL );        \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID ); \
            LogEvent( "traceTASK_DELAY_UNTIL", xTaskGetTickCount(), NULL, xTimeToWake, taskName, portGET_CORE_ID() );                    \
        )

#endif /* def __ASSEMBLER__ */
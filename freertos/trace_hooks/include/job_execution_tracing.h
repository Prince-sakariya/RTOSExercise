#pragma once

// Override Trace hook macros
#ifndef __ASSEMBLER__

    #define TRACE_IF_ENABLED(code) \
    do { \
        extern volatile TickType_t xLoggingEnabled; \
        if (xLoggingEnabled) { code; } } while(0)


    // ----------------------Task Switched In/Out Macro ----------------------------------------------
    #define traceTASK_SWITCHED_IN( )                      \
    TRACE_IF_ENABLED( \
            TaskHandle_t _t = pxCurrentTCBs[ portGET_CORE_ID() ]; \
            const char* taskName = (_t != NULL) ? pcTaskGetName(_t) : "NULL"; \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName ); \
            LogEvent( "TSK_SWITCH_IN", xTaskGetTickCount(), NULL, 0, taskName );  \
        )
    
    // -------- TASK SWITCH OUT ----------
    #define traceTASK_SWITCHED_OUT( ) \
        TRACE_IF_ENABLED( \
            TaskHandle_t _t = pxCurrentTCBs[ portGET_CORE_ID() ]; \
            const char* taskName = (_t != NULL) ? pcTaskGetName(_t) : "NULL"; \
            extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName ); \
            LogEvent( "TSK_SWITCH_OUT", xTaskGetTickCount(), NULL, 0, taskName );  \
        )

#endif /* def __ASSEMBLER__ */
#pragma once

// Override Trace hook macros
#ifndef __ASSEMBLER__

    #define traceTASK_INCREMENT_TICK( xTickCount )                      \
        do {                                                            \
            extern TickType_t xLoggingEnabled;                                              \
            if ( xLoggingEnabled ) {               \
                extern void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks );   \
                LogEvent( "TSK_INCR_TICK", xTickCount, NULL, 0 );                             \
            }                                                           \
        } while( 0 )
    
#endif /* def __ASSEMBLER__ */
#include "logging.h"
#include "esp_log.h"
#include "esp_timer.h"

#define LOG_BUFFER_SIZE 2048
#define LOG_TAG "LOG"

static LogEntry_t logBuffer[ LOG_BUFFER_SIZE ];
static volatile uint32_t logHead = 0;
static volatile uint32_t logTail = 0;

// ------------------ Logging Flag -----------------
volatile TickType_t xLoggingEnabled = 0;

void Log_Init( void )
{
    logHead = 0;
    logTail = 0;
}

// Core logging function
void LogEvent( const char *event, TickType_t tickCount, QueueHandle_t queue, TickType_t waitTicks, const char* taskName, int coreID )
{
    uint32_t i = logHead % LOG_BUFFER_SIZE;
    logBuffer[ i ].event = event;
    logBuffer[ i ].tickCount = tickCount;
    logBuffer[ i ].microSeconds = ( uint32_t )esp_timer_get_time();
    logBuffer[ i ].queueHandle = queue;
    logBuffer[ i ].waitTicks = waitTicks;
    logBuffer[ i ].taskName = taskName;
    logBuffer[ i ].coreID = coreID;

    logHead++;
    if (logHead - logTail > LOG_BUFFER_SIZE) {
        logTail = logHead - LOG_BUFFER_SIZE; // drop oldest
    }
}

// Flush buffer to console
void LogFlush(void)
{
    while (logTail < logHead) {
        uint32_t i = logTail % LOG_BUFFER_SIZE;
        LogEntry_t* e = &logBuffer[ i ];
        const char* safeTaskName = e->taskName ? e->taskName : "NULL"; 

        // Use ESP_LOGE for failures
        if ( strstr( e->event, "FAILED" ) != NULL ) {
            ESP_LOGE(LOG_TAG, "TickCount(ticks)=%lu, Timestamp(us)=%u, EventType=%s, Task=%s, Queue=%p, TickWait(ticks)=%lu, CoreID=%d",
                     e->tickCount, e->microSeconds, e->event, safeTaskName, e->queueHandle, e->waitTicks, e->coreID );
        } 
        else if ( strstr( e->event, "TSK_INCR_TICK") != NULL )
        {
            ESP_LOGI(LOG_TAG, "TickCount(ticks)=%lu, Timestamp(us)=%u , EventType=%s, Task=%s, Queue=NULL, TickWait(ticks)=NULL, CoreID=%d",
                    e->tickCount, e->microSeconds, e->event, safeTaskName, e->coreID );
        }
        else if ( strstr( e->event, "TASK_DELAY") != NULL )
        {
            ESP_LOGI(LOG_TAG, "TickCount(ticks)=%lu, Timestamp(us)=%u , EventType=%s, Task=%s, Queue=NULL, TickWait(ticks)=%lu, CoreID=%d",
                    e->tickCount, e->microSeconds, e->event, safeTaskName, e->waitTicks, e->coreID );
        }
        else if ( strstr( e->event, "TASK_SWITCH" ) != NULL ) 
        {
            ESP_LOGI(LOG_TAG, "TickCount(ticks)=%lu, Timestamp(us)=%u , EventType=%s, Task=%s, Queue=NULL, TickWait(ticks)=NULL, CoreID=%d",
                    e->tickCount, e->microSeconds, e->event, safeTaskName, e->coreID );
        }
        else {
            ESP_LOGI(LOG_TAG, "TickCount(ticks)=%lu, Timestamp(us)=%u , EventType=%s, Task=%s, Queue=%p, TickWait(ticks)=%lu, CoreID=%d",
                     e->tickCount, e->microSeconds, e->event, safeTaskName, e->queueHandle, e->waitTicks, e->coreID );
        }

        logTail++;
    }
}
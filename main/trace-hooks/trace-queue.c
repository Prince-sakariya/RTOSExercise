// #include "trace-queue.h"
#include "logging/logging.h" 
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
// #include "esp_log.h" 

static const char *QUEUE_SEND = "traceQUEUE_SEND";
static const char *QUEUE_SEND_FAILED = "traceQUEUE_SEND_FAILED";
static const char *QUEUE_SEND_FROM_ISR = "traceQUEUE_SEND_FROM_ISR";
static const char *QUEUE_SEND_FROM_ISR_FAILED = "traceQUEUE_SEND_FROM_ISR_FAILED";
static const char *QUEUE_RECEIVE = "traceQUEUE_RECEIVE";
static const char *QUEUE_RECEIVE_FAILED = "traceQUEUE_RECEIVE_FAILED";
static const char *QUEUE_RECEIVE_FROM_ISR = "traceQUEUE_RECEIVE_FROM_ISR";
static const char *QUEUE_RECEIVE_FROM_ISR_FAILED = "traceQUEUE_RECEIVE_FROM_ISR_FAILED";

void vQueueSendTrace( QueueHandle_t xQueue, TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{   
    LogEvent( QUEUE_SEND, tickCount, xQueue, waitTicks, taskName );  // store in buffer
}

void vQueueSendFailedTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{
    LogEvent( QUEUE_SEND_FAILED, tickCount, xQueue, waitTicks, taskName ); // store in buffer
}

void vQueueSendFromISRTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{   
    LogEvent( QUEUE_SEND_FROM_ISR, tickCount, xQueue, waitTicks, taskName );  // store in buffer
}

void vQueueSendFromISRFailedTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{
    LogEvent( QUEUE_SEND_FROM_ISR_FAILED, tickCount, xQueue, waitTicks, taskName ); // store in buffer
}

void vQueueReceiveTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{   
    LogEvent( QUEUE_RECEIVE, tickCount, xQueue, waitTicks, taskName ); // store in buffer
}

void vQueueReceiveFailedTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{
    LogEvent( QUEUE_RECEIVE_FAILED, tickCount, xQueue, waitTicks, taskName ); // store in buffer
}

void vQueueReceiveFromISRTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{   
    LogEvent( QUEUE_RECEIVE_FROM_ISR, tickCount, xQueue, waitTicks, taskName );  // store in buffer
}

void vQueueReceiveFromISRFailedTrace( QueueHandle_t xQueue , TickType_t tickCount, TickType_t waitTicks, const char* taskName )
{
    LogEvent( QUEUE_RECEIVE_FROM_ISR_FAILED, tickCount, xQueue, waitTicks, taskName ); // store in buffer
}
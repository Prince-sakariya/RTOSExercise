// #include "trace-queue.h"
#include "logging/logging.h" 
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
// #include "esp_log.h" 

static const char *QUEUE_SEND = "SEND";
static const char *QUEUE_SEND_FAILED = "SEND FAILED";
static const char *QUEUE_SEND_FROM_ISR = "SEND FROM ISR";
static const char *QUEUE_SEND_FROM_ISR_FAILED = "SEND FROM ISR FAILED";
static const char *QUEUE_RECEIVE = "RECV";
static const char *QUEUE_RECEIVE_FAILED = "RECV FAILED";
static const char *QUEUE_RECEIVE_FROM_ISR = "RECV FROM ISR";
static const char *QUEUE_RECEIVE_FROM_ISR_FAILED = "RECV FROM ISR FAILED";

void vQueueSendTrace( QueueHandle_t xQueue, TickType_t waitTicks )
{   
    LogEvent( QUEUE_SEND, xQueue, waitTicks );  // store in buffer
}

void vQueueSendFailedTrace( QueueHandle_t xQueue )
{
    LogEvent( QUEUE_SEND_FAILED, xQueue, 0 ); // store in buffer
}

void vQueueSendFromISRTrace( QueueHandle_t xQueue )
{   
    LogEvent( QUEUE_SEND_FROM_ISR, xQueue, 0 );  // store in buffer
}

void vQueueSendFromISRFailedTrace( QueueHandle_t xQueue )
{
    LogEvent( QUEUE_SEND_FROM_ISR_FAILED, xQueue, 0 ); // store in buffer
}

void vQueueReceiveTrace( QueueHandle_t xQueue )
{   
    LogEvent( QUEUE_RECEIVE, xQueue, 0 ); // store in buffer
}

void vQueueReceiveFailedTrace( QueueHandle_t xQueue )
{
    LogEvent( QUEUE_RECEIVE_FAILED, xQueue, 0 ); // store in buffer
}

void vQueueReceiveFromISRTrace( QueueHandle_t xQueue )
{   
    LogEvent( QUEUE_RECEIVE_FROM_ISR, xQueue, 0 );  // store in buffer
}

void vQueueReceiveFromISRFailedTrace( QueueHandle_t xQueue )
{
    LogEvent( QUEUE_RECEIVE_FROM_ISR_FAILED, xQueue, 0 ); // store in buffer
}
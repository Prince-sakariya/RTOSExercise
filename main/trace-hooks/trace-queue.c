// #include "trace-queue.h"
#include "logging/logging.h" 
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
// #include "esp_log.h" 

static const char *QUEUE_SEND = "SEND";
static const char *QUEUE_RECEIVE = "RECV";

void vQueueSendTrace( QueueHandle_t pxQueue )
{   
    // ESP_EARLY_LOGI( QUEUE_SEND, "\tTimestamp: %lu\tSent: ", xTaskGetTickCount() );
    LogEvent(QUEUE_SEND, ( char* )pxQueue, 0);  // store in buffer
}

void vQueueReceiveTrace( QueueHandle_t pxQueue )
{   
    // ESP_EARLY_LOGI( QUEUE_RECEIVE, "\tTimestamp: %lu\tReceived: ", xTaskGetTickCount() );
    LogEvent(QUEUE_RECEIVE, ( char* )pxQueue, 0); // store in buffer

}
// #include "trace-queue.h" 
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h" 

static const char *QUEUE_SEND = "QSend";
static const char *QUEUE_RECEIVE = "QReceive";

void vQueueSendTrace( void *pxQueue )
{   
    const char* task_name = "UNKNOWN";

    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) {
        TaskHandle_t current_task = xTaskGetCurrentTaskHandle();
    
        // Check for NULL handle (e.g., if called by an ISR/Timer)
        if (current_task != NULL) {
            task_name = pcTaskGetName(current_task);
        }  
    }
    ESP_EARLY_LOGI( QUEUE_SEND, "QUEUE_SEND. Queue Handle: %p\tTimestamp: %lu\tSent by: %s", pxQueue, xTaskGetTickCount(), task_name );
}

void vQueueReceiveTrace( void *pxQueue )
{   
    const char* task_name = "UNKNOWN";

    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING ) {
        TaskHandle_t current_task = xTaskGetCurrentTaskHandle();
    
        // Check for NULL handle (e.g., if called by an ISR/Timer)
        if (current_task != NULL) {
            task_name = pcTaskGetName(current_task);
        }  
    }
    
    ESP_EARLY_LOGI( QUEUE_RECEIVE, "QUEUE_RECEIVE. Queue Receive: %p\tTimestamp: %lu\tReceived from: %s", pxQueue, xTaskGetTickCount(), task_name );
}
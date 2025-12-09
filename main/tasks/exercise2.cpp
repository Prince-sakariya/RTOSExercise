#include "exercise2.h"
#include "display.h"
#include "logging.h"

#include <esp_log.h>
#include <stdio.h>


// ----------- Task Handles -----------
#define TASK_BIT_PROD1   (1 << 0)
#define TASK_BIT_PROD2   (1 << 1)
#define TASK_BIT_PROD3   (1 << 2)
#define TASK_BIT_PRINT   (1 << 3)

// How large each message can be
#define MSG_SIZE 64

TaskHandle_t masterHandle = NULL;

QueueHandle_t xitemsQueue;
TaskHandle_t ProducerHandle1 = NULL;
TaskHandle_t ProducerHandle2 = NULL;
TaskHandle_t ProducerHandle3 = NULL;
TaskHandle_t PrinterHandle = NULL;

// ------------------------------------------------------
// Queue Creation Wrapper
// ------------------------------------------------------

void createItemsQueue() {
    xitemsQueue = xQueueCreate(3, sizeof( char [ MSG_SIZE] ));
}

// ------------------------------------------------------
// Master Timer Task
// ------------------------------------------------------

void masterTimerStart( void* args ) {
    masterHandle = xTaskGetCurrentTaskHandle();
 
    vTaskDelay(pdMS_TO_TICKS(TASK_RUN_TIME));

    
    // Signal all tasks individually 
    if (ProducerHandle1)        xTaskNotifyGive(ProducerHandle1);
    if (ProducerHandle2)        xTaskNotifyGive(ProducerHandle2);
    if (ProducerHandle3)        xTaskNotifyGive(ProducerHandle3);
    if (PrinterHandle)        xTaskNotifyGive(PrinterHandle);
    
    // Delete the queue AFTER sending stop signals
    QueueHandle_t q = xitemsQueue;
    xitemsQueue = NULL;
    vQueueDelete(q);

    // Wait for all tasks to confirm shutdown
    uint32_t received = 0;
    while ((received & 0x0F) != 0x0F)   // wait for bits 0–3
    {
        uint32_t val;
        xTaskNotifyWait(0, 0, &val, portMAX_DELAY);
        received |= val;
    }
    
    xLoggingEnabled = 0; // <- Stop logging
    LogFlush();
    ESP_LOGI("MASTER", "All tasks stopped. Queue deleted. Log successfully flushed!");
    
    vTaskDelete(NULL);
}

// ------------------------------------------------------
// Task: Producer Task
// ------------------------------------------------------
void producerTask( void* args ) {
    const char* name = ( const char * ) args;
    TickType_t period;
    uint32_t lDoneBit;
    // Choose interval based on name
    if ( strcmp( name, "Line 1") == 0 ) {
        period = 100;
        lDoneBit = TASK_BIT_PROD1;
    }
    else if ( strcmp( name, "Line 2") == 0 ) {
        period = 200;
        lDoneBit = TASK_BIT_PROD2;
    }
    else {
        period = 300;
        lDoneBit = TASK_BIT_PROD3;  
    } 

    TickType_t lastWakeTime = xTaskGetTickCount();
    char xItemtoSend[ MSG_SIZE ];
    
    
    for (;;) {
        // Stop request?
        if (ulTaskNotifyTake(pdTRUE, 0) > 0) 
        break; // stop signal
        
        
        // Stop request again after waking (critical!)
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
        break;

        snprintf( xItemtoSend, MSG_SIZE, "%s item produced!", name );
        
        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick) 
        ESP_LOGE( name , "deadline miss at %lu", tick );
        

        // Send item to printer
        // Send item to queue if it exists
        if ( xitemsQueue ) {
            if ( xQueueSend( xitemsQueue , &xItemtoSend, pdMS_TO_TICKS( period ) ) != pdPASS) {
                ESP_LOGE( name , "Could not send to the queue. Tick: %u\r\n", tick );
            }
        }
        xTaskDelayUntil(&lastWakeTime, period);
    }

    // Notify master we are done
    xTaskNotify(masterHandle, lDoneBit, eSetBits);  // use correct bit per task

    vTaskDelete(NULL);

}


// ------------------------------------------------------
// Task: Printer Task
// ------------------------------------------------------
void printerTask(void* args) {
    char xReceivedItem [ MSG_SIZE ];
    const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );

    // Get item from Queue
    for ( ;; ) {
    
        // stop immediately if requested
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            break;

        // Block indefinitely waiting for new messages
        if ( xitemsQueue ) {
            if ( xQueueReceive( xitemsQueue, &xReceivedItem, xTicksToWait ) == pdPASS ) {
                // ESP_LOGI( "printItems", "Printer output: %s. Tick: %u\r\n", xReceivedItem, tick );
                continue;
            }
        }
    }
    xTaskNotify( masterHandle, TASK_BIT_PRINT, eSetBits );

    vTaskDelete(NULL);
}


// ------------------------------------------------------
// Task Creation Wrappers
// ------------------------------------------------------

void createProducerTask() {
    xTaskCreate( producerTask, "Producer1", 4096, ( void *)"Line 1", 5, &ProducerHandle1 );
    xTaskCreate( producerTask, "Producer2", 4096, ( void *)"Line 2", 5, &ProducerHandle2 );
    xTaskCreate( producerTask, "Producer3", 4096, ( void *)"Line 3", 5, &ProducerHandle3 );
}

void createPrinterTask() {
    xTaskCreate( printerTask, "PrinterTask", 4096, NULL, 6, &PrinterHandle );
}

void createMasterTask() {
    xTaskCreate( masterTimerStart, "masterTimerStart", 4096, NULL, 7, NULL );
}
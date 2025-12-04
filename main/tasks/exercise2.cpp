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

TaskHandle_t masterHandle = NULL;

QueueHandle_t xitemsQueue;
TaskHandle_t ProductionLine1Handle = NULL;
TaskHandle_t ProductionLine2Handle = NULL;
TaskHandle_t ProductionLine3Handle = NULL;
TaskHandle_t PrintingHandle        = NULL;

// ------------------------------------------------------
// Queue Creation Wrapper
// ------------------------------------------------------

void createItemsQueue() {
    xitemsQueue = xQueueCreate(3, sizeof( int32_t ));
}

// ------------------------------------------------------
// Master Timer Task
// ------------------------------------------------------

void masterTimerStart( void* args ) {
    masterHandle = xTaskGetCurrentTaskHandle();
    // ESP_LOGI( "MASTER TIMER", "Timer started");
    // Wait for 1000 ticks
    vTaskDelay(pdMS_TO_TICKS(TASK_RUN_TIME));

    // Signal all tasks individually 
    if (ProductionLine1Handle) xTaskNotifyGive(ProductionLine1Handle);
    if (ProductionLine2Handle) xTaskNotifyGive(ProductionLine2Handle);
    if (ProductionLine3Handle) xTaskNotifyGive(ProductionLine3Handle);
    if (PrintingHandle)        xTaskNotifyGive(PrintingHandle);

    // Wait for all tasks to confirm shutdown
    uint32_t received = 0;
    while ((received & 0x0F) != 0x0F)   // wait for bits 0–3
    {
        uint32_t val;
        xTaskNotifyWait(0, 0, &val, portMAX_DELAY);
        received |= val;
    }

    QueueHandle_t q = xitemsQueue;
    xitemsQueue = NULL;
    vQueueDelete(q);

    // ESP_LOGI("MASTER", "All tasks stopped. Queue deleted.");
    LogFlush();

    vTaskDelete(NULL);
}

// ------------------------------------------------------
// Task: Production Line 1 (0.1s resolution)
// ------------------------------------------------------
void produceItemProductionLine1(void* args) {
    const TickType_t period = 100;
    TickType_t lastWakeTime = xTaskGetTickCount();
    BaseType_t xStatus;
    int32_t lItemtoSend1;
    lItemtoSend1 = 1;

    for (;;) {
        // Stop request?
        if (ulTaskNotifyTake(pdTRUE, 0) > 0) 
        break; // stop signal

        xTaskDelayUntil(&lastWakeTime, period);

        // Stop request again after waking (critical!)
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            break;

        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE( "produceItemProductionLine1", "deadline miss at %lu", tick );
        
        // Send item to printer
        xStatus = xQueueSendToBack( xitemsQueue , &lItemtoSend1, 0 );

        if ( xStatus != pdPASS ) {
            ESP_LOGE( "produceItemProductionLine1", "Could not send to the queue. Tick: %u\r\n", tick );
        }
    }

    // Notify master we are done
    xTaskNotify(masterHandle, TASK_BIT_PROD1, eSetBits);  // use correct bit per task

    vTaskDelete(NULL);
}

// ------------------------------------------------------
// Task: Production Line 2 (0.2s resolution)
// ------------------------------------------------------
void produceItemProductionLine2(void* args) {
    const TickType_t period = 200;
    TickType_t lastWakeTime = xTaskGetTickCount();
    BaseType_t xStatus;
    int32_t lItemtoSend2;
    lItemtoSend2 = 2;
    
    for (;;) {
        // Stop request?
        if (ulTaskNotifyTake(pdTRUE, 0) > 0) 
        break; // stop signal

        xTaskDelayUntil(&lastWakeTime, period);

        // Stop request again after waking (critical!)
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            break;

        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE( "produceItemProductionLine2", "deadline miss at %lu", tick );
        
        // Send item to printer
        xStatus = xQueueSendToBack( xitemsQueue , &lItemtoSend2, 0 );

        if ( xStatus != pdPASS ) {
            ESP_LOGE( "produceItemProductionLine2", "Could not send to the queue. Tick: %u\r\n", tick );
        }
    }

    // Notify master we are done
    xTaskNotify(masterHandle, TASK_BIT_PROD2, eSetBits);  // use correct bit per task

    vTaskDelete(NULL);
}

// ------------------------------------------------------
// Task: Production Line 3 (0.3s resolution)
// ------------------------------------------------------
void produceItemProductionLine3(void* args) {
    const TickType_t period = 300;
    TickType_t lastWakeTime = xTaskGetTickCount();
    BaseType_t xStatus;
    int32_t lItemtoSend3;
    lItemtoSend3 = 3;

    for (;;) {
        // Stop request?
        if (ulTaskNotifyTake(pdTRUE, 0) > 0) 
        break; // stop signal

        xTaskDelayUntil(&lastWakeTime, period);

        // Stop request again after waking (critical!)
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            break;

        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE( "produceItemProductionLine3", "deadline miss at %lu", tick );
        
        // Send item to printer
        xStatus = xQueueSendToBack( xitemsQueue , &lItemtoSend3, 0 );

        if ( xStatus != pdPASS ) {
            ESP_LOGE( "produceItemProductionLine3", "Could not send to the queue. Tick: %u\r\n", tick );
        }
    }

    // Notify master we are done
    xTaskNotify(masterHandle, TASK_BIT_PROD3, eSetBits);  // use correct bit per task

    vTaskDelete(NULL);
}

// ------------------------------------------------------
// Task: Print items
// ------------------------------------------------------
void printItems(void* args) {
    int32_t lReceivedItem;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
    
    // Get item from Queue
    for ( ;; ) {
    
        // stop immediately if requested
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            break;
        
        TickType_t tick = xTaskGetTickCount();
        // if ( uxQueueMessagesWaiting( xitemsQueue ) != 0 ) {
        //     ESP_LOGI( "printItems", "Queue should have been empty!\r\n" );
        // }
        
        xStatus = xQueueReceive(xitemsQueue, &lReceivedItem, xTicksToWait);
        
        // check stop again in case we woke because master sent the signal
        if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            break;

        if ( xStatus != pdPASS ) {
            // Print on serial console
            // ESP_LOGI("printItems", "Printing: Item from Production Line %d", lReceivedItem);
            ESP_LOGE( "printItems", "Could not receive from the queue. Tick: %u\r\n", tick );
        }
        // else {
        //     ESP_LOGE( "printItems", "Could not receive from the queue. Tick: %u\r\n", tick );
        // }
    }
    xTaskNotify(masterHandle, TASK_BIT_PRINT, eSetBits);

    vTaskDelete(NULL);
}


// ------------------------------------------------------
// Task Creation Wrappers
// ------------------------------------------------------
void startProductionLine1Task() {
    xTaskCreate( produceItemProductionLine1, "produceItemProductionLine1", 4096, NULL, 3, &ProductionLine1Handle );
}

void startProductionLine2Task() {
    xTaskCreate( produceItemProductionLine2, "produceItemProductionLine2", 4096, NULL, 3, &ProductionLine2Handle );
}

void startProductionLine3Task() {
    xTaskCreate( produceItemProductionLine3, "produceItemProductionLine3", 4096, NULL, 3, &ProductionLine3Handle );
}

void startPrintingTask() {
    xTaskCreate( printItems, "printItems", 4096, NULL, 3, &PrintingHandle );
}

void startTaskTimer() {
    xTaskCreate( masterTimerStart, "masterTimerStart", 4096, NULL, 5, NULL );
}
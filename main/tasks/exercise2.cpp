#include "exercise2.h"
#include "display.h"

#include <esp_log.h>
#include <stdio.h>

// ----------- Task Creation Handles -----------
QueueHandle_t xitemsQueue;

// ------------------------------------------------------
// Queue Creation Wrapper
// ------------------------------------------------------
// void createItemsQueue(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize) {
//     QUEUE_LENGTH = uxQueueLength;
//     MAX_STRING_LEN = uxItemSize;
//     xitemsQueue = xQueueCreate(QUEUE_LENGTH, MAX_STRING_LEN);
// }

void createItemsQueue() {
    xitemsQueue = xQueueCreate(3, sizeof( int32_t ));
}

// ------------------------------------------------------
// Task: Production Line 1 (0.1s resolution)
// ------------------------------------------------------
void produceItemProductionLine1(void* args) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = 100;
    BaseType_t xStatus;
    int32_t lItemtoSend1;
    lItemtoSend1 = 1;

    for (;;) {
        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE( "produceItemProductionLine1", "deadline miss at %lu", tick );
        
        xTaskDelayUntil(&lastWakeTime, period);
        // Send item to printer
        xStatus = xQueueSendToBack( xitemsQueue , &lItemtoSend1, 0 );

        if ( xStatus != pdPASS ) {
            ESP_LOGE( "produceItemProductionLine1", "Could not send to the queue. Tick: %u\r\n", tick );
        }

    }
}

// ------------------------------------------------------
// Task: Production Line 2 (0.2s resolution)
// ------------------------------------------------------
void produceItemProductionLine2(void* args) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = 200;
    BaseType_t xStatus;
    int32_t lItemtoSend2;
    lItemtoSend2 = 2;

    for (;;) {
        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE( "produceItemProductionLine2", "deadline miss at %lu", tick );
        
        xTaskDelayUntil(&lastWakeTime, period);
        // Send item to printer
        xStatus = xQueueSendToBack( xitemsQueue , &lItemtoSend2, 0 );

        if ( xStatus != pdPASS ) {
            ESP_LOGE( "produceItemProductionLine2", "Could not send to the queue. Tick: %u\r\n", tick );
        }

    }
}

// ------------------------------------------------------
// Task: Production Line 3 (0.3s resolution)
// ------------------------------------------------------
void produceItemProductionLine3(void* args) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = 300;
    BaseType_t xStatus;
    int32_t lItemtoSend3;
    lItemtoSend3 = 3;

    for (;;) {
        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE( "produceItemProductionLine3", "deadline miss at %lu", tick );
        
        xTaskDelayUntil(&lastWakeTime, period);
        // Send item to printer
        xStatus = xQueueSendToBack( xitemsQueue , &lItemtoSend3, 0 );

        if ( xStatus != pdPASS ) {
            ESP_LOGE( "produceItemProductionLine3", "Could not send to the queue. Tick: %u\r\n", tick );
        }

    }
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
        TickType_t tick = xTaskGetTickCount();
        // if ( uxQueueMessagesWaiting( xitemsQueue ) != 0 ) {
        //     ESP_LOGI( "printItems", "Queue should have been empty!\r\n" );
        // }
        
        xStatus = xQueueReceive(xitemsQueue, &lReceivedItem, xTicksToWait);
        
        if ( xStatus == pdPASS ) {
            // Print on serial console
            ESP_LOGI("printItems", "Printing: Item from Production Line %d", lReceivedItem);
        }
        else {
            ESP_LOGE( "printItems", "Could not receive from the queue. Tick: %u\r\n", tick );
        }
    }
}


// ------------------------------------------------------
// Task Creation Wrappers
// ------------------------------------------------------
void startProductionLine1Task() {
    xTaskCreate(produceItemProductionLine1, "produceItemProductionLine1", 4096, NULL, 3, NULL);
}

void startProductionLine2Task() {
    xTaskCreate(produceItemProductionLine2, "produceItemProductionLine2", 4096, NULL, 3, NULL);
}

void startProductionLine3Task() {
    xTaskCreate(produceItemProductionLine3, "produceItemProductionLine3", 4096, NULL, 3, NULL);
}

void startPrintingTask() {
    xTaskCreate(printItems, "printItems", 4096, NULL, 3, NULL);
}


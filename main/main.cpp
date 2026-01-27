#include "display.h"
// #include "tasks/stopwatch.h"
// #include "tasks/exercise2.h"
#include <esp_log.h>
#include "logging/logging.h"

#define PRIORITY_LOW    1
#define PRIORITY_MID    2
#define PRIORITY_HIGH   3

SemaphoreHandle_t xResource;
TaskHandle_t xJ1Handle = NULL;
TaskHandle_t xJ2Handle = NULL;
TaskHandle_t xJ3Handle = NULL;


void vNormalExecution(void)
{
    // Variable for loop
    int i;

    // Loop from 0 to 1000
    for (i = 0; i <= 4000; i++) {
    }
}

void vCriticalExecution(int amount)
{
    // Variable for loop
    int i;

    // Loop from 0 to 1000
    for (i = 0; i <= amount; i++) {
    }
}

void vJ3(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    // vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(0.4));

    for(;;) {
        vNormalExecution();

        // Take semphore and handle priority inversion
        if (xSemaphoreTake(xResource, portMAX_DELAY) == pdTRUE) {

            // Critical Section
            vCriticalExecution(37000);

            xSemaphoreGive(xResource);
            
        }
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));
    }
}

void vJ2(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2));

    for(;;) {
        vNormalExecution();        
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1.5));
    }
}

void vJ1(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));

    for(;;) {
        vNormalExecution();
        // Take semphore and handle priority inversion
        if (xSemaphoreTake(xResource, portMAX_DELAY) == pdTRUE) {

            // Critical section
            vCriticalExecution(9000);
            
            xSemaphoreGive(xResource);
            
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1.25));
    }
}

// Define the task that will stop logging after 100 ticks
void vStopLoggingTask(void *pvParameters) {
    // Wait for 10 ticks
    vTaskDelay(pdMS_TO_TICKS(3.7)); // 10 ticks delay

    // Stop logging by setting xLoggingEnabled to 0
    xLoggingEnabled = 0;
    LogFlush();

    // Delete the task after it performs its job to free resources
    vTaskDelete(NULL);
}

extern "C" void app_main() {

    /*--------------------------------------------------------------------*/
    // Exercise 1
    // Log_Init();
    // xLoggingEnabled = 1; // <-- Start logging
    // startInitDisplayTask();
    // startTimekeepingTask();
    // startTimeDisplayTask();
    // startButtonInputTask();
    // LogFlush();

    /*--------------------------------------------------------------------*/
    // Exercise 2 / Project 
    // Log_Init();
    // xLoggingEnabled = 1; // <-- Start logging
    // // Create the items Queue
    // createItemsQueue();
    // // Start Task Timer
    // createMasterTask();
    // // Create the periodic tasks
    // createProducerTask();
    // // Start Printer
    // createPrinterTask();
    // ESP_LOGI("app_main", "Tasks started");

    /*--------------------------------------------------------------------*/
    // Resource Sharing
    /*--------------------------------------------------------------------*/

    // Note: Use Binary Semaphore to see priority inversion
    xResource = xSemaphoreCreateBinary();
    xSemaphoreGive(xResource);
    
    // PIP, already implemented in mutexes in freertos
    // xResource = xSemaphoreCreateMutex();



    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
    
    xTaskCreate(vJ3,  "J3",  4096, NULL, PRIORITY_LOW,  &xJ3Handle);
    xTaskCreate(vJ2,  "J2",  4096, NULL, PRIORITY_MID, &xJ2Handle);
    xTaskCreate(vJ1, "J1", 4096, NULL, PRIORITY_HIGH, &xJ1Handle);
    xTaskCreate(vStopLoggingTask, "Logger", 4096, NULL, 5, NULL);

    /*--------------------------------------------------------------------*/
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

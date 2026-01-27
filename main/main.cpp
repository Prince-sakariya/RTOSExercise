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

// Pointer to track who currently holds the binary semaphore
volatile TaskHandle_t xResourceOwner = NULL; 

// Manually track the priority of the task holding the semaphore
volatile UBaseType_t xResourceOwnerOriginalPriority = 0;

void vNormalExecution(void)
{
    // Variable for loop
    int i;

    // Loop from 0 to 1000
    for (i = 0; i <= 1000; i++) {
    }
}

void vCriticalExecution(void)
{
    // Variable for loop
    int i;

    // Loop from 0 to 1000
    for (i = 0; i <= 17500; i++) {
    }
}

// Custom Priority Inversion Protocol
void vHandlePriorityInversion(void) {
    if (xResourceOwner != NULL) {
        UBaseType_t currentOwnerPriority = uxTaskPriorityGet(xResourceOwner);
        TaskHandle_t currentTaskHandle = xTaskGetCurrentTaskHandle();
        UBaseType_t currentTaskPriority = uxTaskPriorityGet(currentTaskHandle);

        // If the current task has higher priority than the owner, we need to handle priority inversion
        if (currentTaskPriority > currentOwnerPriority) {
            // Invert priority: temporarily set the priority of the resource owner to the current task's priority
            xResourceOwnerOriginalPriority = currentOwnerPriority;
            vTaskPrioritySet(xResourceOwner, currentTaskPriority);
        }
    }
}

// Restore the original priority of the task that holds the semaphore
void vRestoreOriginalPriority(void) {
    if (xResourceOwner != NULL && xResourceOwnerOriginalPriority != 0) {
        vTaskPrioritySet(xResourceOwner, xResourceOwnerOriginalPriority);
        xResourceOwnerOriginalPriority = 0;
    }
}

void vJ3(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));

    for(;;) {
        vNormalExecution();

        // Take semphore and handle priority inversion
        if (xSemaphoreTake(xResource, portMAX_DELAY) == pdTRUE) {
            xResourceOwner = xTaskGetCurrentTaskHandle();
            vHandlePriorityInversion();

            // Critical Section
            vCriticalExecution();

            // Restore the original priority
            vRestoreOriginalPriority();

            xResourceOwner = NULL; 
            xSemaphoreGive(xResource);
            
        }
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));
    }
}

void vJ2(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(4));

    for(;;) {
        vNormalExecution();        
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(4));
    }
}

void vJ1(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2));

    for(;;) {
        vNormalExecution();

        // Take semphore and handle priority inversion
        if (xSemaphoreTake(xResource, portMAX_DELAY) == pdTRUE) {
            xResourceOwner = xTaskGetCurrentTaskHandle();
            vHandlePriorityInversion(); // Handle priority inversion before entering critical section
            
            // Critical section
            vCriticalExecution();
            
            // Restore the original priority
            vRestoreOriginalPriority();
            
            xResourceOwner = NULL;
            xSemaphoreGive(xResource);
            
        }
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2));
    }
}

// Define the task that will stop logging after 100 ticks
void vStopLoggingTask(void *pvParameters) {
    // Wait for 1000 ticks (assumes tick rate is set to 1000 Hz or 1ms per tick)
    vTaskDelay(pdMS_TO_TICKS(10)); // 20 ticks delay

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
    /*--------------------------------------------------------------------*/
    
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

    // Note: Use Binary Semaphore to allow manual priority manipulation
    xResource = xSemaphoreCreateBinary();
    xSemaphoreGive(xResource);

    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
    
    xTaskCreate(vJ3,  "J3",  4096, NULL, PRIORITY_LOW,  &xJ1Handle);
    xTaskCreate(vJ2,  "J2",  4096, NULL, PRIORITY_MID, &xJ1Handle);
    xTaskCreate(vJ1, "J1", 4096, NULL, PRIORITY_HIGH, &xJ1Handle);
    xTaskCreate(vStopLoggingTask, "Logger", 4096, NULL, 5, NULL);

    /*--------------------------------------------------------------------*/
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

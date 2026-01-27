#include "display.h"
#include <esp_log.h>
#include "logging/logging.h"

// Define the function pointer type for aperiodic tasks
typedef void (*vAperiodicFunction_t)(uint32_t);

// Define a structure for the aperiodic request
typedef struct {
    vAperiodicFunction_t vAperiodicWorkerTask; // The function to call
    uint32_t ulValue;                         // Data to process
} xAperiodicRequest_t;

QueueHandle_t xAperiodicReadyQueue;           // Ready queue for storing the requests
#define APERIODIC_QUEUE_SIZE 10               // Max number of pending requests

void vSetupReadyQueue(void) {
    // Allocate the ready queue
    xAperiodicReadyQueue = xQueueCreate(APERIODIC_QUEUE_SIZE, sizeof(xAperiodicRequest_t));

    if (xAperiodicReadyQueue == NULL) {
        // Handle error: Not enough heap memory
        ESP_LOGE("vSetupReadyQueue", "Failed to create ready queue");
        for(;;); // Trap the system if the queue creation fails
    }
}

BaseType_t xSubmitAperiodicRequest(vAperiodicFunction_t xFunc, uint32_t ulParam) {
    xAperiodicRequest_t xReq;
    xReq.vAperiodicWorkerTask = xFunc;  // Function pointer for the aperiodic request
    xReq.ulValue = ulParam;

    // Send the request to the ready queue
    return xQueueSendToBack(xAperiodicReadyQueue, &xReq, 0);  // Non-blocking
}

// Aperiodic Worker Task
void vAperiodicWorkerTask(void *pvParameters) {
    xAperiodicRequest_t xReceivedRequest;

    for(;;) {
        // Block until something arrives in the ready queue
        if (xQueueReceive(xAperiodicReadyQueue, &xReceivedRequest, portMAX_DELAY) == pdPASS) {
            // Execute the function passed in the structure
            xReceivedRequest.vAperiodicWorkerTask(xReceivedRequest.ulValue);
        }
    }
}

// Computation function (dummy)
void vComputation(uint32_t val) {
    int i;
    for (i = 0; i < val; i++ ) {
        i++;
        i--;
    }
}

// Interrupt Handler (simulates an interrupt causing an aperiodic request)
void vHardwareInterrupt_Handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xAperiodicRequest_t xRequest;

    // Set up the request for computation
    xRequest.vAperiodicWorkerTask = vComputation;
    xRequest.ulValue = 15000;  // Example value to pass to the computation function

    // Send to the ready queue from the interrupt
    xQueueSendToBackFromISR(xAperiodicReadyQueue, &xRequest, &xHigherPriorityTaskWoken);

    // Force a context switch if the worker task has a higher priority
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Periodic Task to handle aperiodic requests from the ready queue
void vPeriodicExecutorTask(void *pvParameters) {
    const TickType_t xPeriod = pdMS_TO_TICKS(100); // Period: 100 ms
    TickType_t xLastWakeTime = xTaskGetTickCount();
    xAperiodicRequest_t xReceivedRequest;

    for(;;) {
        // Wait until the next cycle
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        // Process all available requests in FIFO order
        while (xQueueReceive(xAperiodicReadyQueue, &xReceivedRequest, 0) == pdPASS) {
            // Execute the function
            xReceivedRequest.vAperiodicWorkerTask(xReceivedRequest.ulValue);
        }
    }
}

// Polling-Server (PS) Task
void vPollingServerTask(void *pvParameters) {
    const TickType_t xPeriod = pdMS_TO_TICKS(100);  // Server period: 100 ms
    const uint32_t ulCapacityMax = 5000;           // Maximum "capacity units" per period
    uint32_t ulRemainingCapacity = ulCapacityMax;  // Remaining capacity in current period
    TickType_t xLastWakeTime = xTaskGetTickCount();
    xAperiodicRequest_t xReceivedRequest;

    for (;;) {
        // Wait until the next period
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        // Replenish server capacity at the start of the period
        ulRemainingCapacity = ulCapacityMax;

        // Only serve requests if there is capacity and requests pending
        while ((ulRemainingCapacity > 0) && 
               (xQueueReceive(xAperiodicReadyQueue, &xReceivedRequest, 0) == pdPASS)) {

            // Execute the aperiodic request
            xReceivedRequest.vAperiodicWorkerTask(xReceivedRequest.ulValue);

            // Reduce remaining capacity based on the "cost" of the request
            // Here we assume each request consumes 1 unit of capacity.
            ulRemainingCapacity--;
        }
    }
}

extern "C" void app_main() {

/*--------------------------------------------------------------------*/
    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
    
    // Set up the ready queue for aperiodic requests
    vSetupReadyQueue();

    // Create the worker task to handle aperiodic requests
    if (xTaskCreate(vAperiodicWorkerTask, "AperiodicWorker", 2048, NULL, 2, NULL) != pdPASS) {
        ESP_LOGE("app_main", "Failed to create AperiodicWorker task");
        return;
    }

    // Create the periodic executor task
    if (xTaskCreate(vPeriodicExecutorTask, "PeriodicExecutor", 2048, NULL, 2, NULL) != pdPASS) {
        ESP_LOGE("app_main", "Failed to create PeriodicExecutor task");
        return;
    }

    // Create the Polling-Server task
    if (xTaskCreate(vPollingServerTask, "PollingServer", 2048, NULL, 3, NULL) != pdPASS) {
        ESP_LOGE("app_main", "Failed to create PollingServer task");
        return;
    }

    ESP_LOGI("app_main", "Tasks started");

    /*--------------------------------------------------------------------*/
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

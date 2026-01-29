#include "display.h"
#include <esp_log.h>
#include "logging/logging.h"

QueueHandle_t xAperiodicReadyQueue;           // Ready queue for storing the requests
#define APERIODIC_QUEUE_SIZE 10               // Max number of pending requests
SemaphoreHandle_t xAperiodicTaskSemaphore;    // Binary semaphore

/***********************************************************************************/
                    /* Utils */
/***********************************************************************************/
// Computation function
void vComputation(uint32_t val) {
    int i;
    for (i = 0; i < val; i++ ) {
        i++;
        i--;
    }
}

/***********************************************************************************/
                    /* Task A: Aperiodic Request Generator */
/***********************************************************************************/

// Define the function pointer type for aperiodic tasks
typedef void (*vAperiodicFunction_t)( uint32_t );

// Define a structure for the aperiodic request
typedef struct {
    vAperiodicFunction_t vAperiodicWorkerTask; // The function to call
    uint32_t ulValue;                         // Data to process
} xAperiodicRequest_t;

// Task to generate aperiodic tasks 
void vAperiodicRequestGeneratorTask( void *pvParameters ) {
    xAperiodicRequest_t xRequest;

    // Set up the request for computation
    xRequest.vAperiodicWorkerTask = vComputation;
    xRequest.ulValue = 40000;  // Example value to pass to the computation function


    TickType_t xNextWakeTime = xTaskGetTickCount();
    while ( 1 ) {
        vTaskDelayUntil( &xNextWakeTime, pdMS_TO_TICKS( rand() % 10 + 35 )); // Random delay

        // Send to the ready queue
        if (xQueueSendToBack(xAperiodicReadyQueue, &xRequest, 0) != pdPASS) {
            ESP_LOGW("Aperiodic Generator", "Queue send failed");
        }

    }
}

// Aperiodic Worker Task (Not a periodic task) (relavant for only tasks a and b)
// void vAperiodicRequestExecutorTask(void *pvParameters) {
//     xAperiodicRequest_t xReceivedRequest;
//     for(;;) {
//         // Block until something arrives in the ready queue
//         if (xQueueReceive(xAperiodicReadyQueue, &xReceivedRequest, portMAX_DELAY) == pdPASS) {
//             // Execute the function passed in the structure
//             xReceivedRequest.vAperiodicWorkerTask(xReceivedRequest.ulValue);
//             // After execution, the task should be blocked until a new request arrives
//             xSemaphoreTake(xAperiodicTaskSemaphore, portMAX_DELAY);
//         }
//     }
// }

/***********************************************************************************/
                    /* Task B: Ready Queue for Apperiodic Requests */
/***********************************************************************************/
// Ready Queue for storing aperiodic requests
void vSetupAperiodicTasksReadyQueue(void) {
    // Allocate the ready queue
    xAperiodicReadyQueue = xQueueCreate(APERIODIC_QUEUE_SIZE, sizeof(xAperiodicRequest_t));

    if (xAperiodicReadyQueue == NULL) {
        // Handle error: Not enough heap memory
        ESP_LOGE("vSetupReadyQueue", "Failed to create ready queue");
        for(;;); // Trap the system if the queue creation fails
    }
}

/***********************************************************************************/
                    /* Task C Periodic Task (Executes Apperiodic Request) */
/***********************************************************************************/

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
            // After execution, the task should be blocked until a new request arrives
            // xSemaphoreTake(xAperiodicTaskSemaphore, portMAX_DELAY);
            vComputation(1);
        }
    }
}

// BaseType_t xSubmitAperiodicRequest(vAperiodicFunction_t xFunc, uint32_t ulParam) {
//     xAperiodicRequest_t xReq;
//     xReq.vAperiodicWorkerTask = xFunc;  // Function pointer for the aperiodic request
//     xReq.ulValue = ulParam;
//     // Send the request to the ready queue
//     return xQueueSendToBack(xAperiodicReadyQueue, &xReq, 0);  // Non-blocking
// }

// // Polling-Server (PS) Task
// void vPollingServerTask(void *pvParameters) {
//     const TickType_t xPeriod = pdMS_TO_TICKS(100);  // Server period: 100 ms
//     const uint32_t ulCapacityMax = 5000;           // Maximum "capacity units" per period
//     uint32_t ulRemainingCapacity = ulCapacityMax;  // Remaining capacity in current period
//     TickType_t xLastWakeTime = xTaskGetTickCount();
//     xAperiodicRequest_t xReceivedRequest;
//     for (;;) {
//         // Wait until the next period
//         vTaskDelayUntil(&xLastWakeTime, xPeriod);
//         // Replenish server capacity at the start of the period
//         ulRemainingCapacity = ulCapacityMax;
//         // Only serve requests if there is capacity and requests pending
//         while ((ulRemainingCapacity > 0) && 
//                (xQueueReceive(xAperiodicReadyQueue, &xReceivedRequest, 0) == pdPASS)) {
//             // Execute the aperiodic request
//             xReceivedRequest.vAperiodicWorkerTask(xReceivedRequest.ulValue);
//             // Reduce remaining capacity based on the "cost" of the request
//             // Here we assume each request consumes 1 unit of capacity.
//             ulRemainingCapacity--;
//         }
//     }
// }

/***********************************************************************************/
                    /* Miscellaneous (Logging) */
/***********************************************************************************/
void vStopLoggingTask(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(300));

    // Stop logging by setting xLoggingEnabled to 0
    xLoggingEnabled = 0;
    LogFlush();

    // Delete the task after it performs its job to free resources
    vTaskDelete(NULL);
}

/***********************************************************************************/
                    /* MAIN*/
/***********************************************************************************/

extern "C" void app_main() {
/*--------------------------------------------------------------------*/
    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
    
    // Binary Semaphore
    // xAperiodicTaskSemaphore = xSemaphoreCreateBinary();

    // Set up the ready queue for aperiodic requests
    vSetupAperiodicTasksReadyQueue();

    // Create aperiodic request generator task
    xTaskCreate( vAperiodicRequestGeneratorTask, "AperiodicReqGen", 4096, NULL, 2, NULL );
    
    // Create the executor aperiodic requests ( only for tasks a and b)
    // xTaskCreate( vAperiodicRequestExecutorTask, "AperiodicReqExecutor", 2048, NULL, 2, NULL );
    
    // Create periodic task for executing aperiodic requests
    xTaskCreate( vPeriodicExecutorTask, "PeriodicExecuter", 4096, NULL, 2, NULL );

    // Logger
    xTaskCreate(vStopLoggingTask, "Logger", 4096, NULL, 5, NULL);

    ESP_LOGI("app_main", "Tasks started");

    /*--------------------------------------------------------------------*/
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

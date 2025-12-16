#include "display.h"
// #include "tasks/stopwatch.h"
#include "tasks/exercise2.h"
#include <esp_log.h>
#include "logging/logging.h"

// void vTaskFunction( void * pvParameters )
// {
//     TickType_t xLastWakeTime;
//     xLastWakeTime = xTaskGetTickCount();
//     char *pcTaskName;
//     pcTaskName = ( char * ) pvParameters;
//     for( ;; )
//     {
//         /* Print out the name of this task. */
//         ESP_EARLY_LOGI( "Task", "%s",pcTaskName );
//         vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 250 ) );
//     }
// }



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
    
    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
 
// Create the items Queue
createItemsQueue();

// Start Task Timer
createMasterTask();

// Create the periodic tasks
createProducerTask();

// Start Printer
createPrinterTask();

    ESP_LOGI("app_main", "Tasks started");
    /*--------------------------------------------------------------------*/

    // static const char * pcTextForTask1 = "Task 1 is running";
    // static const char * pcTextForTask2 = "Task 2 is running";
    // /* Create the first task with a priority of 1. */
    // xTaskCreate( vTaskFunction, "Task 1", 1000, ( void * ) pcTextForTask1, 1, NULL );
    // /* Create the second task at a higher priority of 2. */
    // xTaskCreate( vTaskFunction, "Task 2", 1000, ( void * ) pcTextForTask2, 2, NULL );
    
    // LogFlush();


    /*--------------------------------------------------------------------*/
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

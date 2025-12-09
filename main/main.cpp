#include "display.h"
#include "tasks/stopwatch.h"
#include "tasks/exercise2.h"
#include <esp_log.h>
#include "logging/logging.h"

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
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

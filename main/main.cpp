#include "display.h"
// #include "tasks/stopwatch.h"
#include "tasks/exercise2.h"
#include <esp_log.h>

extern "C" void app_main() {

    // Exercise 1
    // startInitDisplayTask();
    // startTimekeepingTask();
    // startTimeDisplayTask();
    // startButtonInputTask();

    // Exercise 2
    // Create the items Queue
    createItemsQueue();

    // Start Task Timer
    startTaskTimer();

    // Create the periodic tasks
    startProductionLine1Task();
    startProductionLine2Task();
    startProductionLine3Task();

    // Start Printer
    startPrintingTask();

    ESP_LOGI("app_main", "Tasks started");

    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

#include "display.h"
// #include "tasks/stopwatch.h"
// #include "tasks/exercise2.h"
#include "tasks/exercise3.h"
#include <esp_log.h>
// #include "logging/logging.h"

// // ------------------------------------------------------
// // Interrupt Handler
// // ------------------------------------------------------
void interruptHandler( void *args ) {
    int gpio_num = ( int )args;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Wait for the semaphore before processing the interrupt
    if ( xSemaphoreTakeFromISR( xInterruptSemaphore, &xHigherPriorityTaskWoken ) ==  pdTRUE ) {
        // Now we can safely process the interrupt
        if ( gpio_num == BOTTOM_LEFT ) {
            if ( clockStatus == 0 ) {
            printStatus = 0;
            clockStatus = 1;
            } else if ( clockStatus == 1 ) {
                printStatus = 0;
                clockStatus = 2;
            } else if ( clockStatus == 2 ) {
                printStatus = 0;
                clockStatus = 1;
            }
        } 
        else if ( gpio_num == TOP_LEFT ) {
            printStatus = 0;
            clockStatus = 0;
        }

        // Release the semaphore after processing the interrupt
        xSemaphoreGiveFromISR( xInterruptSemaphore, &xHigherPriorityTaskWoken );

        // Optionally, force a context switch if higher priority tasks are woken
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
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

    /*--------------------------------------------------------------------*/
    // Exercise 3

    // // Enable Interrupts by GPIO pins at runtime
    // gpio_install_isr_service(0);

    // // Binary Semaphore 
    // // to fix interrupt overlap
    // xInterruptSemaphore = xSemaphoreCreateBinary();
    // if ( xInterruptSemaphore != NULL ) {
    //     // Initially, the semaphore is available
    //     xSemaphoreGive( xInterruptSemaphore );
    // } 
    
    // // Set Interrupts
    // gpio_set_intr_type( BOTTOM_LEFT, GPIO_INTR_NEGEDGE );
    // gpio_set_intr_type( TOP_LEFT, GPIO_INTR_NEGEDGE );
    // gpio_isr_handler_add( BOTTOM_LEFT, interruptHandler, ( void* )BOTTOM_LEFT );
    // gpio_isr_handler_add( TOP_LEFT, interruptHandler, ( void* )TOP_LEFT );

    // startInitDisplayTask();
    // startTimekeepingTask();
    // startTimeDisplayTask();

/*--------------------------------------------------------------------*/
    // Exercise 4

    ESP_LOGI("app_main", "Tasks started");

    /*--------------------------------------------------------------------*/
    vTaskStartScheduler();
    /* vTaskStartScheduler is blocking - this should never be reached */
    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}

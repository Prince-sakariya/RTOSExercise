#ifndef TASKS_EXERCISE2
#define TASKS_EXERCISE2

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// ----------- Tags -------------

// ----------- Timing -----------
#define TASK_RUN_TIME 1000

// ----------- Task Creation Handles -----------
void createProducerTask();
void createPrinterTask();
void createMasterTask();

// ----------- Task Creation Wrappers -----------
void producerTask( void* args );
void printItems( void* args );
void masterTimerStart( void* args );

// ----------- Queue Creation Wrapper -----------
// void createItemsQueue(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize);
void createItemsQueue();

#endif // TASKS_EXERCISE2
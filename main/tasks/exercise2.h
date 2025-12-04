#ifndef TASKS_EXERCISE2
#define TASKS_EXERCISE2

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

// ----------- Tags -------------

// ----------- Timing -----------
#define TICKS_PER_MS 1000
#define TASK_RUN_TIME 1000

// ----------- Task Creation Handles -----------
void startProductionLine1Task();
void startProductionLine2Task();
void startProductionLine3Task();
void startPrintingTask();
void startTaskTimer();

// ----------- Task Creation Wrappers -----------
void produceItemProductionLine1( void* args );
void produceItemProductionLine2( void* args );
void produceItemProductionLine3( void* args );
void printItems( void* args );
void masterTimerStart( void* args );

// ----------- Queue Creation Wrapper -----------
// void createItemsQueue(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize);
void createItemsQueue();

#endif // TASKS_EXERCISE2
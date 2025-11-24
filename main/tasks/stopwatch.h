#ifndef TASKS_STOPWATCH_H
#define TASKS_STOPWATCH_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// ----------- Timing -----------
#define TICKS_PER_MS 1000

// ----------- Global Stopwatch State -----------
extern unsigned short buttonPressed;
extern unsigned short clockStatus;
extern unsigned short printStatus;
extern unsigned int   clockCount;

// ----------- Task Creation Wrappers -----------
// void startInitDisplayTask();
void startTimekeepingTask();
void startTimeDisplayTask();
void startButtonInputTask();

// ----------- Task Creation Wrappers -----------
// void startInitDisplayTask();
void timekeeping(void* args);
void timeDisplay(void* args);
void buttonInput(void* args);

// ----------- Helper Functions -----------
void printTimeOnDisplay(unsigned int time, bool paused);

#endif // TASKS_STOPWATCH_H

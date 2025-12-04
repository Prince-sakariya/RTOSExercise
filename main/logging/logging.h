#ifndef LOGGING_H
#define LOGGING_H

#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

// ------------------ Log Entry Struct ------------------
typedef struct {
    const char *event;
    TickType_t tickCount;      // FreeRTOS tick
    uint32_t microSeconds;     // Fine-grained timestamp
    QueueHandle_t queueHandle;         // Queue (handle) pointer 
    TickType_t waitTicks;      // Max ticks the task will wait
    const char* taskName;      // Calling task name
} LogEntry_t;

// ------------------ Logging Flag -----------------
extern volatile TickType_t xLoggingEnabled;

// ------------------ Logging API ------------------
#ifdef __cplusplus
extern "C" {
#endif
void Log_Init(void);  // optional if needed
void LogEvent(
    const char *event, 
    TickType_t tickCount, 
    QueueHandle_t pxQueue, 
    TickType_t waitTicks, 
    const char* taskName 
);

void LogFlush(void);

#ifdef __cplusplus
}
#endif
#endif // LOGGING_H

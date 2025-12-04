#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LOG_BUFFER_SIZE 1024
#define LOG_TAG "APP_LOG"

static LogEntry_t logBuffer[LOG_BUFFER_SIZE];
static volatile uint32_t logHead = 0;
static volatile uint32_t logTail = 0;

void Log_Init(void)
{
    logHead = 0;
    logTail = 0;
}

// Add a log entry (fast, non-blocking)
void LogEvent(const char *event, char *obj, uint32_t data)
{
    uint32_t i = logHead % LOG_BUFFER_SIZE;
    logBuffer[i].timestamp = xTaskGetTickCount();
    logBuffer[i].event = event;
    logBuffer[i].obj = obj;
    logBuffer[i].data = data;

    logHead++;
    if (logHead - logTail > LOG_BUFFER_SIZE) {
        logTail = logHead - LOG_BUFFER_SIZE; // drop oldest
    }
}

// Flush the log buffer using ESP_LOGI
void LogFlush(void)
{
    while (logTail < logHead) {
        uint32_t i = logTail % LOG_BUFFER_SIZE;
        LogEntry_t *e = &logBuffer[i];
        ESP_LOGI(LOG_TAG, "[%lu] %s",
                 e->timestamp, e->event);
        logTail++;
    }

    // ESP_LOGI(LOG_TAG, "FLUSHING");
}

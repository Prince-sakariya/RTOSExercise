#ifndef LOGGING_H
#define LOGGING_H

#include "stdint.h"

// ------------------ Log Entry ------------------
typedef struct {
    uint32_t timestamp;
    const char *event;
    void *obj;
    uint32_t data;
} LogEntry_t;

// ------------------ Logging API ------------------
#ifdef __cplusplus
extern "C" {
#endif
void Log_Init(void);  // optional if needed
void LogEvent(const char *event, char *obj, uint32_t data);
void LogFlush(void);

#ifdef __cplusplus
}
#endif
#endif // LOGGING_H

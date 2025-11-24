#include "stopwatch.h"
#include "display.h"
#include <esp_log.h>
#include <stdio.h>
#include <Arduino.h>

// ----------- Global Variables -----------
unsigned short buttonPressed = 0;
unsigned short clockStatus = 0;
unsigned short printStatus = 0;
unsigned int   clockCount = 0;

// ------------------------------------------------------
// Helper: Print time to e-paper display
// ------------------------------------------------------
void printTimeOnDisplay(unsigned int time, bool paused) {
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(5, 53 + 60);

    double ms = (double)clockCount / TICKS_PER_MS;
    display.print(ms);
    display.println("s");

    if (paused) {
        display.println("-");
    }

    display.display(true);  // partial update
}

// ------------------------------------------------------
// Task: Timekeeping (0.1s resolution)
// ------------------------------------------------------
void timekeeping(void* args) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = 100;

    for (;;) {
        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE("timekeeping", "deadline miss at %lu", tick);

        xTaskDelayUntil(&lastWakeTime, period);

        clockCount += period * (clockStatus == 1 || clockStatus == 3);
    }
}

// ------------------------------------------------------
// Task: Update the display once per second
// ------------------------------------------------------
void timeDisplay(void* args) {
    printf("\n\nWelcome to sophisticated stop watch:\n\n");

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = 1000;

    for (;;) {
        TickType_t tick = xTaskGetTickCount();
        if (lastWakeTime + period < tick)
            ESP_LOGE("timeDisplay", "deadline miss at %lu", tick);

        xTaskDelayUntil(&lastWakeTime, period);

        if (clockStatus == 0) {
            if (printStatus == 0) {
                clockCount = 0;
                printf("Stop Watch - Ready - Press bottom left button to start\n");
                printTimeOnDisplay(clockCount, true);
                printStatus = 1;
            }
        } else if (clockStatus == 1) {
            printf("Stop Watch - Running (%fs)\n", (double)clockCount / TICKS_PER_MS);
            printTimeOnDisplay(clockCount, false);

        } else if (clockStatus == 2) {
            if (printStatus == 0) {
                printf("Stop Watch - Stopped: (%fs)\n", (double)clockCount / TICKS_PER_MS);
                printTimeOnDisplay(clockCount, true);
                printStatus = 1;
            }
        }
    }
}

// ------------------------------------------------------
// Task: Button handling
// ------------------------------------------------------
void buttonInput(void* args) {
    for (;;) {
        if (clockStatus == 0) {
            if (digitalRead(BOTTOM_LEFT) == HIGH) {
                printStatus = 0;
                clockStatus = 1;
                vTaskDelay(1000);
            }
        } else if (clockStatus == 1) {
            if (digitalRead(BOTTOM_LEFT) == HIGH) {
                printStatus = 0;
                clockStatus = 2;
                vTaskDelay(1000);
            }
        } else if (clockStatus == 2) {
            if (digitalRead(BOTTOM_LEFT) == HIGH) {
                printStatus = 0;
                clockStatus = 1;
                vTaskDelay(1000);
            } else if (digitalRead(TOP_LEFT) == HIGH) {
                printStatus = 0;
                clockStatus = 0;
                vTaskDelay(1000);
            }
        }
    }
}

// ------------------------------------------------------
// Task Creation Wrappers
// ------------------------------------------------------
void startTimekeepingTask() {
    xTaskCreate(timekeeping, "timekeeping", 4096, NULL, 3, NULL);
}

void startTimeDisplayTask() {
    xTaskCreate(timeDisplay, "timeDisplay", 4096, NULL, 2, NULL);
}

void startButtonInputTask() {
    xTaskCreate(buttonInput, "buttonInput", 4096, NULL, 1, NULL);
}

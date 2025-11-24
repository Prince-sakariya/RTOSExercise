#include "display.h"


#include <esp_log.h>
#include <stdio.h>
#include <Arduino.h>

// ----------- Display Object -----------
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display(WatchyDisplay{});


// ------------------------------------------------------
// Task: Display Initialization
// ------------------------------------------------------
void initDisplay(void* pvParameters) {
    ESP_LOGI("initDisplay", "initializing display");

    pinMode(DISPLAY_CS, OUTPUT);
    pinMode(DISPLAY_RES, OUTPUT);
    pinMode(DISPLAY_DC, OUTPUT);
    pinMode(DISPLAY_BUSY, OUTPUT);
    pinMode(BOTTOM_LEFT, INPUT);
    pinMode(BOTTOM_RIGHT, INPUT);
    pinMode(TOP_LEFT, INPUT);
    pinMode(TOP_RIGHT, INPUT);

    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    display.init(0, true, 10, true);
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold24pt7b);
    display.setCursor(0, 90);
    display.display(false);

    ESP_LOGI("initDisplay", "finished display initialization");
    vTaskDelete(NULL);
}

// ------------------------------------------------------
// Task Creation Wrapper
// ------------------------------------------------------
void startInitDisplayTask() {
    xTaskCreate(initDisplay, "initDisplay", 4096, NULL, configMAX_PRIORITIES - 1, NULL);
}

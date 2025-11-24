#ifndef DISPLAY_H
#define DISPAY_H

#include <Display.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <GxEPD2_BW.h>

// ----------- GPIO Pins -----------
#define BOTTOM_LEFT     26
#define TOP_LEFT        25
#define BOTTOM_RIGHT    4
#define TOP_RIGHT       35
#define DISPLAY_CS      5
#define DISPLAY_RES     9
#define DISPLAY_DC      10
#define DISPLAY_BUSY    19

extern GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;

// ----------- Task Creation Wrappers -----------
void startInitDisplayTask();

// ----------- Task Creation Wrappers -----------
void startInitDisplayTask();

#endif // DISPLAY_H
#ifndef ESP_POMODORO_CLOCK_LEDMANAGER_H
#define ESP_POMODORO_CLOCK_LEDMANAGER_H

#include "FastLED.h"

#define LED_UPDATE_EVERY_MS 40

class LEDManager {
    CRGB ledArray[2]{};
    ulong &currentTickMs;
    ulong lastUpdateMs = 0;

    bool shouldUpdate();
    void display(uint keepFrameForMs);

public:
    explicit LEDManager(ulong &currentTickMs) : currentTickMs(currentTickMs) {}

    void setup();

    void off();

    void showTouch(bool left, bool center, bool right);

    void animateAlarm();
};


#endif //ESP_POMODORO_CLOCK_LEDMANAGER_H

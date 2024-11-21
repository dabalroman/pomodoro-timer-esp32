#ifndef ESP_POMODORO_CLOCK_LEDMANAGER_H
#define ESP_POMODORO_CLOCK_LEDMANAGER_H

#include "FastLED.h"
#include "LEDManagerState.h"

#define LED_UPDATE_EVERY_MS 25

#define LED_GPIO 12
#define LED_TYPE WS2811
#define LED_COLOR_ORDER GRB
#define LED_AMOUNT 2
#define LED_BRIGHTNESS 64

class LEDManager {
    LEDManagerState state = LEDManagerState::off;
    CRGB ledArray[2]{};
    ulong &currentTickMs;
    ulong lastUpdateMs = 0;

    bool shouldUpdate() const;

    void renderTouchLeft();

    void renderTouchCenter();

    void renderTouchRight();

    void renderOff();

    void renderRinging();

    void renderCountingDown();

    void renderCountdownPaused();

public:
    explicit LEDManager(ulong &currentTickMs) : currentTickMs(currentTickMs) {}

    void setup();

    void update();

    void setState(LEDManagerState newState);
};


#endif //ESP_POMODORO_CLOCK_LEDMANAGER_H

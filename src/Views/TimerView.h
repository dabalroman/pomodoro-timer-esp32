//
// Created by rd on 18/11/2024.
//

#ifndef ESP_POMODORO_CLOCK_TIMERVIEW_H
#define ESP_POMODORO_CLOCK_TIMERVIEW_H


#include "View.h"
#include "DeviceState.h"

class TimerView : public View {
    ulong &countdownStartValueMs;
    ulong &countdownStartTickMs;
    state &deviceState;
    ulong &lastTickMs;

public:
    TimerView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            ulong &countdownStartValueMs,
            ulong &countdownStartTickMs,
            state &deviceState,
            ulong &lastTickMs
    ) : View(display, touch),
        countdownStartValueMs(countdownStartValueMs),
        countdownStartTickMs(countdownStartTickMs),
        deviceState(deviceState),
        lastTickMs(lastTickMs) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_TIMERVIEW_H

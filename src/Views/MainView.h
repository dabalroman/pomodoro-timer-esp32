#ifndef ESP_POMODORO_CLOCK_MAINVIEW_H
#define ESP_POMODORO_CLOCK_MAINVIEW_H

#include "View.h"
#include "DeviceState.h"

class MainView : public View {
    ulong &countdownStartValueMs;
    ulong &countdownStartTickMs;
    DeviceState &deviceState;
    ulong &lastTickMs;

public:
    MainView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &countdownStartValueMs,
            ulong &countdownStartTickMs,
            DeviceState &deviceState,
            ulong &lastTickMs
    ) : View(display, touch, ledManager),
        countdownStartValueMs(countdownStartValueMs),
        countdownStartTickMs(countdownStartTickMs),
        deviceState(deviceState),
        lastTickMs(lastTickMs) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_MAINVIEW_H

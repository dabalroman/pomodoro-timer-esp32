#ifndef ESP_POMODORO_CLOCK_MAINVIEW_H
#define ESP_POMODORO_CLOCK_MAINVIEW_H

#include "View.h"
#include "DeviceState.h"

class MainView : public View {
    ulong &countdownStartValueMs;
    ulong &countdownStartTickMs;
    DeviceState &deviceState;

public:
    MainView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            ulong &countdownStartValueMs,
            ulong &countdownStartTickMs,
            DeviceState &deviceState
    ) : View(display, touch, ledManager, lastTickMs),
        countdownStartValueMs(countdownStartValueMs),
        countdownStartTickMs(countdownStartTickMs),
        deviceState(deviceState) {}

    void handleInput() override;

    void render() override;

    bool shouldRender() const override;
};


#endif //ESP_POMODORO_CLOCK_MAINVIEW_H

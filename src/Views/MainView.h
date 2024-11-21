#ifndef ESP_POMODORO_CLOCK_MAINVIEW_H
#define ESP_POMODORO_CLOCK_MAINVIEW_H

#include "View.h"

class MainView : public View {
    ulong &countdownStartValueMs;
    ulong &countdownStartTickMs;

public:
    MainView(
            DeviceState &deviceState,
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            ulong &countdownStartValueMs,
            ulong &countdownStartTickMs
    ) : View(deviceState, display, touch, ledManager, lastTickMs),
        countdownStartValueMs(countdownStartValueMs),
        countdownStartTickMs(countdownStartTickMs) {}

    void handleInput() override;

    void render() override;

    bool shouldRender() const override;
};


#endif //ESP_POMODORO_CLOCK_MAINVIEW_H

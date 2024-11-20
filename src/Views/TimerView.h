#ifndef ESP_POMODORO_CLOCK_TIMERVIEW_H
#define ESP_POMODORO_CLOCK_TIMERVIEW_H

#include "View.h"
#include "DeviceState.h"

class TimerView : public View {
    ulong &countdownStartValueMs;
    ulong &countdownStartTickMs;
    DeviceState &deviceState;

    // Pause feature
    bool isPaused = false;
    bool prevIsPaused = false;
    ulong countdownPausedValueMs = 0;
    ulong countdownStartValueMsBackup = 0;

public:
    TimerView(
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

    ulong getTimeLeftMs() const;

    void onExit();
};


#endif //ESP_POMODORO_CLOCK_TIMERVIEW_H

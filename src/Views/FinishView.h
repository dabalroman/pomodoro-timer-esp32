#ifndef ESP_POMODORO_CLOCK_FINISHVIEW_H
#define ESP_POMODORO_CLOCK_FINISHVIEW_H

#include "View.h"

class FinishView : public View {
    uint8_t &pomodoroFinishedAmount;

public:
    FinishView(
            DeviceState &deviceState,
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            uint8_t &pomodoroFinishedAmount
    ) : View(deviceState, display, touch, ledManager, lastTickMs),
        pomodoroFinishedAmount(pomodoroFinishedAmount) {}

    void handleInput() override;

    void render() override;

    bool shouldRender() const override;
};

#endif //ESP_POMODORO_CLOCK_FINISHVIEW_H

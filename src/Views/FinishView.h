#ifndef ESP_POMODORO_CLOCK_FINISHVIEW_H
#define ESP_POMODORO_CLOCK_FINISHVIEW_H

#include "View.h"
#include "DeviceState.h"

class FinishView : public View {
    DeviceState &deviceState;

public:
    FinishView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            DeviceState &deviceState
    ) : View(display, touch, ledManager, lastTickMs),
        deviceState(deviceState) {}

    void handleInput() override;

    void render() override;

    bool shouldRender() const override;
};

#endif //ESP_POMODORO_CLOCK_FINISHVIEW_H

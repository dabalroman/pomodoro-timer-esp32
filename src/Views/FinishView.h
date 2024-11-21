#ifndef ESP_POMODORO_CLOCK_FINISHVIEW_H
#define ESP_POMODORO_CLOCK_FINISHVIEW_H

#include "View.h"

class FinishView : public View {
public:
    FinishView(
            DeviceState &deviceState,
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs
    ) : View(deviceState, display, touch, ledManager, lastTickMs) {}

    void handleInput() override;

    void render() override;

    bool shouldRender() const override;
};

#endif //ESP_POMODORO_CLOCK_FINISHVIEW_H

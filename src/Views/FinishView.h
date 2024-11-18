#ifndef ESP_POMODORO_CLOCK_FINISHVIEW_H
#define ESP_POMODORO_CLOCK_FINISHVIEW_H

#include "View.h"
#include "DeviceState.h"

class FinishView : public View {
    state &deviceState;
    ulong &lastTickMs;

public:
    FinishView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            state &deviceState,
            ulong &lastTickMs
    ) : View(display, touch),
        deviceState(deviceState),
        lastTickMs(lastTickMs) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_FINISHVIEW_H

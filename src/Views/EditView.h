#ifndef ESP_POMODORO_CLOCK_EDITVIEW_H
#define ESP_POMODORO_CLOCK_EDITVIEW_H

#include "View.h"
#include "DeviceState.h"
#include "PreferencesManager.h"

class EditView : public View {
    PreferencesManager &preferencesManager;
    ulong &countdownStartValueMs;
    state &deviceState;
    ulong &lastTickMs;

public:
    EditView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            PreferencesManager &preferencesManager,
            ulong &countdownStartValueMs,
            state &deviceState,
            ulong &lastTickMs
    ) : View(display, touch),
        preferencesManager(preferencesManager),
        countdownStartValueMs(countdownStartValueMs),
        deviceState(deviceState),
        lastTickMs(lastTickMs) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_EDITVIEW_H

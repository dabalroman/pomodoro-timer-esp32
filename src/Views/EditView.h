#ifndef ESP_POMODORO_CLOCK_EDITVIEW_H
#define ESP_POMODORO_CLOCK_EDITVIEW_H

#include "View.h"
#include "DeviceState.h"
#include "PreferencesManager.h"

class EditView : public View {
    PreferencesManager &preferencesManager;
    ulong &countdownStartValueMs;
    DeviceState &deviceState;

    bool hasEditBeenRendered = true;

public:
    EditView(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            PreferencesManager &preferencesManager,
            ulong &countdownStartValueMs,
            DeviceState &deviceState
    ) : View(display, touch, ledManager, lastTickMs),
        preferencesManager(preferencesManager),
        countdownStartValueMs(countdownStartValueMs),
        deviceState(deviceState) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_EDITVIEW_H

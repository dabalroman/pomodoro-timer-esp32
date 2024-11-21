#ifndef ESP_POMODORO_CLOCK_EDITTIMERVIEW_H
#define ESP_POMODORO_CLOCK_EDITTIMERVIEW_H

#include "View.h"
#include "PreferencesManager.h"

class EditTimerView : public View {
    PreferencesManager &preferencesManager;
    ulong &countdownStartValueMs;

    bool hasEditBeenRendered = true;

public:
    EditTimerView(
            DeviceState &deviceState,
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            PreferencesManager &preferencesManager,
            ulong &countdownStartValueMs
    ) : View(deviceState, display, touch, ledManager, lastTickMs),
        preferencesManager(preferencesManager),
        countdownStartValueMs(countdownStartValueMs) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_EDITTIMERVIEW_H

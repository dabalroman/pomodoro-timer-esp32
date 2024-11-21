#ifndef ESP_POMODORO_CLOCK_SETTINGSVIEW_H
#define ESP_POMODORO_CLOCK_SETTINGSVIEW_H


#include "View.h"
#include "PreferencesManager.h"

enum Settings {
    timerLength = 0,
    targetAmountOfPomodoros = 1,
    goBack = 2
};

class SettingsView : public View {
    PreferencesManager &preferencesManager;

    uint8_t selectedOptionId = 0;
    const uint8_t amountOfOptions = 3;

public:
    SettingsView(
            DeviceState &deviceState,
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            PreferencesManager &preferencesManager
    ) : View(deviceState, display, touch, ledManager, lastTickMs),
        preferencesManager(preferencesManager) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_SETTINGSVIEW_H

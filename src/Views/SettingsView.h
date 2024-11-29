#ifndef ESP_POMODORO_CLOCK_SETTINGSVIEW_H
#define ESP_POMODORO_CLOCK_SETTINGSVIEW_H


#include "View.h"
#include "PreferencesManager.h"

enum Settings {
    timerLength = 0,
    targetAmountOfPomodoros = 1,
    wifiCredentials = 2,
    syncTimeViaNTP = 3,
    goBack = 4,
};

class SettingsView : public View {
    PreferencesManager &preferencesManager;

    uint8_t selectedOptionId = 0;
    uint8_t optionsListOffset = 0;
    const uint8_t amountOfOptionsOnScreen = 3;
    const uint8_t amountOfOptions = Settings::goBack + 1;

    const char *options[Settings::goBack + 1] = {
            "Timer length",
            "Pomodoro target",
            "Wifi credentials",
            "Wifi sync time",
            "Return",
    };

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

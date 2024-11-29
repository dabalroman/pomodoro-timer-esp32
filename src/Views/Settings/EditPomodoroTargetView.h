#ifndef ESP_POMODORO_CLOCK_EDITPOMODOROTARGETVIEW_H
#define ESP_POMODORO_CLOCK_EDITPOMODOROTARGETVIEW_H

#include "Views/View.h"
#include "PreferencesManager.h"

class EditPomodoroTargetView : public View {
    PreferencesManager &preferencesManager;
    uint8_t &pomodoroTargetAmount;

    bool hasEditBeenRendered = true;

public:
    EditPomodoroTargetView(
            DeviceState &deviceState,
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs,
            PreferencesManager &preferencesManager,
            uint8_t &pomodoroTargetAmount
    ) : View(deviceState, display, touch, ledManager, lastTickMs),
        preferencesManager(preferencesManager),
        pomodoroTargetAmount(pomodoroTargetAmount) {}

    void handleInput() override;

    void render() override;
};


#endif //ESP_POMODORO_CLOCK_EDITPOMODOROTARGETVIEW_H

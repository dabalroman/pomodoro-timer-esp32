#ifndef ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H
#define ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H

#include <Preferences.h>

#define PREFERENCES_NAMESPACE "ns"
#define PREFERENCES_KEY_COUNTDOWN_START_VALUE_MS "csvm"
#define PREFERENCES_KEY_TARGET_POMODORO_AMOUNT "tpa"

class PreferencesManager {
    Preferences preferences;

public:
    PreferencesManager() {
        preferences.begin(PREFERENCES_NAMESPACE, false);
    }

    ~PreferencesManager() {
        preferences.end();
    }

    ulong getCountdownStartValueMs(ulong defaultValue) {
        preferences.begin(PREFERENCES_NAMESPACE, true);
        ulong value = preferences.getULong(PREFERENCES_KEY_COUNTDOWN_START_VALUE_MS, defaultValue);
        preferences.end();

        return value;
    }

    void saveCountdownStartValueMs(ulong value) {
        preferences.begin(PREFERENCES_NAMESPACE, false);
        preferences.putULong(PREFERENCES_KEY_COUNTDOWN_START_VALUE_MS, value);
        preferences.end();
    }

    uint getTargetPomodoroAmount(uint defaultValue) {
        preferences.begin(PREFERENCES_NAMESPACE, true);
        ulong value = preferences.getUInt(PREFERENCES_KEY_TARGET_POMODORO_AMOUNT, defaultValue);
        preferences.end();

        return value;
    }

    void saveTargetPomodoroAmount(uint value) {
        preferences.begin(PREFERENCES_NAMESPACE, false);
        preferences.putUInt(PREFERENCES_KEY_TARGET_POMODORO_AMOUNT, value);
        preferences.end();
    }
};

#endif //ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H

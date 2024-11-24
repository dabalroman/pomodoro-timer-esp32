#ifndef ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H
#define ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H

#include <Preferences.h>

#define PREFERENCES_NAMESPACE "ns"
#define PREFERENCES_KEY_COUNTDOWN_START_VALUE_MS "csvm"
#define PREFERENCES_KEY_TARGET_POMODORO_AMOUNT "tpa"
#define PREFERENCES_KEY_WIFI_SSID "wifi_ssid"
#define PREFERENCES_KEY_WIFI_PASSWORD "wifi_pass"

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

    uint8_t getTargetPomodoroAmount(uint8_t defaultValue) {
        preferences.begin(PREFERENCES_NAMESPACE, true);
        ulong value = preferences.getUChar(PREFERENCES_KEY_TARGET_POMODORO_AMOUNT, defaultValue);
        preferences.end();

        return value;
    }

    void saveTargetPomodoroAmount(uint8_t value) {
        preferences.begin(PREFERENCES_NAMESPACE, false);
        preferences.putUChar(PREFERENCES_KEY_TARGET_POMODORO_AMOUNT, value);
        preferences.end();
    }

    // Wi-Fi SSID
    String getWiFiSSID(const String &defaultValue = "") {
        preferences.begin(PREFERENCES_NAMESPACE, true);
        String ssid = preferences.getString(PREFERENCES_KEY_WIFI_SSID, defaultValue);
        preferences.end();
        return ssid;
    }

    void saveWiFiSSID(const String &ssid) {
        preferences.begin(PREFERENCES_NAMESPACE, false);
        preferences.putString(PREFERENCES_KEY_WIFI_SSID, ssid);
        preferences.end();
    }

    // Wi-Fi Password
    String getWiFiPassword(const String &defaultValue = "") {
        preferences.begin(PREFERENCES_NAMESPACE, true);
        String password = preferences.getString(PREFERENCES_KEY_WIFI_PASSWORD, defaultValue);
        preferences.end();
        return password;
    }

    void saveWiFiPassword(const String &password) {
        preferences.begin(PREFERENCES_NAMESPACE, false);
        preferences.putString(PREFERENCES_KEY_WIFI_PASSWORD, password);
        preferences.end();
    }
};

#endif //ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H

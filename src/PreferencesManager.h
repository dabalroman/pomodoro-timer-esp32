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

    ulong countdownStartValueMs = 0;
    uint8_t targetPomodoroAmount = 0;
    String wifiSSID = "";
    String wifiPassword = "";

public:
    PreferencesManager() {
        preferences.begin(PREFERENCES_NAMESPACE, true);
        this->countdownStartValueMs = preferences.getULong(PREFERENCES_KEY_COUNTDOWN_START_VALUE_MS, 25 * 60 * 1000);
        this->targetPomodoroAmount = preferences.getUChar(PREFERENCES_KEY_TARGET_POMODORO_AMOUNT, 8);
        this->wifiSSID = preferences.getString(PREFERENCES_KEY_WIFI_SSID, "");
        this->wifiPassword = preferences.getString(PREFERENCES_KEY_WIFI_PASSWORD, "");
        preferences.end();
    }

    ulong getCountdownStartValueMs() const {
        return this->countdownStartValueMs;
    }

    void saveCountdownStartValueMs(ulong value) {
        this->countdownStartValueMs = value;
        this->preferences.begin(PREFERENCES_NAMESPACE, false);
        this->preferences.putULong(PREFERENCES_KEY_COUNTDOWN_START_VALUE_MS, value);
        this->preferences.end();
    }

    uint8_t getTargetPomodoroAmount() const {
        return this->targetPomodoroAmount;
    }

    void saveTargetPomodoroAmount(uint8_t value) {
        this->targetPomodoroAmount = value;
        this->preferences.begin(PREFERENCES_NAMESPACE, false);
        this->preferences.putUChar(PREFERENCES_KEY_TARGET_POMODORO_AMOUNT, value);
        this->preferences.end();
    }

    String getWiFiSSID() const {
        return this->wifiSSID;
    }

    void saveWiFiSSID(const String &ssid) {
        this->wifiSSID = ssid;
        this->preferences.begin(PREFERENCES_NAMESPACE, false);
        this->preferences.putString(PREFERENCES_KEY_WIFI_SSID, ssid);
        this->preferences.end();
    }

    String getWiFiPassword() const {
        return this->wifiPassword;
    }

    void saveWiFiPassword(const String &password) {
        this->wifiPassword = password;
        this->preferences.begin(PREFERENCES_NAMESPACE, false);
        this->preferences.putString(PREFERENCES_KEY_WIFI_PASSWORD, password);
        this->preferences.end();
    }
};

#endif //ESP_POMODORO_CLOCK_PREFERENCESMANAGER_H

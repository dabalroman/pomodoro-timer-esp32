#ifndef ESP_POMODORO_CLOCK_DEVICESTATE_H
#define ESP_POMODORO_CLOCK_DEVICESTATE_H

enum DeviceState {
    ready,
    settings,
    edit_timeMinutes,
    edit_timeSeconds,
    edit_pomodoroTarget,
    settings_wifiCredentials,
    settings_syncTimeViaNTP,
    counting,
    finish
};

#endif //ESP_POMODORO_CLOCK_DEVICESTATE_H

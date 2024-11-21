#ifndef ESP_POMODORO_CLOCK_DEVICESTATE_H
#define ESP_POMODORO_CLOCK_DEVICESTATE_H

enum DeviceState {
    ready,
    settings,
    editMinutes,
    editSeconds,
    counting,
    finish
};

#endif //ESP_POMODORO_CLOCK_DEVICESTATE_H

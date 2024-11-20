#ifndef ESP_POMODORO_CLOCK_LEDMANAGERSTATE_H
#define ESP_POMODORO_CLOCK_LEDMANAGERSTATE_H

enum LEDManagerState {
    ringing,
    countingDown,
    countdownPaused,
    idle,
    off,
    touchCenter,
    touchLeft,
    touchRight,
};


#endif //ESP_POMODORO_CLOCK_LEDMANAGERSTATE_H

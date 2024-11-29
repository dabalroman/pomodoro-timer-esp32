#ifndef ESP_POMODORO_CLOCK_LEDMANAGERSTATE_H
#define ESP_POMODORO_CLOCK_LEDMANAGERSTATE_H

enum LEDManagerState {
    ringing,
    countingDown,
    countdownPaused,
    idle,
    off,
    touchSelect,
    touchLeft,
    touchRight,
    longTouchSelect,
    longTouchLeft,
    longTouchRight,
};


#endif //ESP_POMODORO_CLOCK_LEDMANAGERSTATE_H

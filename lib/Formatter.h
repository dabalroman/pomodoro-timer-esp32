#include <Arduino.h>

#ifndef ESP_POMODORO_CLOCK_FORMATTERS_H
#define ESP_POMODORO_CLOCK_FORMATTERS_H

class Formatter {
public:
    static String formatNumber(ulong number) {
        return number < 10 ? '0' + String(number) : String(number);
    }

    static ulong getSecondsFromTime(ulong timeInTicksMs) {
        return (timeInTicksMs / 1000) % 60;
    }

    static ulong getMinutesFromTime(ulong timeInTicksMs) {
        return timeInTicksMs / 60000;
    }

    static String formatTime(ulong timeInTicksMs) {
        ulong seconds = getSecondsFromTime(timeInTicksMs);
        ulong minutes = getMinutesFromTime(timeInTicksMs);

        return formatNumber(minutes) + ':' + formatNumber(seconds);
    }
};

#endif //ESP_POMODORO_CLOCK_FORMATTERS_H

#ifndef ESP_POMODORO_CLOCK_FORMATTERS_H
#define ESP_POMODORO_CLOCK_FORMATTERS_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

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

    static void printCenteredText(Adafruit_SSD1306 &display, const char* text, short x, short y) {
        int16_t x1, y1;
        uint16_t textWidth, textHeight;

        display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);

        int16_t xPos = x - (textWidth / 2);

        display.setCursor(xPos, y);
        display.print(text);
    }
};

#endif //ESP_POMODORO_CLOCK_FORMATTERS_H

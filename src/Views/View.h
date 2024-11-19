#ifndef ESP_POMODORO_CLOCK_VIEW_H
#define ESP_POMODORO_CLOCK_VIEW_H

#include <Adafruit_SSD1306.h>
#include "TouchManager.h"
#include "LEDManager/LEDManager.h"
#include "../lib/Formatter.h"

class View {
protected:
    Adafruit_SSD1306 &display;
    TouchManager &touch;
    LEDManager &ledManager;

public:
    View(Adafruit_SSD1306 &display, TouchManager &touch, LEDManager &ledManager)
            : display(display), touch(touch), ledManager(ledManager) {}

    virtual void handleInput() = 0; // To be implemented by derived classes
    virtual void render();

    virtual ~View() = default;
};


#endif //ESP_POMODORO_CLOCK_VIEW_H

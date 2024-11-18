#ifndef ESP_POMODORO_CLOCK_VIEW_H
#define ESP_POMODORO_CLOCK_VIEW_H

#include <Adafruit_SSD1306.h>
#include "TouchManager.h"
#include "../lib/Formatter.h"

class View {
protected:
    Adafruit_SSD1306 &display;
    TouchManager &touch;

public:
    View(Adafruit_SSD1306 &display, TouchManager &touch)
            : display(display), touch(touch) {}

    virtual void handleInput() = 0; // To be implemented by derived classes
    virtual void render();

    virtual ~View() = default;
};


#endif //ESP_POMODORO_CLOCK_VIEW_H

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
    ulong &lastTickMs;

    ulong lastRenderedOnTickMs = 0;

public:
    View(
            Adafruit_SSD1306 &display,
            TouchManager &touch,
            LEDManager &ledManager,
            ulong &lastTickMs
    ) : display(display),
        touch(touch),
        ledManager(ledManager),
        lastTickMs(lastTickMs) {}

    virtual void handleInput() = 0;
    virtual void render();

    virtual bool shouldRender() const;

    virtual ~View() = default;
};


#endif //ESP_POMODORO_CLOCK_VIEW_H

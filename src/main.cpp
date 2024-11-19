#include "../lib/Formatter.h"
#include "DeviceState.h"
#include "PreferencesManager.h"
#include "TouchManager.h"
#include "Views/EditView.h"
#include "Views/FinishView.h"
#include "Views/MainView.h"
#include "Views/TimerView.h"
#include "LEDManager/LEDManager.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Wire.h>

#define TOUCH_THRESHOLD 1500

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

ulong lastTickMs = 0;
ulong countdownStartTickMs = 0;
ulong countdownStartValueMs = 25 * 60 * 1000;
ulong lastScreenUpdate = 0;

TouchManager touch;
PreferencesManager preferencesManager;
DeviceState deviceState= ready;
LEDManager ledManager(lastTickMs);

EditView editView(display, touch, ledManager, preferencesManager, countdownStartValueMs, deviceState, lastTickMs);
MainView mainView(display, touch, ledManager, countdownStartValueMs, countdownStartTickMs, deviceState, lastTickMs);
TimerView timerView(display, touch, ledManager, countdownStartValueMs, countdownStartTickMs, deviceState, lastTickMs);
FinishView finishView(display, touch, ledManager, deviceState, lastTickMs);

void triggerTouchLeft() {
    touch.leftButton.trigger();
}

void triggerTouchSelect() {
    touch.selectButton.trigger();
}

void triggerTouchRight() {
    touch.rightButton.trigger();
}

void setup() {
    Serial.begin(115200);

    ledManager.setup();

    // Setup display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    delay(2000);
    display.clearDisplay();
    display.setFont(&FreeMonoBold18pt7b);
    display.setTextColor(WHITE);


    // Setup touch
    touchAttachInterrupt(touch.leftButton.getGPIO(), triggerTouchLeft, TOUCH_THRESHOLD);
    touchAttachInterrupt(touch.selectButton.getGPIO(), triggerTouchSelect, TOUCH_THRESHOLD);
    touchAttachInterrupt(touch.rightButton.getGPIO(), triggerTouchRight, TOUCH_THRESHOLD);


    // Setup environment
    lastTickMs = millis();
    countdownStartTickMs = lastTickMs;
    countdownStartValueMs = preferencesManager.getCountdownStartValueMs(25 * 60 * 1000);
}

View *getCurrentView() {
    switch (deviceState) {
        case DeviceState::editMinutes:
        case DeviceState::editSeconds:
            return &editView;

        case DeviceState::ready:
            return &mainView;

        case DeviceState::counting:
            return &timerView;

        case DeviceState::finish:
            return &finishView;
    }

    return &mainView;
}

void touchDebugOverlay() {
    display.setFont();
    display.setCursor(0, 56);
    display.print(touch.getTouchDebugValue());
}

void loop() {
    lastTickMs = millis();

    touch.update();

    // Touch feedback is the most important one, so let it override other values before render
    if(touch.leftButton.isTouched()) {
        ledManager.setState(LEDManagerState::touchLeft);
    } else if(touch.selectButton.isTouched()) {
        ledManager.setState(LEDManagerState::touchCenter);
    } else if(touch.rightButton.isTouched()) {
        ledManager.setState(LEDManagerState::touchRight);
    }

    ledManager.update();

    // Handle input with higher polling to get more tactile UX
    View *currentView = getCurrentView();
    currentView->handleInput();

    if(lastTickMs - lastScreenUpdate >= 100) {
        lastScreenUpdate = lastTickMs;

        currentView->render();
        display.display();
    }

    delay(20);
}

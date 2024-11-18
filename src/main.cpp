#include "../lib/Formatter.h"
#include "DeviceState.h"
#include "PreferencesManager.h"
#include "TouchManager.h"
#include "Views/EditView.h"
#include "Views/FinishView.h"
#include "Views/MainView.h"
#include "Views/TimerView.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <FastLED.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Wire.h>

#define TOUCH_THRESHOLD 1500

#define LED_GPIO 12
#define LED_TYPE WS2811
#define LED_COLOR_ORDER GRB
#define LED_AMOUNT 2
#define LED_BRIGHTNESS 32

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
CRGB ledArray[2];

TouchManager touch;
PreferencesManager preferencesManager;

state deviceState = ready;
ulong lastTickMs = 0;
ulong countdownStartTickMs = 0;
ulong countdownStartValueMs = 25 * 60 * 1000;

EditView editView(display, touch, preferencesManager, countdownStartValueMs, deviceState, lastTickMs);
MainView mainView(display, touch, countdownStartValueMs, countdownStartTickMs, deviceState, lastTickMs);
TimerView timerView(display, touch, countdownStartValueMs, countdownStartTickMs, deviceState, lastTickMs);
FinishView finishView(display, touch, deviceState, lastTickMs);

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

    // Setup leds
    FastLED.addLeds<LED_TYPE, LED_GPIO, LED_COLOR_ORDER>(ledArray, LED_AMOUNT)
            .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_BRIGHTNESS);
    ledArray[0].setRGB(random(255), random(255), random(255));
    ledArray[1].setRGB(random(255), random(255), random(255));
    FastLED.show();

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
        case state::editMinutes:
        case state::editSeconds:
            return &editView;

        case state::ready:
            return &mainView;

        case state::counting:
            return &timerView;

        case state::finish:
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

    touch.updateState();

    View *currentView = getCurrentView();
    currentView->handleInput();
    currentView->render();

    touchDebugOverlay();

    display.display();

    delay(100);
}

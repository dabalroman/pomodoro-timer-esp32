#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include "TouchManager.h"
#include "../lib/Formatter.h"
#include "DeviceState.h"
#include "Views/EditView.h"
#include "Views/MainView.h"
#include "Views/TimerView.h"
#include "Views/FinishView.h"

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

state deviceState = ready;
ulong lastTickMs = 0;
ulong startupTickMs = 0;
ulong countdownStartTickMs = 0;
ulong countdownStartValueMs = 25 * 60 * 1000;

EditView editView(display, touch, countdownStartValueMs, deviceState, lastTickMs);
MainView mainView(display, touch, countdownStartValueMs, countdownStartTickMs, deviceState, lastTickMs);
TimerView timerView(display, touch, countdownStartValueMs, countdownStartTickMs, deviceState, lastTickMs);
FinishView finishView(display, touch, deviceState, lastTickMs);

void triggerTouchLeft() {
    touch.left.trigger();
}

void triggerTouchSelect() {
    touch.select.trigger();
}

void triggerTouchRight() {
    touch.right.trigger();
}

void setup() {
    Serial.begin(115200);

    FastLED.addLeds<LED_TYPE, LED_GPIO, LED_COLOR_ORDER>(ledArray, LED_AMOUNT)
            .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_BRIGHTNESS);
    ledArray[0].setRGB(random(255), random(255), random(255));
    ledArray[1].setRGB(random(255), random(255), random(255));
    FastLED.show();

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    delay(2000);
    display.clearDisplay();

    display.setFont(&FreeMonoBold18pt7b);
    display.setTextColor(WHITE);

    touchAttachInterrupt(touch.left.getGPIO(), triggerTouchLeft, TOUCH_THRESHOLD);
    touchAttachInterrupt(touch.select.getGPIO(), triggerTouchSelect, TOUCH_THRESHOLD);
    touchAttachInterrupt(touch.right.getGPIO(), triggerTouchRight, TOUCH_THRESHOLD);

    lastTickMs = millis();
    startupTickMs = lastTickMs;
    countdownStartTickMs = lastTickMs;
}

View* getCurrentView() {
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
}

void touchDebugOverlay() {
    display.setFont();
    display.setCursor(0, 56);
    display.print(touch.getTouchDebugValue());
}

void loop() {
    lastTickMs = millis();

    touch.updateState();

    View* currentView = getCurrentView();
    if (currentView) {
        currentView->handleInput();
        currentView->render();
    }

    touchDebugOverlay();

    display.display();

    delay(100);
}

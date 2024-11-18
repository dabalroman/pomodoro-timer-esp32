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

void countingView() {
    //Handle input
    if (touch.select.takeActionIfPossible()) {
        deviceState = state::ready;
    }

    //Handle view
    ulong countdownValue = countdownStartTickMs + countdownStartValueMs - lastTickMs;

    // React to overflow
    if (countdownValue >= 4000000000) {
        countdownValue = 0;
        deviceState = state::finish;
    }

    String text = Formatter::formatTime(countdownValue);

    display.setFont();
    display.setCursor(28, 9);
    display.print("Get to work!");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
}

void finishView() {
    //Handle input
    if (touch.select.takeActionIfPossible()) {
        deviceState = state::ready;
    }

    //Handle view
    bool showDigits = (lastTickMs / 500) % 2 == 0;
    String text = "00:00";

    display.setFont();
    display.setCursor(0, 9);
    display.print("Time to take a break!");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);

    if (showDigits) {
        display.print(text);
    }
}

void loop() {
    lastTickMs = millis();

    touch.updateState();

    display.clearDisplay();

    switch (deviceState) {
        case state::editSeconds:
        case state::editMinutes:
            editView.handleInput();
            editView.render();
            break;

        case state::ready:
            mainView.handleInput();
            mainView.render();
            break;

        case state::counting:
            countingView();
            break;

        case state::finish:
            finishView();
            break;
    }

    display.setFont();
    display.setCursor(0, 56);
    display.print(touch.getTouchDebugValue());

    display.display();

    delay(100);
}

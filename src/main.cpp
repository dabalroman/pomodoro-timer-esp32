#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include "TouchManager.h"

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

enum state {
    ready,
    editMinutes,
    editSeconds,
    counting,
    finish
};

TouchManager touch;

state deviceState = ready;
ulong lastTickMs = 0;
ulong startupTickMs = 0;
ulong countdownStartTickMs = 0;
ulong countdownStartValueMs = 25 * 60 * 1000;

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
    ledArray[0].setRGB(0, 255, 0);
    ledArray[1].setRGB(255, 0, 0);
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

String formatNumber(ulong number) {
    return number < 10 ? '0' + String(number) : String(number);
}

ulong getSecondsFromTime(ulong timeInTicksMs) {
    return (timeInTicksMs / 1000) % 60;
}

ulong getMinutesFromTime(ulong timeInTicksMs) {
    return timeInTicksMs / 60000;
}

String formatTime(ulong timeInTicksMs) {
    ulong seconds = getSecondsFromTime(timeInTicksMs);
    ulong minutes = getMinutesFromTime(timeInTicksMs);

    return formatNumber(minutes) + ':' + formatNumber(seconds);
}

void editView() {
    //Handle input
    if (touch.select.takeActionIfPossible()) {
        if (deviceState == state::editMinutes) {
            deviceState = state::editSeconds;
        } else if (deviceState == state::editSeconds) {
            deviceState = state::ready;
        }
    }

    if (touch.right.isTouched()) {
        if (deviceState == state::editMinutes) {
            countdownStartValueMs += 60 * 1000;
        } else if (deviceState == state::editSeconds) {
            countdownStartValueMs += 1000;
        }

        // Max value is 99:59
        if (countdownStartValueMs >= (100 * 60 - 1) * 1000) {
            countdownStartValueMs = (100 * 60 - 1) * 1000;
        }
    }

    if (touch.left.isTouched()) {
        if (deviceState == state::editMinutes) {
            countdownStartValueMs -= 60 * 1000;
        } else if (deviceState == state::editSeconds) {
            countdownStartValueMs -= 1000;
        }

        // React to overflow
        if (countdownStartValueMs >= 4000000000) {
            countdownStartValueMs = 0;
        }
    }

    //Handle view
    bool showDigit = (lastTickMs / 300) % 2 == 0;
    String text = "";

    if (deviceState == editMinutes) {
        if (showDigit) {
            text += formatNumber(getMinutesFromTime(countdownStartValueMs));
        } else {
            text += "  ";
        }

        text += ":" + formatNumber(getSecondsFromTime(countdownStartValueMs));
    } else if (deviceState == editSeconds) {
        text += formatNumber(getMinutesFromTime(countdownStartValueMs)) + ":";

        if (showDigit) {
            text += formatNumber(getSecondsFromTime(countdownStartValueMs));
        } else {
            text += "  ";
        }
    }

    display.setFont();
    display.setCursor(37, 9);
    display.print("Edit time");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
}

void readyView() {
    //Handle input
    if (touch.right.takeActionIfPossible()) {
        deviceState = state::editMinutes;
    }

    if (touch.select.takeActionIfPossible()) {
        deviceState = state::counting;
        countdownStartTickMs = lastTickMs;
    }

    //Handle view
    String text = formatTime(countdownStartValueMs);

    display.setFont();
    display.setCursor(49, 9);
    display.print("Ready");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
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

    String text = formatTime(countdownValue);

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
            editView();
            break;

        case state::ready:
            readyView();
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

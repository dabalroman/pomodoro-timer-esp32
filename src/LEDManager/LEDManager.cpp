#include "LEDManager.h"
#include "FastLED.h"

#define LED_GPIO 12
#define LED_TYPE WS2811
#define LED_COLOR_ORDER GRB
#define LED_AMOUNT 2
#define LED_BRIGHTNESS 32

void LEDManager::setup() {
    FastLED.addLeds<LED_TYPE, LED_GPIO, LED_COLOR_ORDER>(ledArray, LED_AMOUNT)
            .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_BRIGHTNESS);

    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setRGB(0, 0, 0);

    FastLED.show();
}

bool LEDManager::shouldUpdate() {
    return this->currentTickMs - this->lastUpdateMs <= LED_UPDATE_EVERY_MS;
}

void LEDManager::display(uint keepFrameForMs = 0) {
    this->lastUpdateMs = this->currentTickMs + keepFrameForMs;
    FastLED.show();
}

void LEDManager::off() {
    if (!this->shouldUpdate()) {
        return;
    }

    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setRGB(0, 0, 0);

    this->display();
}

void LEDManager::showTouch(bool left, bool center, bool right) {
    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setRGB(0, 0, 0);

    if (left || center) {
        ledArray[0].setRGB(255, 0, 255);
    }

    if (right || center) {
        ledArray[1].setRGB(255, 0, 255);
    }

    this->display(100);
}

void LEDManager::animateAlarm() {
    if (!this->shouldUpdate()) {
        return;
    }

    if ((currentTickMs / 500) % 2) {
        ledArray[0].setRGB(255, 0, 0);
        ledArray[1].setRGB(0, 0, 255);
    } else {
        ledArray[0].setRGB(0, 0, 255);
        ledArray[1].setRGB(255, 0, 0);
    }

    this->display();
}

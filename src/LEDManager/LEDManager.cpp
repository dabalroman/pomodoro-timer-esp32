#include "LEDManager.h"
#include "FastLED.h"

void LEDManager::setup() {
    FastLED.addLeds<LED_TYPE, LED_GPIO, LED_COLOR_ORDER>(ledArray, LED_AMOUNT)
            .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_BRIGHTNESS);

    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setRGB(0, 0, 0);

    FastLED.show();
}

void LEDManager::update() {
    if (!shouldUpdate()) {
        return;
    }

    switch (this->state) {
        case LEDManagerState::ringing:
            this->renderRinging();
            break;

        case LEDManagerState::touchLeft:
            this->renderTouchLeft();
            break;

        case LEDManagerState::touchCenter:
            this->renderTouchCenter();
            break;

        case LEDManagerState::touchRight:
            this->renderTouchRight();
            break;

        case LEDManagerState::off:
        default:
            this->renderOff();
            break;
    }

    this->lastUpdateMs = this->currentTickMs;
    FastLED.show();
}

bool LEDManager::shouldUpdate() const {
    return (this->currentTickMs - this->lastUpdateMs) >= LED_UPDATE_EVERY_MS;
}

void LEDManager::setState(LEDManagerState newState) {
    this->state = newState;
}

void LEDManager::renderOff() {
    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setRGB(0, 0, 0);
}

void LEDManager::renderTouchLeft() {
    ledArray[0].setRGB(0, 255, 0);
    ledArray[1].setRGB(0, 0, 0);
}

void LEDManager::renderTouchCenter() {
    ledArray[0].setRGB(0, 127, 127);
    ledArray[1].setRGB(0, 127, 127);
}


void LEDManager::renderTouchRight() {
    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setRGB(0, 0, 255);
}

void LEDManager::renderRinging() {
    if ((currentTickMs / 500) % 2) {
        ledArray[0].setRGB(255, 0, 0);
        ledArray[1].setRGB(0, 0, 255);
    } else {
        ledArray[0].setRGB(0, 0, 255);
        ledArray[1].setRGB(255, 0, 0);
    }
}

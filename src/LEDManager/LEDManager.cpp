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

        case LEDManagerState::longTouchLeft:
            this->renderTouchLeft(true);
            break;

        case LEDManagerState::touchSelect:
            this->renderTouchSelect();
            break;

        case LEDManagerState::longTouchSelect:
            this->renderTouchSelect(true);
            break;

        case LEDManagerState::touchRight:
            this->renderTouchRight();
            break;

        case LEDManagerState::longTouchRight:
            this->renderTouchRight(true);
            break;

        case LEDManagerState::countingDown:
            this->renderCountingDown();
            break;

        case LEDManagerState::countdownPaused:
            this->renderCountdownPaused();
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

void LEDManager::renderTouchLeft(bool isLongTouch) {
    ledArray[0].setHSV(85 + isLongTouch * 15, 255, 255);
    ledArray[1].setRGB(0, 0, 0);
}

void LEDManager::renderTouchSelect(bool isLongTouch) {
    ledArray[0].setHSV(128 + isLongTouch * 15, 255, 127);
    ledArray[1].setHSV(128 + isLongTouch * 15, 255, 127);
}

void LEDManager::renderTouchRight(bool isLongTouch) {
    ledArray[0].setRGB(0, 0, 0);
    ledArray[1].setHSV(170 + isLongTouch * 15, 255, 255);
}

void LEDManager::renderRinging() {
    const ulong cycleDurationMs = 1000;
    ulong cyclePosition = this->currentTickMs % cycleDurationMs;

    float normalizedPosition = static_cast<float>(cyclePosition) / static_cast<float>(cycleDurationMs);

    // Overshoot for longer peaks
    float intensity = 0.5f + 0.6f * sin(2 * 3.1415f * normalizedPosition);
    intensity = intensity < 0.0f ? 0.0f : (intensity > 1.0f ? 1.0f : intensity);
    intensity *= 255;

    uint8_t hue = (this->currentTickMs / 100) % 256;
    CRGB color = CHSV(hue, 255, static_cast<uint8_t>(intensity));
    ledArray[0].setRGB(color.r, color.g, color.b);

    hue = (this->currentTickMs / 100 + 128) % 256;
    color = CHSV(hue, 255, static_cast<uint8_t>(255 - intensity));
    ledArray[1].setRGB(color.r, color.g, color.b);
}

void LEDManager::renderCountingDown() {
    const ulong cycleDurationMs = 10000;
    ulong cyclePosition = this->currentTickMs % cycleDurationMs;

    float normalizedPosition = static_cast<float>(cyclePosition) / static_cast<float>(cycleDurationMs);

    // Overshoot for longer peaks
    float intensity = 0.5f + 0.6f * sin(2 * 3.1415f * normalizedPosition);
    intensity = intensity < 0.0f ? 0.0f : (intensity > 1.0f ? 1.0f : intensity);
    intensity *= 127;

    ledArray[0].setRGB(0, 0, static_cast<uint8_t>(intensity));
    ledArray[1].setRGB(0, 0, static_cast<uint8_t>(127 - intensity));
}

void LEDManager::renderCountdownPaused() {
    const ulong cycleDurationMs = 4000;
    ulong cyclePosition = this->currentTickMs % cycleDurationMs;

    float normalizedPosition = static_cast<float>(cyclePosition) / static_cast<float>(cycleDurationMs);

    // Overshoot for longer peaks
    float intensity = 0.5f + 0.6f * sin(2 * 3.1415f * normalizedPosition);
    intensity = intensity < 0.0f ? 0.0f : (intensity > 1.0f ? 1.0f : intensity);
    intensity *= 127;

    ledArray[0].setRGB(static_cast<uint8_t>(intensity), 0, 0);
    ledArray[1].setRGB(static_cast<uint8_t>(intensity), 0, 0);
}


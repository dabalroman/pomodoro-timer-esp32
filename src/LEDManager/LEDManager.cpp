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

        case LEDManagerState::countingDown:
            this->renderCountingDown();
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
    const ulong cycleDurationMs = 1000;
    ulong cyclePosition = this->currentTickMs % cycleDurationMs;

    float normalizedPosition = static_cast<float>(cyclePosition) / static_cast<float>(cycleDurationMs);

    // Overshoot for longer peaks
    float intensity = 0.5f + 0.6f * sin(2 * 3.1415f * normalizedPosition);
    intensity = intensity < 0.0f ? 0.0f : (intensity > 1.0f ? 1.0f : intensity);
    intensity *= 255;

    ledArray[0].setRGB(static_cast<uint8_t>(intensity), 0, static_cast<uint8_t>(255 - intensity));
    ledArray[1].setRGB(static_cast<uint8_t>(255 - intensity), 0, static_cast<uint8_t>(intensity));
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

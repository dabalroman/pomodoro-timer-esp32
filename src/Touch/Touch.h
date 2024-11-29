#include <Arduino.h>

#ifndef ESP_POMODORO_CLOCK_TOUCH_H
#define ESP_POMODORO_CLOCK_TOUCH_H

#define LONG_TOUCH_MS 1000

class Touch {
private:
    uint8_t gpio;
    ulong touchedAt = 0;
    ulong canTouchAt = 0;
    bool touched = false;
    bool isActionTaken = false;

    void takeAction() {
        isActionTaken = true;
        this->canTouchAt = millis() + 50;
    }

public:
    explicit Touch(uint8_t gpio) {
        this->gpio = gpio;
    }

    void trigger() {
        this->touched = true;
        this->isActionTaken = false;
        this->touchedAt = millis();
    }

    void preventAccidentalActionFor(ulong delay = 500) {
        this->canTouchAt = millis() + delay;
    }

    uint8_t getGPIO() const {
        return gpio;
    }

    bool takeActionIfPossible() {
        if (touched && !isActionTaken && canTouchAt <= millis()) {
            this->takeAction();
            return true;
        }

        return false;
    }

    bool takeActionIfPossibleLongTouch() {
        if (touched && isLongTouch() && !isActionTaken && canTouchAt <= millis()) {
            this->takeAction();
            return true;
        }

        return false;
    }

    bool isTouched() const {
        return touched;
    }

    bool isLongTouch() const {
        return touchedAt + LONG_TOUCH_MS <= millis();
    }

    void updateState() {
        if (this->touched && !touchInterruptGetLastStatus(this->gpio)) {
            this->touched = false;
            this->isActionTaken = false;
        }
    }
};

#endif //ESP_POMODORO_CLOCK_TOUCH_H

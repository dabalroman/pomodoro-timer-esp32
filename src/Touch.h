#include <Arduino.h>

#ifndef ESP_POMODORO_CLOCK_TOUCH_H
#define ESP_POMODORO_CLOCK_TOUCH_H

#define LONG_TOUCH_MS 1000

class Touch {
private:
    uint8_t gpio;
    ulong touchedAt = 0;
    bool touched = false;
    bool isActionTaken = false;

public:
    explicit Touch(uint8_t gpio) {
        this->gpio = gpio;
    }

    void trigger() {
        this->touched = true;
        this->isActionTaken = false;
        this->touchedAt = millis();
    }

    uint8_t getGPIO() const {
        return gpio;
    }

    bool canTakeAction() const {
        return !isActionTaken;
    }

    bool takeActionIfPossible() {
        if(touched && !isActionTaken) {
            isActionTaken = true;
            return true;
        }

        return false;
    }

    void takeAction() {
        isActionTaken = true;
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

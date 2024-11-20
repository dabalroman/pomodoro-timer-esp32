#include "TimerView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

ulong TimerView::getTimeLeftMs() const {
    return this->countdownStartTickMs + this->countdownStartValueMs - this->lastTickMs;
}

void TimerView::handleInput() {
    if (touch.selectButton.takeActionIfPossible()) {
        this->isPaused = !this->isPaused;

        if (this->isPaused) {
            this->countdownPausedValueMs = this->getTimeLeftMs();
        } else {
            this->countdownStartTickMs = this->lastTickMs;
            this->countdownStartValueMs = this->countdownPausedValueMs;
        }
    }

    if (touch.leftButton.takeActionIfPossible()) {
        this->deviceState = DeviceState::ready;
    }
}

void TimerView::render() {
    ledManager.setState(this->isPaused ? countdownPaused : countingDown);

    View::render();

    ulong countdownValue = this->isPaused ? this->countdownPausedValueMs : this->getTimeLeftMs();

    display.setFont();
    if (this->isPaused) {
        display.setCursor(44, 9);
        display.print("Paused");
    } else {
        display.setCursor(28, 9);
        display.print("Get to work!");
    }

    String text = Formatter::formatTime(countdownValue);
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
}

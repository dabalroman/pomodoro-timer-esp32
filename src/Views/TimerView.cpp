#include "TimerView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

ulong TimerView::getTimeLeftMs() const {
    return this->countdownStartTickMs + this->countdownStartValueMs - this->lastTickMs;
}

void TimerView::handleInput() {
    if (touch.selectButton.takeActionIfPossible()) {
        this->isPaused = !this->isPaused;

        // Remember original countdown value, so pause feature does not affect future timers
        if (this->countdownStartValueMsBackup == 0) {
            this->countdownStartValueMsBackup = countdownStartValueMs;
        }

        if (this->isPaused) {
            this->countdownPausedValueMs = this->getTimeLeftMs();
        } else {
            this->countdownStartTickMs = this->lastTickMs;
            this->countdownStartValueMs = this->countdownPausedValueMs;
        }
    }

    if (touch.leftButton.takeActionIfPossibleLongTouch()) {
        this->deviceState = DeviceState::ready;
        this->onExit();
    }
}

void TimerView::onExit() {
    // Restore original countdown value
    if (this->countdownStartValueMsBackup != 0) {
        this->countdownStartValueMs = this->countdownStartValueMsBackup;
    }

    this->isPaused = false;
    this->prevIsPaused = false;
    this->countdownPausedValueMs = 0;
    this->countdownStartValueMsBackup = 0;
}

void TimerView::render() {
    ledManager.setState(this->isPaused ? countdownPaused : countingDown);

    if (!this->shouldRender()) {
        return;
    }

    View::render();

    ulong currentCountdownValueMs = this->isPaused ? this->countdownPausedValueMs : this->getTimeLeftMs();

    // Overflow; Timer has finished
    if (currentCountdownValueMs > 4000000000) {
        this->deviceState = DeviceState::finish;
        this->onExit();
        currentCountdownValueMs = 0;
    }

    display.setFont();
    if (this->isPaused) {
        display.setCursor(44, 9);
        display.print("Paused");
    } else {
        display.setCursor(28, 9);
        display.print("Get to work!");
    }

    String text = Formatter::formatTime(currentCountdownValueMs);
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);

    this->prevIsPaused = isPaused;
}

bool TimerView::shouldRender() const {
    // 1fps is enough for this view; update on pause state change
    return this->lastTickMs - this->lastRenderedOnTickMs >= 1000
           || this->prevIsPaused != this->isPaused;
}

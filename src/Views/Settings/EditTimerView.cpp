#include "EditTimerView.h"
#include "Fonts/FreeMonoBold18pt7b.h"

void EditTimerView::handleInput() {
    if (!this->hasEditBeenRendered) {
        return;
    }

    if (touch.selectButton.takeActionIfPossible()) {
        if (this->deviceState == DeviceState::edit_timeMinutes) {
            this->deviceState = DeviceState::edit_timeSeconds;
            this->touch.preventAccidentalActionFor();
        } else if (this->deviceState == DeviceState::edit_timeSeconds) {
            this->preferencesManager.saveCountdownStartValueMs(this->countdownStartValueMs);
            this->deviceState = DeviceState::settings;
            this->touch.preventAccidentalActionFor();
        }
    }

    if (touch.rightButton.isTouched()) {
        if (this->deviceState == DeviceState::edit_timeMinutes) {
            this->countdownStartValueMs += 60 * 1000;
        } else if (this->deviceState == DeviceState::edit_timeSeconds) {
            this->countdownStartValueMs += 10000;
        }
    }

    if (touch.leftButton.isTouched()) {
        if (this->deviceState == DeviceState::edit_timeMinutes) {
            this->countdownStartValueMs -= 60 * 1000;
        } else if (this->deviceState == DeviceState::edit_timeSeconds) {
            this->countdownStartValueMs -= 10000;
        }
    }

    if (this->countdownStartValueMs < 10000 || this->countdownStartValueMs > 4000000000) {
        this->countdownStartValueMs = 10000;
    }

    if (this->countdownStartValueMs > (99 * 60) * 1000) {
        this->countdownStartValueMs = (99 * 60) * 1000;
    }

    this->hasEditBeenRendered = false;
}

void EditTimerView::render() {
    this->ledManager.setState(LEDManagerState::idle);

    if (!this->shouldRender()) {
        return;
    }

    View::render();

    bool showDigit = (lastTickMs / 300) % 4 > 0;
    String text = "";

    if (this->deviceState == edit_timeMinutes) {
        text += showDigit ? Formatter::formatNumber(Formatter::getMinutesFromTime(this->countdownStartValueMs)) : "  ";
        text += ":" + Formatter::formatNumber(Formatter::getSecondsFromTime(this->countdownStartValueMs));
    } else if (this->deviceState == edit_timeSeconds) {
        text += Formatter::formatNumber(Formatter::getMinutesFromTime(this->countdownStartValueMs)) + ":";
        text += showDigit ? Formatter::formatNumber(Formatter::getSecondsFromTime(this->countdownStartValueMs)) : "  ";
    }

    this->display.setFont();
    this->display.setCursor(37, 9);
    this->display.print("Edit time");

    this->display.setFont(&FreeMonoBold18pt7b);
    this->display.setCursor(12, 44);
    this->display.print(text);

    this->hasEditBeenRendered = true;
}

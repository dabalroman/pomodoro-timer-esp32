#include "EditView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void EditView::handleInput() {
    if (!hasEditBeenRendered) {
        return;
    }

    if (touch.selectButton.takeActionIfPossible()) {
        if (deviceState == DeviceState::editMinutes) {
            deviceState = DeviceState::editSeconds;
        } else if (deviceState == DeviceState::editSeconds) {
            preferencesManager.saveCountdownStartValueMs(countdownStartValueMs);
            deviceState = DeviceState::ready;
        }
    }

    if (touch.rightButton.isTouched()) {
        if (deviceState == DeviceState::editMinutes) {
            countdownStartValueMs += 60 * 1000;
        } else if (deviceState == DeviceState::editSeconds) {
            countdownStartValueMs += 10000;
        }

        // Max value is 99:59
        if (countdownStartValueMs >= (100 * 60 - 1) * 1000) {
            countdownStartValueMs = (100 * 60 - 1) * 1000;
        }
    }

    if (touch.leftButton.isTouched()) {
        if (deviceState == DeviceState::editMinutes) {
            countdownStartValueMs -= 60 * 1000;
        } else if (deviceState == DeviceState::editSeconds) {
            countdownStartValueMs -= 10000;
        }
    }

    this->hasEditBeenRendered = false;
}

void EditView::render() {
    ledManager.setState(LEDManagerState::idle);

    View::render();

    bool showDigit = (lastTickMs / 300) % 2 == 0;
    String text = "";

    if (deviceState == editMinutes) {
        text += showDigit ? Formatter::formatNumber(Formatter::getMinutesFromTime(countdownStartValueMs)) : "  ";
        text += ":" + Formatter::formatNumber(Formatter::getSecondsFromTime(countdownStartValueMs));
    } else if (deviceState == editSeconds) {
        text += Formatter::formatNumber(Formatter::getMinutesFromTime(countdownStartValueMs)) + ":";
        text += showDigit ? Formatter::formatNumber(Formatter::getSecondsFromTime(countdownStartValueMs)) : "  ";
    }

    display.setFont();
    display.setCursor(37, 9);
    display.print("Edit time");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);

    this->hasEditBeenRendered = true;
}

#include "EditView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void EditView::handleInput() {
    if (touch.select.takeActionIfPossible()) {
        if (deviceState == state::editMinutes) {
            deviceState = state::editSeconds;
        } else if (deviceState == state::editSeconds) {
            deviceState = state::ready;
        }
    }

    if (touch.right.isTouched()) {
        if (deviceState == state::editMinutes) {
            countdownStartValueMs += 60 * 1000;
        } else if (deviceState == state::editSeconds) {
            countdownStartValueMs += 1000;
        }

        // Max value is 99:59
        if (countdownStartValueMs >= (100 * 60 - 1) * 1000) {
            countdownStartValueMs = (100 * 60 - 1) * 1000;
        }
    }

    if (touch.left.isTouched()) {
        if (deviceState == state::editMinutes) {
            countdownStartValueMs -= 60 * 1000;
        } else if (deviceState == state::editSeconds) {
            countdownStartValueMs -= 1000;
        }

        // React to overflow
        if (countdownStartValueMs >= 4000000000) { // Wrap-around overflow check
            countdownStartValueMs = 0;
        }
    }
}

void EditView::render() {
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
}

#include "MainView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void MainView::handleInput() {
    if (touch.right.takeActionIfPossible()) {
        deviceState = state::editMinutes;
    }

    if (touch.select.takeActionIfPossible()) {
        deviceState = state::counting;
        countdownStartTickMs = lastTickMs;
    }
}

void MainView::render() {
    String text = Formatter::formatTime(countdownStartValueMs);

    display.setFont();
    display.setCursor(49, 9);
    display.print("Ready");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
}

#include "TimerView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void TimerView::handleInput() {
    if (touch.selectButton.takeActionIfPossible()) {
        deviceState = state::ready;
    }
}

void TimerView::render() {
    View::render();

    ulong countdownValue = countdownStartTickMs + countdownStartValueMs - lastTickMs;

    // React to overflow
    if (countdownValue >= 4000000000) {
        countdownValue = 0;
        deviceState = state::finish;
    }

    String text = Formatter::formatTime(countdownValue);

    display.setFont();
    display.setCursor(28, 9);
    display.print("Get to work!");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
}

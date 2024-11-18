#include "FinishView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void FinishView::handleInput() {
    if (touch.selectButton.takeActionIfPossible()) {
        deviceState = state::ready;
    }
}

void FinishView::render() {
    View::render();

    bool showDigits = (lastTickMs / 500) % 2 == 0;
    String text = "00:00";

    display.setFont();
    display.setCursor(0, 9);
    display.print("Time to take a break!");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);

    if (showDigits) {
        display.print(text);
    }
}

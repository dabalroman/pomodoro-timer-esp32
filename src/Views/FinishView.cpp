#include "FinishView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void FinishView::handleInput() {
    if (touch.selectButton.takeActionIfPossible()) {
        deviceState = DeviceState::ready;
    }
}

void FinishView::render() {
    ledManager.setState(LEDManagerState::ringing);

    if (!this->shouldRender()) {
        return;
    }

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

bool FinishView::shouldRender() const {
    // 2fps is enough for this view
    return this->lastTickMs - this->lastRenderedOnTickMs >= 500;
}


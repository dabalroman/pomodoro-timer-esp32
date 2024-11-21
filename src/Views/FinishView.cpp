#include "FinishView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void FinishView::handleInput() {
    if (touch.selectButton.takeActionIfPossible()) {
        this->deviceState = DeviceState::ready;
    }
}

void FinishView::render() {
    this->ledManager.setState(LEDManagerState::ringing);

    if (!this->shouldRender()) {
        return;
    }

    View::render();

    bool showDigits = (lastTickMs / 500) % 2 == 0;
    String text = "00:00";

    this->display.setFont();
    this->display.setCursor(0, 9);
    this->display.print("Time to take a break!");

    this->display.setFont(&FreeMonoBold18pt7b);
    this->display.setCursor(12, 44);

    if (showDigits) {
        this->display.print(text);
    }
}

bool FinishView::shouldRender() const {
    // 2fps is enough for this view
    return this->lastTickMs - this->lastRenderedOnTickMs >= 500;
}


#include "MainView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void MainView::handleInput() {
    if (touch.rightButton.takeActionIfPossibleLongTouch()) {
        deviceState = DeviceState::editMinutes;
    }

    if (touch.selectButton.takeActionIfPossible()) {
        deviceState = DeviceState::counting;
        countdownStartTickMs = lastTickMs;
    }
}

void MainView::render() {
    ledManager.setState(idle);

    if (!this->shouldRender()) {
        return;
    }

    View::render();

    String text = Formatter::formatTime(countdownStartValueMs);

    display.setFont();
    display.setCursor(49, 9);
    display.print("Ready");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(12, 44);
    display.print(text);
}

bool MainView::shouldRender() const {
    // 1fps is already too much
    // TODO: Make it update only on device state change, on first render
    return this->lastTickMs - this->lastRenderedOnTickMs >= 1000;
}

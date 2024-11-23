#include "MainView.h"
#include <Fonts/FreeMonoBold18pt7b.h>

void MainView::handleInput() {
    if (touch.rightButton.takeActionIfPossibleLongTouch()) {
        this->deviceState = DeviceState::settings;
    }

    if (touch.selectButton.takeActionIfPossible()) {
        this->deviceState = DeviceState::counting;
        this->countdownStartTickMs = lastTickMs;
    }
}

void MainView::render() {
    this->ledManager.setState(idle);

    if (!this->shouldRender()) {
        return;
    }

    View::render();

    String text = Formatter::formatTime(countdownStartValueMs);

    this->display.setFont();
    Formatter::printCenteredText(this->display, "Ready", 64, 9);

    if (this->pomodoroTargetAmount != 0) {
        char buffer[7];
        snprintf(buffer, sizeof(buffer), "%d / %d", this->pomodoroFinishedAmount, this->pomodoroTargetAmount);
        Formatter::printCenteredText(this->display, buffer, 64, 49);
    }

    this->display.setFont(&FreeMonoBold18pt7b);
    this->display.setCursor(12, 44);
    this->display.print(text);
}

bool MainView::shouldRender() const {
    // 1fps is already too much
    // TODO: Make it update only on device state change, on first render
    return this->lastTickMs - this->lastRenderedOnTickMs >= 1000;
}

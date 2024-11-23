#include "EditPomodoroTargetView.h"
#include "Fonts/FreeMonoBold18pt7b.h"

void EditPomodoroTargetView::handleInput() {
    if (!this->hasEditBeenRendered) {
        return;
    }

    if (touch.selectButton.takeActionIfPossible()) {
        this->preferencesManager.saveTargetPomodoroAmount(this->pomodoroTargetAmount);
        this->deviceState = DeviceState::settings;
    }

    if (touch.rightButton.isTouched()) {
        this->pomodoroTargetAmount++;
    }

    if (touch.leftButton.isTouched()) {
        this->pomodoroTargetAmount--;
    }

    if (this->pomodoroTargetAmount >= 255) {
        this->pomodoroTargetAmount = 0;
    }

    if (this->pomodoroTargetAmount > 32) {
        this->pomodoroTargetAmount = 32;
    }

    this->hasEditBeenRendered = false;
}

void EditPomodoroTargetView::render() {
    this->ledManager.setState(LEDManagerState::idle);

    if (!this->shouldRender()) {
        return;
    }

    View::render();

    String text = Formatter::formatNumber(this->pomodoroTargetAmount);

    this->display.setFont();
    this->display.setCursor(20, 9);
    this->display.print("Pomodoro target");

    this->display.setFont(&FreeMonoBold18pt7b);
    this->display.setCursor(46, 44);
    this->display.print(text);

    this->hasEditBeenRendered = true;
}


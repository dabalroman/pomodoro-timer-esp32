#include "SettingsView.h"

void SettingsView::handleInput() {
    if(this->touch.selectButton.takeActionIfPossible()) {
        switch (this->selectedOptionId) {
            case Settings::timerLength:
                this->deviceState = DeviceState::editMinutes;
                break;

            case Settings::targetAmountOfPomodoros:
                this->deviceState = DeviceState::ready;
                break;

            case Settings::goBack:
                this->selectedOptionId = 0;
                this->deviceState = DeviceState::ready;
                break;
        }
    }

    if(this->touch.leftButton.takeActionIfPossible()) {
        this->selectedOptionId--;
    }

    if(this->touch.rightButton.takeActionIfPossible()) {
        this->selectedOptionId++;
    }

    if(this->selectedOptionId == this->amountOfOptions) {
        this->selectedOptionId = 0;
    }

    // Overflow of uint8_t
    if(this->selectedOptionId > this->amountOfOptions) {
        this->selectedOptionId = this->amountOfOptions - 1;
    }
}

void SettingsView::render() {
    this->ledManager.setState(LEDManagerState::idle);

    if (!shouldRender()) {
        return;
    }

    View::render();

    this->display.setFont();
    this->display.setCursor(37, 9);
    this->display.print("Settings");

    this->display.setCursor(0, static_cast<int16_t>(22 + 12 * this->selectedOptionId));
    this->display.print(">");

    this->display.setCursor(10, 22);
    this->display.print("Timer length");

    this->display.setCursor(10, 22 + 12);
    this->display.print("Amount of pomodoros");

    this->display.setCursor(10, 22 + 12 * 2);
    this->display.print("Return");
}

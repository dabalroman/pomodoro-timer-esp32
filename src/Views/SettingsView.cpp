#include "SettingsView.h"

void SettingsView::handleInput() {
    if (this->touch.selectButton.takeActionIfPossible()) {
        switch (this->selectedOptionId) {
            case Settings::timerLength:
                this->deviceState = DeviceState::edit_timeMinutes;
                break;

            case Settings::targetAmountOfPomodoros:
                this->deviceState = DeviceState::edit_pomodoroTarget;
                break;

            case Settings::wifiCredentials:
                this->deviceState = DeviceState::settings_wifiCredentials;
                break;

            case Settings::syncTimeViaNTP:
                this->deviceState = DeviceState::settings_syncTimeViaNTP;
                break;

            case Settings::goBack:
                this->selectedOptionId = 0;
                this->deviceState = DeviceState::ready;
                break;
        }
        this->touch.preventAccidentalActionFor();
    }

    if (this->touch.leftButton.takeActionIfPossible()) {
        this->selectedOptionId--;
    }

    if (this->touch.rightButton.takeActionIfPossible()) {
        this->selectedOptionId++;
    }

    if (this->selectedOptionId == this->amountOfOptions) {
        this->selectedOptionId = 0;
    }

    // Rollover to first option
    if (this->selectedOptionId > this->amountOfOptions) {
        this->selectedOptionId = this->amountOfOptions - 1;
    }

    // Determine options offset on display
    if (this->selectedOptionId >= this->optionsListOffset + this->amountOfOptionsOnScreen) {
        this->optionsListOffset = this->selectedOptionId - this->amountOfOptionsOnScreen + 1;
    }

    if (this->selectedOptionId < this->optionsListOffset) {
        this->optionsListOffset = this->selectedOptionId;
    }

    // Do not allow for scrolling past the last option
    if (this->optionsListOffset > this->amountOfOptions - this->amountOfOptionsOnScreen) {
        this->optionsListOffset = this->amountOfOptions - this->amountOfOptionsOnScreen;
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

    this->display.setCursor(0, static_cast<int16_t>(22 + 12 * (this->selectedOptionId - this->optionsListOffset)));
    this->display.print(">");

    this->display.setCursor(10, 22);
    this->display.print(this->options[this->optionsListOffset]);

    this->display.setCursor(10, 22 + 12);
    this->display.print(this->options[this->optionsListOffset + 1]);

    this->display.setCursor(10, 22 + 12 * 2);
    this->display.print(this->options[this->optionsListOffset + 2]);
}

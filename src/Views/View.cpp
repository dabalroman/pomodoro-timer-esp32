#include "View.h"

void View::render() {
    this->lastRenderedOnTickMs = this->lastTickMs;
    this->display.clearDisplay();
}

bool View::shouldRender() const {
    // Cap to 20fps
    return this->lastTickMs - this->lastRenderedOnTickMs >= 50;
}

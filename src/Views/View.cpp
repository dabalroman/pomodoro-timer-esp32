#include "View.h"

void View::render() {
    this->lastRenderedOnTickMs = this->lastTickMs;
    this->display.clearDisplay();
}

bool View::shouldRender() const {
    // Cap to 10fps
    return this->lastTickMs - this->lastRenderedOnTickMs >= 100;
}

#include "Touch.h"

#ifndef ESP_POMODORO_CLOCK_TOUCHMANAGER_H
#define ESP_POMODORO_CLOCK_TOUCHMANAGER_H

#define GPIO_TOUCH_L 3
#define GPIO_TOUCH_S 5
#define GPIO_TOUCH_R 7


class TouchManager {
public:
    Touch leftButton = Touch(GPIO_TOUCH_L);
    Touch selectButton = Touch(GPIO_TOUCH_S);
    Touch rightButton = Touch(GPIO_TOUCH_R);

    String getTouchDebugValue() const {
        return String()
               + "L" + (this->leftButton.isTouched() ? "_" : "-")
               + (this->leftButton.canTakeAction() ? "O" : "X")
               + "      S" + (this->selectButton.isTouched() ? "_" : "-")
               + (this->selectButton.canTakeAction() ? "O" : "X")
               + "      R" + (this->rightButton.isTouched() ? "_" : "-")
               + (this->rightButton.canTakeAction() ? "O" : "X");
    }

    void update() {
        this->leftButton.updateState();
        this->selectButton.updateState();
        this->rightButton.updateState();
    }
};

#endif //ESP_POMODORO_CLOCK_TOUCHMANAGER_H

#include "Touch.h"

#ifndef ESP_POMODORO_CLOCK_TOUCHMANAGER_H
#define ESP_POMODORO_CLOCK_TOUCHMANAGER_H

#define GPIO_TOUCH_L 3
#define GPIO_TOUCH_S 5
#define GPIO_TOUCH_R 7


class TouchManager {
public:
    Touch left = Touch(GPIO_TOUCH_L);
    Touch select = Touch(GPIO_TOUCH_S);
    Touch right = Touch(GPIO_TOUCH_R);

    String getTouchDebugValue() const {
        return String()
                + "L" + (this->left.isTouched() ? "_" : "-") + (this->left.canTakeAction() ? "O" : "X")
                + "      S" + (this->select.isTouched() ? "_" : "-") + (this->select.canTakeAction() ? "O" : "X")
                + "      R" + (this->right.isTouched() ? "_" : "-") + (this->right.canTakeAction() ? "O" : "X");
    }

    void updateState() {
        this->left.updateState();
        this->select.updateState();
        this->right.updateState();
    }
};

#endif //ESP_POMODORO_CLOCK_TOUCHMANAGER_H

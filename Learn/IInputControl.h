#pragma once
#include <queue>
#include "InputManager.h"
class IInputControl {
public:
    IInputControl() {
        InputManager::instance().registerControl(this);
    }

    virtual ~IInputControl() {
        InputManager::instance().unregisterControl(this);
    }

    virtual void processKeyboard(unsigned char key, int x, int y) = 0;
    virtual void processMouse(int button, int state, int x, int y) = 0;
    virtual void processMouseMotion(int x, int y) = 0;
};

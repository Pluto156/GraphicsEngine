#include "stdafx.h"
#include "InputManager.h"
void InputManager::Update() {
    // 处理键盘事件
    while (!keyboard_events_.empty()) {
        auto& event = keyboard_events_.front();
        for (auto* control : controls_) {
            control->processKeyboard(event.key, event.x, event.y);
        }
        keyboard_events_.pop();
    }

    while (!specialkey_events_.empty()) {
        auto& event = specialkey_events_.front();
        for (auto* control : controls_) {
            control->processSpecialKeys(event.key, event.x, event.y);
        }
        specialkey_events_.pop();
    }

    // 处理鼠标事件
    while (!mouse_events_.empty()) {
        auto& event = mouse_events_.front();
        for (auto* control : controls_) {
            control->processMouse(event.button, event.state, event.x, event.y);
        }
        mouse_events_.pop();
    }

    // 处理鼠标移动事件
    while (!mouse_motion_events_.empty()) {
        auto& event = mouse_motion_events_.front();
        for (auto* control : controls_) {
            control->processMouseMotion(event.x, event.y);
        }
        mouse_motion_events_.pop();
    }
}
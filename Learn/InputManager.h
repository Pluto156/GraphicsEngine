#pragma once
#include<queue>
#include "IManager.h"
class InputManager:IManager {
public:
    class IInputControl {
    public:
        IInputControl() {
            InputManager::instance().registerControl(this);
        }

        virtual ~IInputControl() {
            InputManager::instance().unregisterControl(this);
        }

        virtual void processKeyboard(unsigned char key, int x, int y) = 0;
        virtual void processSpecialKeys(int key, int x, int y) = 0;
        virtual void processMouse(int button, int state, int x, int y) = 0;
        virtual void processMouseMotion(int x, int y) = 0;
    };

    static InputManager& instance() {
        static InputManager instance;
        return instance;
    }

    void registerControl(IInputControl* control) {
        controls_.push_back(control);
    }

    void unregisterControl(IInputControl* control) {
        auto it = std::find(controls_.begin(), controls_.end(), control);
        if (it != controls_.end()) {
            controls_.erase(it);
        }
    }

    void enqueueKeyboardEvent(unsigned char key, int x, int y) {
        keyboard_events_.push({ key, x, y });
    }

    void enqueueSpecialKeyEvent(int key, int x, int y) {
        specialkey_events_.push({ key, x, y });
    }

    void enqueueMouseEvent(int button, int state, int x, int y) {
        mouse_events_.push({ button, state, x, y });
    }

    void enqueueMouseMotionEvent(int x, int y) {
        mouse_motion_events_.push({ x, y });
    }

    void Update()override {
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

private:
    struct KeyboardEvent {
        unsigned char key;
        int x;
        int y;
    };

    struct SpecialKeyEvent {
        int key;
        int x;
        int y;
    };

    struct MouseEvent {
        int button;
        int state;
        int x;
        int y;
    };

    struct MouseMotionEvent {
        int x;
        int y;
    };

    std::vector<IInputControl*> controls_;
    std::queue<KeyboardEvent> keyboard_events_;
    std::queue<SpecialKeyEvent> specialkey_events_;
    std::queue<MouseEvent> mouse_events_;
    std::queue<MouseMotionEvent> mouse_motion_events_;

    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
};


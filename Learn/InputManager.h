#pragma once
#include<queue>
#include "IManager.h"
class InputManager:public IManager {
public:
    class IInputControl {
    public:
        IInputControl() {
            InputManager::Instance().registerControl(this);
        }

        virtual ~IInputControl() {
            InputManager::Instance().unregisterControl(this);
        }

        virtual void processKeyboard(unsigned char key, int x, int y) = 0;
        virtual void processSpecialKeys(int key, int x, int y) = 0;
        virtual void processMouse(int button, int state, int x, int y) = 0;
        virtual void processMouseMotion(int x, int y) = 0;
    };

    static InputManager& Instance() {
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

    void Update()override;

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


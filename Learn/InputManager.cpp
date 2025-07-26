#include "stdafx.h"
#include "InputManager.h"

InputManager& InputManager::Instance() {
    static InputManager inst;
    return inst;
}

void InputManager::onKeyDown(int key, int x, int y) {
    KeyCode kc = MapNormalKey(key);
    if (kc != KeyCode::None && !keys_held_.count(kc)) keys_down_.insert(kc);
    keys_held_.insert(kc);
}

void InputManager::onKeyUp(int key, int x, int y) {
    KeyCode kc = MapNormalKey(key);
    keys_held_.erase(kc);
    keys_up_.insert(kc);
}

void InputManager::onSpecialKeyDown(int key, int x, int y) {
    KeyCode kc = MapSpecialKey(key);
    if (kc != KeyCode::None && !keys_held_.count(kc)) keys_down_.insert(kc);
    keys_held_.insert(kc);
}

void InputManager::onSpecialKeyUp(int key, int x, int y) {
    KeyCode kc = MapSpecialKey(key);
    keys_held_.erase(kc);
    keys_up_.insert(kc);
}

void InputManager::onMouseButton(int button, int state, int x, int y) {
    KeyCode kc = MapMouseButton(button);
    if (kc == KeyCode::None) return;
    if (state == GLUT_DOWN) {
        if (!keys_held_.count(kc)) keys_down_.insert(kc);
        keys_held_.insert(kc);
    }
    else {
        keys_held_.erase(kc);
        keys_up_.insert(kc);
    }
}

void InputManager::onMouseMove(int x, int y) {
    mouse_x_ = x; mouse_y_ = y;
    mouse_delta_x_ = mouse_x_ - last_mouse_x_;
    mouse_delta_y_ = mouse_y_ - last_mouse_y_;
    last_mouse_x_ = mouse_x_;
    last_mouse_y_ = mouse_y_;
}

float InputManager::GetAxis(const std::string& axisName) const {
    if (axisName == "Mouse X") return static_cast<float>(mouse_delta_x_);
    if (axisName == "Mouse Y") return static_cast<float>(mouse_delta_y_);
    return 0.0f;
}

void InputManager::Update() {}
void InputManager::LateUpdate() {
    keys_down_.clear();
    keys_up_.clear();
    mouse_delta_x_ = mouse_delta_y_ = 0;
}

bool InputManager::GetKey(KeyCode key) const {
    return keys_held_.count(key) > 0;
}
bool InputManager::GetKeyDown(KeyCode key) const {
    return keys_down_.count(key) > 0;
}
bool InputManager::GetKeyUp(KeyCode key) const {
    return keys_up_.count(key) > 0;
}

KeyCode InputManager::MapNormalKey(int key) const {
    if (key >= 'A' && key <= 'Z') key += 32;  // 转小写
    if (key >= 'a' && key <= 'z') return static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (key - 'a'));
    if (key >= '0' && key <= '9') return static_cast<KeyCode>(static_cast<int>(KeyCode::Alpha0) + (key - '0'));
    switch (key) {
    case ' ': return KeyCode::Space;
    case 13: return KeyCode::Return;
    case 8: return KeyCode::Backspace;
    case '\t': return KeyCode::Tab;
    case '`': return KeyCode::BackQuote;
    case '-': return KeyCode::Minus;
    case '=': return KeyCode::Equals;
    case '[': return KeyCode::LeftBracket;
    case ']': return KeyCode::RightBracket;
    case '\\': return KeyCode::Backslash;
    case ';': return KeyCode::Semicolon;
    case '\'': return KeyCode::Quote;
    case ',': return KeyCode::Comma;
    case '.': return KeyCode::Period;
    case '/': return KeyCode::Slash;
    case '~': return KeyCode::Tilde;
    case '!': return KeyCode::Exclaim;
    case '@': return KeyCode::At;
    case '#': return KeyCode::Hash;
    case '$': return KeyCode::Dollar;
    case '%': return KeyCode::Percent;
    case '^': return KeyCode::Caret;
    case '&': return KeyCode::Ampersand;
    case '*': return KeyCode::Asterisk;
    case '(': return KeyCode::LeftParen;
    case ')': return KeyCode::RightParen;
    case '_': return KeyCode::Underscore;
    case '+': return KeyCode::Plus;
    case '<': return KeyCode::Less;
    case '>': return KeyCode::Greater;
    case '?': return KeyCode::Question;
    default: return KeyCode::None;
    }
}

KeyCode InputManager::MapSpecialKey(int key) const {
    switch (key) {
    case GLUT_KEY_UP: return KeyCode::UpArrow;
    case GLUT_KEY_DOWN: return KeyCode::DownArrow;
    case GLUT_KEY_LEFT: return KeyCode::LeftArrow;
    case GLUT_KEY_RIGHT: return KeyCode::RightArrow;
    case GLUT_KEY_PAGE_UP: return KeyCode::PageUp;
    case GLUT_KEY_PAGE_DOWN: return KeyCode::PageDown;
    case GLUT_KEY_HOME: return KeyCode::Home;
    case GLUT_KEY_END: return KeyCode::End;
    case GLUT_KEY_INSERT: return KeyCode::Insert;
    case GLUT_KEY_F1: return KeyCode::F1;
    case GLUT_KEY_F2: return KeyCode::F2;
    case GLUT_KEY_F3: return KeyCode::F3;
    case GLUT_KEY_F4: return KeyCode::F4;
    case GLUT_KEY_F5: return KeyCode::F5;
    case GLUT_KEY_F6: return KeyCode::F6;
    case GLUT_KEY_F7: return KeyCode::F7;
    case GLUT_KEY_F8: return KeyCode::F8;
    case GLUT_KEY_F9: return KeyCode::F9;
    case GLUT_KEY_F10: return KeyCode::F10;
    case GLUT_KEY_F11: return KeyCode::F11;
    case GLUT_KEY_F12: return KeyCode::F12;
    default: return KeyCode::None;
    }
}

KeyCode InputManager::MapMouseButton(int button) const {
    switch (button) {
    case GLUT_LEFT_BUTTON: return KeyCode::Mouse0;
    case GLUT_RIGHT_BUTTON: return KeyCode::Mouse1;
    case GLUT_MIDDLE_BUTTON: return KeyCode::Mouse2;
    default: return KeyCode::None;
    }
}

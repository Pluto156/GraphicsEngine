#include "stdafx.h"
#include "InputManager.h"

// 普通键按下
void InputManager::onKeyDown(int key, int x, int y) {
    if (!keys_held_.count(key)) {
        keys_down_.insert(key);  // 只在第一次按下时记录
    }
    keys_held_.insert(key);
}

// 普通键抬起
void InputManager::onKeyUp(int key, int x, int y) {
    keys_up_.insert(key);
    keys_held_.erase(key);
}

// 特殊键按下
void InputManager::onSpecialKeyDown(int key, int x, int y) {
    if (!special_keys_held_.count(key)) {
        special_keys_down_.insert(key);
    }
    special_keys_held_.insert(key);
}

// 特殊键抬起
void InputManager::onSpecialKeyUp(int key, int x, int y) {
    special_keys_up_.insert(key);
    special_keys_held_.erase(key);
}

// 查询接口
bool InputManager::GetKey(int key) const {
    return keys_held_.count(key) || special_keys_held_.count(key) || mouse_buttons_held_.count(key);
}

bool InputManager::GetKeyDown(int key) const {
    return keys_down_.count(key) || special_keys_down_.count(key) || mouse_buttons_down_.count(key);
}

bool InputManager::GetKeyUp(int key) const {
    return keys_up_.count(key) || special_keys_up_.count(key) || mouse_buttons_up_.count(key);
}

bool InputManager::GetSpecialKey(int key) const {
    return special_keys_held_.count(key) > 0;
}

bool InputManager::GetSpecialKeyDown(int key) const {
    return special_keys_down_.count(key) > 0;
}

bool InputManager::GetSpecialKeyUp(int key) const {
    return special_keys_up_.count(key) > 0;
}

void InputManager::Update() {



}
void InputManager::LateUpdate()
{
    keys_down_.clear();
    keys_up_.clear();
    special_keys_down_.clear();
    special_keys_up_.clear();
    mouse_buttons_down_.clear();
    mouse_buttons_up_.clear();
    // 鼠标 delta 归零（每帧只生效一次）
    mouse_delta_x_ = 0;
    mouse_delta_y_ = 0;
}


void InputManager::onMouseMove(int x, int y) {
    mouse_x_ = x;
    mouse_y_ = y;

    mouse_delta_x_ = mouse_x_ - last_mouse_x_;
    mouse_delta_y_ = mouse_y_ - last_mouse_y_;

    last_mouse_x_ = mouse_x_;
    last_mouse_y_ = mouse_y_;
}

float InputManager::GetAxis(const std::string& axis) const {
    if (axis == "Mouse X") return static_cast<float>(mouse_delta_x_);
    if (axis == "Mouse Y") return static_cast<float>(mouse_delta_y_);
    return 0.0f;
}

void InputManager::onMouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (!mouse_buttons_held_.count(button)) {
            mouse_buttons_down_.insert(button);
        }
        mouse_buttons_held_.insert(button);
    }
    else if (state == GLUT_UP) {
        mouse_buttons_held_.erase(button);
        mouse_buttons_up_.insert(button);
    }
}

#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "IManager.h"

class InputManager : public IManager {
public:
    static InputManager& Instance() {
        static InputManager instance;
        return instance;
    }

    // GLUT 回调接口
    void onKeyDown(int key, int x, int y);
    void onKeyUp(int key, int x, int y);
    void onSpecialKeyDown(int key, int x, int y);
    void onSpecialKeyUp(int key, int x, int y);

    // 状态查询接口（类似 Unity）
    bool GetKey(int key) const;
    bool GetKeyDown(int key) const;
    bool GetKeyUp(int key) const;

    bool GetSpecialKey(int key) const;
    bool GetSpecialKeyDown(int key) const;
    bool GetSpecialKeyUp(int key) const;

    // 每帧调用：清理上一帧的状态
    void Update() override;
    void LateUpdate() override;

    void onMouseMove(int x, int y);
    float GetAxis(const std::string& axisName) const;
    void onMouseButton(int button, int state, int x, int y);

private:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_set<int> keys_down_;       // 本帧按下
    std::unordered_set<int> keys_held_;       // 当前按住
    std::unordered_set<int> keys_up_;         // 本帧抬起

    std::unordered_set<int> special_keys_down_;
    std::unordered_set<int> special_keys_held_;
    std::unordered_set<int> special_keys_up_;

    std::unordered_set<int> mouse_buttons_held_;
    std::unordered_set<int> mouse_buttons_down_;
    std::unordered_set<int> mouse_buttons_up_;

    int mouse_x_ = 0, mouse_y_ = 0;
    int last_mouse_x_ = 0, last_mouse_y_ = 0;
    int mouse_delta_x_ = 0, mouse_delta_y_ = 0;
};

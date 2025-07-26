#pragma once
#include <unordered_set>
#include <string>
#include "IManager.h"

enum class KeyCode {
    None = 0,            // 未分配（无按键）
    Backspace,           // Backspace 键
    Tab,                 // Tab 键
    Clear,               // 清除键
    Return,              // 回车键（Return）
    Pause,               // Pause 暂停键
    Escape,              // Esc 键
    Space,               // 空格键

    // 小键盘
    Keypad0,             // 小键盘数字 0
    Keypad1,             // 小键盘数字 1
    Keypad2,             // 小键盘数字 2
    Keypad3,             // 小键盘数字 3
    Keypad4,             // 小键盘数字 4
    Keypad5,             // 小键盘数字 5
    Keypad6,             // 小键盘数字 6
    Keypad7,             // 小键盘数字 7
    Keypad8,             // 小键盘数字 8
    Keypad9,             // 小键盘数字 9
    KeypadPeriod,        // 小键盘句点（.）
    KeypadDivide,        // 小键盘除号（/）
    KeypadMultiply,      // 小键盘乘号（*）
    KeypadMinus,         // 小键盘减号（-）
    KeypadPlus,          // 小键盘加号（+）
    KeypadEnter,         // 小键盘回车
    KeypadEquals,        // 小键盘等号（=）

    // 箭头与控制键
    UpArrow,             // 向上箭头
    DownArrow,           // 向下箭头
    RightArrow,          // 向右箭头
    LeftArrow,           // 向左箭头
    Insert,              // Insert 插入键
    Home,                // Home 主页键
    End,                 // End 结束键
    PageUp,              // 向上翻页
    PageDown,            // 向下翻页

    // 功能键
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,  // F1-F15 功能键

    // 主要键（数字、符号、字母）
    Alpha0,              // 字母数字键盘上的数字 0
    Alpha1,              // 字母数字键盘上的数字 1
    Alpha2,              // 字母数字键盘上的数字 2
    Alpha3,              // 字母数字键盘上的数字 3
    Alpha4,              // 字母数字键盘上的数字 4
    Alpha5,              // 字母数字键盘上的数字 5
    Alpha6,              // 字母数字键盘上的数字 6
    Alpha7,              // 字母数字键盘上的数字 7
    Alpha8,              // 字母数字键盘上的数字 8
    Alpha9,              // 字母数字键盘上的数字 9

    Exclaim,             // 叹号（!）
    DoubleQuote,         // 双引号（"）
    Hash,                // 井号（#）
    Dollar,              // 美元符号（$）
    Percent,             // 百分号（%）
    Ampersand,           // 与号（&）
    Quote,               // 单引号（'）
    LeftParen,           // 左圆括号（(）
    RightParen,          // 右圆括号（)）
    Asterisk,            // 星号（*）
    Plus,                // 加号（+）
    Comma,               // 逗号（,）
    Minus,               // 减号（-）
    Period,              // 句号（.）
    Slash,               // 斜杠（/）
    Colon,               // 冒号（:）
    Semicolon,           // 分号（;）
    Less,                // 小于号（<）
    Equals,              // 等号（=）
    Greater,             // 大于号（>）
    Question,            // 问号（?）
    At,                  // 艾特符号（@）
    LeftBracket,         // 左方括号（[）
    Backslash,           // 反斜杠（\）
    RightBracket,        // 右方括号（]）
    Caret,               // 插入符号（^）
    Underscore,          // 下划线（_）
    BackQuote,           // 反引号（`）

    // 字母键 A-Z
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    LeftCurlyBracket,    // 左大括号（{）
    Pipe,                // 竖线（|）
    RightCurlyBracket,   // 右大括号（}）
    Tilde,               // 波浪号（~）

    Numlock,             // NumLock 键
    CapsLock,            // CapsLock 键
    ScrollLock,          // Scroll Lock 键

    RightShift,          // 右 Shift
    LeftShift,           // 左 Shift
    RightControl,        // 右 Ctrl
    LeftControl,         // 左 Ctrl
    RightAlt,            // 右 Alt
    LeftAlt,             // 左 Alt

    RightMeta,           // 右 Meta（Windows 或 Command）
    LeftMeta,            // 左 Meta（Windows 或 Command）
    RightCommand,        // 右 Command
    LeftCommand,         // 左 Command
    RightApple,          // 右 Apple
    LeftApple,           // 左 Apple
    RightWindows,        // 右 Windows
    LeftWindows,         // 左 Windows

    AltGr,               // Alt Gr 键
    Help,                // 帮助键
    Print,               // 打印键
    SysReq,              // 系统请求键
    Break,               // 中断键
    Menu,                // 菜单键

    // 鼠标按键
    Mouse0,              // 左（或主要）鼠标按钮
    Mouse1,              // 右（或辅助）鼠标按钮
    Mouse2,              // 中间鼠标按钮
    Mouse3,              // 第四鼠标按钮（附加）
    Mouse4,              // 第五鼠标按钮（附加）
    Mouse5,              // 第六鼠标按钮（附加）
    Mouse6               // 第七鼠标按钮（附加）
};



class InputManager : public IManager {
public:
    static InputManager& Instance();
    void onKeyDown(int key, int x, int y);
    void onKeyUp(int key, int x, int y);
    void onSpecialKeyDown(int key, int x, int y);
    void onSpecialKeyUp(int key, int x, int y);
    void onMouseButton(int button, int state, int x, int y);
    void onMouseMove(int x, int y);

    bool GetKey(KeyCode key) const;
    bool GetKeyDown(KeyCode key) const;
    bool GetKeyUp(KeyCode key) const;
    float GetAxis(const std::string& axisName) const;

    void Update() override;
    void LateUpdate() override;

private:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    KeyCode MapNormalKey(int key) const;
    KeyCode MapSpecialKey(int key) const;
    KeyCode MapMouseButton(int button) const;

    std::unordered_set<KeyCode> keys_down_, keys_held_, keys_up_;
    int mouse_x_ = 0, mouse_y_ = 0, last_mouse_x_ = 0, last_mouse_y_ = 0;
    int mouse_delta_x_ = 0, mouse_delta_y_ = 0;
};

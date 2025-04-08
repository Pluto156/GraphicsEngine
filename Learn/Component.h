#pragma once
class GameObject;
class Component {
public:
    GameObject* gameObject = nullptr;  // 反向引用所属对象

    virtual void Start() {}   // 初始化时调用
    virtual void Update() {}  // 每帧更新
    virtual ~Component() = default;
};


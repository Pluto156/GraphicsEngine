#pragma once
class GameObject;
class Component {
public:
    GameObject* gameObject = nullptr;  // ����������������

    virtual void Start() {}   // ��ʼ��ʱ����
    virtual void Update() {}  // ÿ֡����
    virtual ~Component() = default;
};


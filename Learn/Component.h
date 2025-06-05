#pragma once
enum class ComponentType
{
    Component,
    Transform,
    RigidBody,
    Collider,
    BoxCollider,
    SphereCollider,
    PlaneCollider,
    MeshRenderer,
    CharacterController
};
class GameObject;
class Component {
public:
    GameObject* gameObject = nullptr;  // 反向引用所属对象
    static ComponentType GetType();

    virtual void Start() {}   // 初始化时调用
    virtual void Update() {}  // 每帧更新
    virtual ~Component() = default;

};




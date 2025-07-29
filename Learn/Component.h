#pragma once
#include "Reflection.h"
enum class ComponentType
{
    Component,
    Transform,
    RigidBody,
    Collider,
    BoxCollider,
    SphereCollider,
    PlaneCollider,
    MeshFilter,
    MeshRenderer,
    CharacterController,
    GameScript,
    Light
};
class GameObject;
class Transform;
class Component {
public:
    GameObject* gameObject = nullptr;  // 反向引用所属对象
    Transform* transform;// 反向引用所属对象Transform

    virtual ComponentType GetType() const = 0;

    virtual void Start() {}   // 初始化时调用
    virtual void Update() {}  // 每帧更新
    virtual ~Component() = default;

    // Called after clone + pointer map fix
    virtual void PostClone(CloneContext&) {}

    Component* Clone(CloneContext& ctx) const {
        auto* typeInfo = ReflectionRegistry::Instance().GetTypeInfo(GetType());
        if (typeInfo) {
            return typeInfo->Clone(this, ctx);
        }
        return nullptr;
    }
};




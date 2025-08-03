#pragma once
#include "Component.h"

// 添加前向声明
namespace PhysicsLit {
    class RigidBodyPrimitive;
}
class Collider;
class GameScript : public Component {
    REGISTER_ABSTRACT_COMPONENT(GameScript, ComponentType::GameScript, Component)
public:
    bool IsGameScript() const override { return true; }
    GameScript();
    virtual ~GameScript();

    // 触发器和碰撞器函数：非纯虚，提供默认空实现
    virtual void OnColliderEnter(Collider* other) {}
    virtual void OnColliderStay(Collider* other) {}
    virtual void OnColliderExit(Collider* other) {}

    virtual void OnTriggerEnter(Collider* other) {}
    virtual void OnTriggerStay(Collider* other) {}
    virtual void OnTriggerExit(Collider* other) {}
};



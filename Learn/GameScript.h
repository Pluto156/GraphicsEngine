#pragma once
#include "Component.h"

// 添加前向声明
namespace PhysicsLit {
    class RigidBodyPrimitive;
}

class GameScript : public Component {
    REGISTER_ABSTRACT_COMPONENT(GameScript, ComponentType::GameScript, Component)
public:
    bool IsGameScript() const override { return true; }
    GameScript();
    virtual ~GameScript();

    // 触发器和碰撞器函数：非纯虚，提供默认空实现
    virtual void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*) {}
    virtual void OnColliderStay(PhysicsLit::RigidBodyPrimitive*) {}
    virtual void OnColliderExit(PhysicsLit::RigidBodyPrimitive*) {}

    virtual void OnTriggerEnter(PhysicsLit::RigidBodyPrimitive*) {}
    virtual void OnTriggerStay(PhysicsLit::RigidBodyPrimitive*) {}
    virtual void OnTriggerExit(PhysicsLit::RigidBodyPrimitive*) {}
};



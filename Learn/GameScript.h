#pragma once
#include "Component.h"

// 添加前向声明
namespace PhysicsLit {
    class RigidBodyPrimitive;
}

class GameScript : public Component {
public:
    static ComponentType GetType();
    GameScript();
    virtual ~GameScript();

    virtual void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*) = 0;
    virtual void OnColliderStay(PhysicsLit::RigidBodyPrimitive*) = 0;
    virtual void OnColliderExit(PhysicsLit::RigidBodyPrimitive*) = 0;
};



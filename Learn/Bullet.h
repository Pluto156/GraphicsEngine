#pragma once
class Bullet:public GameScript
{
    REGISTER_COMPONENT_DERIVED(Bullet, ComponentType::Bullet, GameScript)

public:
    void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderStay(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderExit(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerStay(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerExit(PhysicsLit::RigidBodyPrimitive*);

    void Awake()override;
    void Update()override;
    CVector3 StartPos;
    CVector3 MoveDir;
private:

    float MaxFlyDistance = 10;
    RigidBody* rigidBody;
};


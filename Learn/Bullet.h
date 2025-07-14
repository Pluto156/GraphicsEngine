#pragma once
class Bullet:public GameScript
{
public:
    void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderStay(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderExit(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerStay(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerExit(PhysicsLit::RigidBodyPrimitive*);

    void Update()override;

private:
    CVector3 StartPos;
    float MaxFlyDistance = 10;
};


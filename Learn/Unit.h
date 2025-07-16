#pragma once
class Unit : public GameScript
{
public:
    void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderStay(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderExit(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerStay(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerExit(PhysicsLit::RigidBodyPrimitive*);

    void Start()override;
    void Update()override;

private:

    int Health = 1;
};


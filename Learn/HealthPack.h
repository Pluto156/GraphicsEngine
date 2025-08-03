#pragma once
class HealthPack : public GameScript
{
    REGISTER_COMPONENT_DERIVED(HealthPack, ComponentType::HealthPack, GameScript)

public:
    void OnColliderEnter(PhysicsLit::RigidBodyPrimitive* other);
    void Awake()override;
    void Update()override;

private:

    float rotateSpeed = 0.5;
    float Euler_h = 0;
    int addHealth = 5;
    RigidBody* rigidBody;

};


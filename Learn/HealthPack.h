#pragma once
class HealthPack : public GameScript
{
    REGISTER_COMPONENT_DERIVED(HealthPack, ComponentType::HealthPack, GameScript)

public:
    void OnColliderEnter(Collider* other);
    void Awake()override;
    void Update()override;
    int addHealth = 5;

private:

    float rotateSpeed = 0.5;
    float Euler_h = 0;
    RigidBody* rigidBody;

};


#pragma once
class Bullet:public GameScript
{
    REGISTER_COMPONENT_DERIVED(Bullet, ComponentType::Bullet, GameScript)

public:
    void OnTriggerEnter(Collider* other);

    void Awake()override;
    void Update()override;
    CVector3 StartPos;
    CVector3 MoveDir;
private:

    float MaxFlyDistance = 10;
    RigidBody* rigidBody;
};


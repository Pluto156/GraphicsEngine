#pragma once
class CombatManager:public GameScript
{
public:

    static CombatManager& Instance() {
        static CombatManager instance;
        return instance;
    }
    GameObject* Car;
    GameObject* Car2;

    void RecycleCar(GameObject* Car)
    {
        auto unit = Car->GetComponent<Unit>();
        unit->ReSet();
        PhysicsLit::RigidBodyPrimitive* rigidBody = Car->GetComponent<RigidBody>()->rigidBodyPrimitive;
        rigidBody->SetPosition(CVector3(0, -1000, 0));
        ReSpawnCar(Car);
    }

    void ReSpawnCar(GameObject* Car)
    {
        PhysicsLit::RigidBodyPrimitive* rigidBody = Car->GetComponent<RigidBody>()->rigidBodyPrimitive;
        rigidBody->SetPosition(CVector3(6, 0.5 + 2.5 , 0));
    }

private:
    CombatManager() = default;
    ~CombatManager() = default;
    CombatManager(const CombatManager&) = delete;
    void operator=(const CombatManager&) = delete;
};


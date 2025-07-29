#pragma once
#include "CountDownTimerManager.h"

class CombatManager : public GameScript
{
public:

    static CombatManager& Instance() {
        static CombatManager instance;
        return instance;
    }

    GameObject* Car;
    GameObject* Car2;

    void RecycleCar(GameObject* car)
    {
        auto unit = car->GetComponent<Unit>();
        unit->ReSet();

        PhysicsLit::RigidBodyPrimitive* rigidBody = car->GetComponent<RigidBody>()->rigidBodyPrimitive;
        rigidBody->SetPosition(CVector3(0, -1000, 0));

        // 用计时器延迟3秒后调用 ReSpawnCar
        CountDownTimerManager::Instance().AddTimer(3.0f, [this, car]() {
            this->ReSpawnCar(car);
            Debug::Log("3秒后重生");
            });
    }

    void ReSpawnCar(GameObject* car)
    {
        PhysicsLit::RigidBodyPrimitive* rigidBody = car->GetComponent<RigidBody>()->rigidBodyPrimitive;
        rigidBody->SetPosition(CVector3(6, 0.5 + 2.5, 0));
    }

private:
    CombatManager() = default;
    ~CombatManager() = default;
    CombatManager(const CombatManager&) = delete;
    void operator=(const CombatManager&) = delete;
};

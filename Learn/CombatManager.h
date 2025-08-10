#pragma once
#include "CountDownTimerManager.h"
#include <vector>
#include <random>

class CombatManager : public GameScript
{
public:
    static CombatManager& Instance() {
        return *instance;
    }

    CombatManager() {
        instance = this;  // 在构造时绑定单例
    }

    // 预制体
    GameObject* Car = nullptr;
    GameObject* Car2 = nullptr;
    GameObject* HealthPackPrefab = nullptr;

    // ==== 可配置参数 ====
    int maxHealthPacks = 5; // 场上最大血包数
    float minSpawnInterval = 2.0f; // 最短生成间隔
    float maxSpawnInterval = 5.0f; // 最长生成间隔
    CVector3 spawnPositionMin = CVector3(-5, 1, -5);
    CVector3 spawnPositionMax = CVector3(5, 1, 5);

    // ==== 车辆回收 ====
    void RecycleCar(GameObject* car)
    {
        auto unit = car->GetComponent<Unit>();
        unit->ReSet();

        PhysicsLit::RigidBodyPrimitive* rigidBody = car->GetComponent<RigidBody>()->rigidBodyPrimitive;
        rigidBody->SetPosition(CVector3(0, -1000, 0));

        CountDownTimerManager::Instance().AddTimer(3.0f, [this, car]() {
            this->ReSpawnCar(car);
            Debug::Log("3秒后重生车辆");
            });
    }

    void ReSpawnCar(GameObject* car)
    {
        PhysicsLit::RigidBodyPrimitive* rigidBody = car->GetComponent<RigidBody>()->rigidBodyPrimitive;
        rigidBody->SetPosition(CVector3(6, 0.5 + 2.5, 0));
    }

    // ==== 血包回收 ====
    void RecycleHealthPack(GameObject* pack)
    {
        // 移出管理列表
        auto it = std::find(activeHealthPacks.begin(), activeHealthPacks.end(), pack);
        if (it != activeHealthPacks.end())
            activeHealthPacks.erase(it);

        // 将血包移到地图外（可替换为销毁）
        pack->transform->SetPosition(CVector3(0, -1000, 0));
        pack->transform->UpdateRigidBodyTransform();
        pack->transform->UpdateColliderTransform();

        // 尝试安排下一次生成
        TryScheduleHealthPackSpawn();
    }

    void Update() override;


private:
    static CombatManager* instance;
    ~CombatManager() = default;
    CombatManager(const CombatManager&) = delete;
    void operator=(const CombatManager&) = delete;

    std::vector<GameObject*> activeHealthPacks;
    bool isSpawningHealthPack = false;

    void TryScheduleHealthPackSpawn();
    void SpawnHealthPack();
};

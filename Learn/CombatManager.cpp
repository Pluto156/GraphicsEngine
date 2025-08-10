#include "stdafx.h"
#include "CombatManager.h"
CombatManager* CombatManager::instance = nullptr;
void CombatManager::Update()
{
    // 每帧检查是否需要安排生成血包
    if (!isSpawningHealthPack && activeHealthPacks.size() < (size_t)maxHealthPacks) {
        TryScheduleHealthPackSpawn();
    }
}

void CombatManager::TryScheduleHealthPackSpawn()
{
    if (isSpawningHealthPack) return;
    if (activeHealthPacks.size() >= (size_t)maxHealthPacks) return;

    isSpawningHealthPack = true;

    float delay = Math::RandomFloat(minSpawnInterval, maxSpawnInterval);
    CountDownTimerManager::Instance().AddTimer(delay, [this]() {
        SpawnHealthPack();
        isSpawningHealthPack = false;
        });
}

void CombatManager::SpawnHealthPack()
{
    if (!HealthPackPrefab) {
        Debug::Log("HealthPackPrefab 未设置，无法生成血包！");
        return;
    }
    if (activeHealthPacks.size() >= (size_t)maxHealthPacks) return;
    Debug::Log("Begin Clone");

    GameObject* go = GameObjectManager::Instance().Clone(HealthPackPrefab);

    // 随机位置
    float x = Math::RandomFloat(spawnPositionMin.x, spawnPositionMax.x);
    float y = 3;
    float z = Math::RandomFloat(spawnPositionMin.z, spawnPositionMax.z);
    go->transform->SetPosition(CVector3(x, y, z));
    go->transform->UpdateRigidBodyTransform();
    go->transform->UpdateColliderTransform();

    PhysicsLit::PhysicsManager::Instance().AddGameObject(go);
    activeHealthPacks.push_back(go);

    Debug::Log("SpawnHealthPack");
}

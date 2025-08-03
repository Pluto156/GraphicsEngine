#include "stdafx.h"
#include "CombatManager.h"
void CombatManager::Awake()
{
    CountDownTimerManager::Instance().AddTimer(3.0f, [this]() {
        GameObject* go = GameObjectManager::Instance().Clone(HealthPackPrefab);

        go->transform->SetPosition(CVector3(3, 3, 3));
        go->transform->UpdateRigidBodyTransform();
        go->transform->UpdateColliderTransform();
        PhysicsLit::PhysicsManager::Instance().AddGameObject(go);
        Debug::Log("3CountDownTimerManager");
        });
}

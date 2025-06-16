#pragma once
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>
#include "IManager.h"
#include "GameScript.h"


class GameScriptManager : public IManager {
public:

    static GameScriptManager& Instance() {
        static GameScriptManager instance;
        return instance;
    }

    void ReportCollision(PhysicsLit::RigidBodyPrimitive* a, PhysicsLit::RigidBodyPrimitive* b) {
        auto key = MakePairKey(a, b);
        currentFrameContacts.insert(key);

    }

    void Update() override;


private:
    GameScriptManager() = default;
    ~GameScriptManager() = default;
    GameScriptManager(const GameScriptManager&) = delete;
    GameScriptManager& operator=(const GameScriptManager&) = delete;

    using RigidBodyPrimitivePair = std::pair<PhysicsLit::RigidBodyPrimitive*, PhysicsLit::RigidBodyPrimitive*>;

    std::vector<GameScript*> scripts_;
    std::set<RigidBodyPrimitivePair> currentFrameContacts;
    std::set<RigidBodyPrimitivePair> lastFrameContacts;

    static RigidBodyPrimitivePair MakePairKey(PhysicsLit::RigidBodyPrimitive* a, PhysicsLit::RigidBodyPrimitive* b) {
        return (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    void TriggerCollisionEvents() {
        // Enter
        for (const auto& pair : currentFrameContacts) {
            if (lastFrameContacts.find(pair) == lastFrameContacts.end()) {
                Notify(pair.first, pair.second, &GameScript::OnColliderEnter);
            }
            else {
                Notify(pair.first, pair.second, &GameScript::OnColliderStay);
            }
        }

        // Exit
        for (const auto& pair : lastFrameContacts) {
            if (currentFrameContacts.find(pair) == currentFrameContacts.end()) {
                Notify(pair.first, pair.second, &GameScript::OnColliderExit);
            }
        }
    }

    using CollisionCallback = void (GameScript::*)(PhysicsLit::RigidBodyPrimitive*);

    void Notify(PhysicsLit::RigidBodyPrimitive* a, PhysicsLit::RigidBodyPrimitive* b, CollisionCallback callback) {
        GameScript* gs = nullptr;
        gs = PhysicsLit::PhysicsManager::Instance().TryGetGameScript(a);
        if (gs!=nullptr) {
            (gs->*callback)(b);
        }
        gs = PhysicsLit::PhysicsManager::Instance().TryGetGameScript(b);
        if (gs != nullptr) {
            (gs->*callback)(a);
        }
    }


};

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

    void registerScript(GameScript* script) {
        scripts_.push_back(script);
        auto rigidbody = script->gameObject->GetComponent<RigidBody>();
        if (rigidbody != nullptr)
        {
            RigidBodyToScript_[rigidbody->rigidBodyPrimitive] = script;

        }
    }

    void unregisterScript(GameScript* script) {
        auto it = std::find(scripts_.begin(), scripts_.end(), script);
        if (it != scripts_.end()) {
            scripts_.erase(it);
            RigidBodyToScript_.erase(script->gameObject->GetComponent<RigidBody>()->rigidBodyPrimitive);
        }
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
    std::unordered_map<PhysicsLit::RigidBodyPrimitive*, GameScript*> RigidBodyToScript_;

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
        if (RigidBodyToScript_.count(a)) {
            (RigidBodyToScript_[a]->*callback)(b);
        }
        if (RigidBodyToScript_.count(b)) {
            (RigidBodyToScript_[b]->*callback)(a);
        }
    }
};

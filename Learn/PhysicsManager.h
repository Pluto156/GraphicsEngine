#pragma once
#include "IManager.h"
#include "vector"
#include "RigidBody.h"

namespace PhysicsLit
{
    class PhysicsManager :public IManager {
    public:
        static PhysicsManager& Instance() {
            static PhysicsManager instance;
            return instance;
        }

        RigidBody* Register(GameObject* gameobject) {
            RigidBody* rigidbody = gameobject->AddComponent<RigidBody>();
            rigidbodys.push_back(rigidbody);
            return rigidbody;
        }

        void Unregister(GameObject* gameobject) {
            auto it = std::remove(rigidbodys.begin(), rigidbodys.end(), gameobject->GetComponent<RigidBody>());
            rigidbodys.erase(it, rigidbodys.end());
        }

        void Update()override;

        void BeginFrame();
        void UpdatePhysics(const float deltaTime);
        void EndFrame();


        PhysicsManager(const PhysicsManager&) = delete;
        void operator=(const PhysicsManager&) = delete;

    private:
        PhysicsManager() = default;
        ~PhysicsManager() = default;

        std::vector<RigidBody*> rigidbodys;
        long long mCurPhyFrame = 0;
    };
}



#pragma once
#include "IManager.h"
#include "vector"
#include "RigidBody.h"
#include "BVHNode.h"
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
        PhysicsManager()
        {
            mPotentialContacts = new PotentialContact[mMaxPotentialContacts];
        }
        ~PhysicsManager()
        {
            if (mBVHRoot)
                delete mBVHRoot;
            delete[] mPotentialContacts;
        }
        // 由GameObjectManager负责销毁
        std::vector<RigidBody*> rigidbodys;
        // Bounding Volume Hierarchy (BVH)树的根节点
        BVHNode* mBVHRoot = nullptr;
        // 潜在碰撞数组
        PotentialContact* mPotentialContacts;
        // 潜在碰撞数组的长度
        uint32_t mMaxPotentialContacts = 1000;
        long long mCurPhyFrame = 0;
    };
}



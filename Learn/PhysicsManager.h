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
        // ��GameObjectManager��������
        std::vector<RigidBody*> rigidbodys;
        // Bounding Volume Hierarchy (BVH)���ĸ��ڵ�
        BVHNode* mBVHRoot = nullptr;
        // Ǳ����ײ����
        PotentialContact* mPotentialContacts;
        // Ǳ����ײ����ĳ���
        uint32_t mMaxPotentialContacts = 1000;
        long long mCurPhyFrame = 0;
    };
}



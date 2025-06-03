#pragma once
#include "IManager.h"
#include "vector"
#include "Rigidbody.h"
#include "BVHNode.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"

namespace PhysicsLit
{
    class CollisionData;
    class ContactResolver;
    class PhysicsManager :public IManager {
    public:
        static PhysicsManager& Instance() {
            static PhysicsManager instance;
            return instance;
        }

        RigidBody* AddGameObject(GameObject* gameobject) {
            RigidBody* rigidbody = gameobject->GetComponent<RigidBody>();
            auto rigidBodyPrimitive = rigidbody ? rigidbody->rigidBodyPrimitive : nullptr;
            mAllRigidBodyGO.push_back(std::make_pair(gameobject, rigidBodyPrimitive));


            if (gameobject->GetComponent<BoxCollider>() != nullptr)
            {
                auto boxCollider = gameobject->GetComponent<BoxCollider>();
                float radius = boxCollider->mCollider->mHalfSize.len();
                CVector3 pos = gameobject->GetComponent<Transform>()->GetPosition();
                BoundingSphere bv(pos, radius);
                AddBoundingVolume(bv, rigidBodyPrimitive);
            }
            else if (gameobject->GetComponent<PlaneCollider>() != nullptr)
            {
                auto planeCollider = gameobject->GetComponent<PlaneCollider>();
                if (planeCollider)
                {
                    auto transform = gameobject->GetComponent<Transform>();
                    auto scale = CVector3(1,1,1);
                    // ��ʱ����ƽ�泤��Ϊ10����
                    float radius = sqrtf(scale.x * scale.x * 100.0f + scale.z * scale.z * 100.0f);
                    CVector3 pos = transform->GetPosition();
                    BoundingSphere bv(pos, radius);
                    AddBoundingVolume(bv, rigidBodyPrimitive);
                }
            }



            return rigidbody;
        }

        void RemoveGameObject(GameObject* gameobject) {
            // ���Ҳ��Ƴ���Ӧ�� GameObject
            auto it = std::remove_if(mAllRigidBodyGO.begin(), mAllRigidBodyGO.end(),
                [gameobject](const std::pair<GameObject*, RigidBodyPrimitive*>& pair) {
                    return pair.first == gameobject;
                });

            // ����ҵ���ƥ�����Ŀ����ɾ����
            if (it != mAllRigidBodyGO.end()) {
                mAllRigidBodyGO.erase(it, mAllRigidBodyGO.end());
            }
        }


        void Update()override;
        void BeginFrame();
        void UpdatePhysics(const float deltaTime);
        void EndFrame();

        void AddBoundingVolume(const BoundingSphere& boundingVolume, RigidBodyPrimitive* rigidBody);

        PhysicsManager(const PhysicsManager&) = delete;
        void operator=(const PhysicsManager&) = delete;

    private:
        PhysicsManager();

        ~PhysicsManager()
        {
            delete mCollisionData;
            delete mContactResolver;
            delete[] mPotentialContacts;
            if (mBVHRoot)
                delete mBVHRoot;
        }
        // ��GameObjectManager��������
          // ��ǰ�����е����и���
        std::vector<std::pair<GameObject*, RigidBodyPrimitive*>> mAllRigidBodyGO;
        // Bounding Volume Hierarchy (BVH)���ĸ��ڵ�
        BVHNode* mBVHRoot = nullptr;

        // ��ײ����
        CollisionData* mCollisionData;
        // Ǳ����ײ����
        PotentialContact* mPotentialContacts;
        // Ǳ����ײ����ĳ���
        uint32_t mMaxPotentialContacts = 1000;

        // ��ײ������
        ContactResolver* mContactResolver;
        long long mCurPhyFrame = 0;
    };
}



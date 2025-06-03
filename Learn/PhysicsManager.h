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
                    // 临时按照平面长宽为10计算
                    float radius = sqrtf(scale.x * scale.x * 100.0f + scale.z * scale.z * 100.0f);
                    CVector3 pos = transform->GetPosition();
                    BoundingSphere bv(pos, radius);
                    AddBoundingVolume(bv, rigidBodyPrimitive);
                }
            }



            return rigidbody;
        }

        void RemoveGameObject(GameObject* gameobject) {
            // 查找并移除对应的 GameObject
            auto it = std::remove_if(mAllRigidBodyGO.begin(), mAllRigidBodyGO.end(),
                [gameobject](const std::pair<GameObject*, RigidBodyPrimitive*>& pair) {
                    return pair.first == gameobject;
                });

            // 如果找到了匹配的条目，就删除它
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
        // 由GameObjectManager负责销毁
          // 当前场景中的所有刚体
        std::vector<std::pair<GameObject*, RigidBodyPrimitive*>> mAllRigidBodyGO;
        // Bounding Volume Hierarchy (BVH)树的根节点
        BVHNode* mBVHRoot = nullptr;

        // 碰撞数据
        CollisionData* mCollisionData;
        // 潜在碰撞数组
        PotentialContact* mPotentialContacts;
        // 潜在碰撞数组的长度
        uint32_t mMaxPotentialContacts = 1000;

        // 碰撞处理器
        ContactResolver* mContactResolver;
        long long mCurPhyFrame = 0;
    };
}



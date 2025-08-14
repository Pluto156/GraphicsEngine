#pragma once
#include "IManager.h"
#include <unordered_map>
#include <vector>
#include "Rigidbody.h"
#include "BVHNode.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"

namespace PhysicsLit
{
    class CollisionData;
    class ContactResolver;
    struct RaycastHit
    {
        RigidBodyPrimitive* rigidbody = nullptr; // 命中的刚体 primitive（可能为 nullptr）
        GameObject* gameObject = nullptr;        // 对应的 GameObject（若存在）
        float distance = 0.0f;                   // 从射线起点到碰撞点的距离
        CVector3 point;                          // 碰撞点世界坐标
        CVector3 normal;                         // 碰撞法线（单位向量）
        bool hit = false;                        // 是否命中
    };
    class PhysicsManager : public IManager {
    public:
        static PhysicsManager& Instance() {
            static PhysicsManager instance;
            return instance;
        }

        RigidBody* AddGameObject(GameObject* gameobject) {
            RigidBody* rigidbody = gameobject->GetComponent<RigidBody>();
            auto rigidBodyPrimitive = rigidbody ? rigidbody->rigidBodyPrimitive : nullptr;

            GoToRigidBody[gameobject] = rigidBodyPrimitive;
            if (rigidBodyPrimitive) {
                RigidBodyToGO[rigidBodyPrimitive] = gameobject;
            }

            // 添加包围体
            if (auto box = gameobject->GetComponent<BoxCollider>()) {
                float radius = box->mCollider->mHalfSize.len();
                CVector3 pos = gameobject->GetComponent<Transform>()->GetPosition();
                AddBoundingVolume(BoundingSphere(pos, radius), rigidBodyPrimitive);
            }
            else if (auto plane = gameobject->GetComponent<PlaneCollider>()) {
                float radius = sqrtf(1 * 1 * 100.0f + 1 * 1 * 100.0f);
                CVector3 pos = gameobject->GetComponent<Transform>()->GetPosition();
                AddBoundingVolume(BoundingSphere(pos, radius), rigidBodyPrimitive);
            }
            else if (auto sphere = gameobject->GetComponent<SphereCollider>()) {
                float radius = sphere->mCollider->mRadius;
                CVector3 pos = gameobject->transform->GetPosition();
                AddBoundingVolume(BoundingSphere(pos, radius), rigidBodyPrimitive);
            }

            PhysicsManager::Instance().CheckImmediateCollision(rigidbody->rigidBodyPrimitive);

            return rigidbody;
        }

        void RemoveGameObject(GameObject* gameobject);


        std::string GetGameObjectName(RigidBodyPrimitive* rigidbody);

        GameScript* TryGetGameScript(RigidBodyPrimitive* rigidbody)
        {
            auto it = RigidBodyToGO.find(rigidbody);
            if (it != RigidBodyToGO.end() && it->second) {
                return it->second->gameScript;
            }
            return nullptr;
        }

        void Update() override;
        void BeginFrame();
        void UpdatePhysics(const float deltaTime);
        void EndFrame();

        void AddBoundingVolume(const BoundingSphere& boundingVolume, RigidBodyPrimitive* rigidBody);
        void RemoveBoundingVolume(RigidBodyPrimitive* rigidBody);

        PhysicsManager(const PhysicsManager&) = delete;
        void operator=(const PhysicsManager&) = delete;
        void CheckImmediateCollision(RigidBodyPrimitive* newRigid);

        bool CanCollide(CollisionPrimitive* a, CollisionPrimitive* b) {
            return ((b->layer) & a->layerMask) != 0 &&
                ((a->layer) & b->layerMask) != 0;
        }

        // --- Raycast API (Unity-like) ---
        // origin: 射线起点（世界坐标）
        // direction: 单位方向向量（最好归一化）
        // outHit: 输出命中信息（最近一次命中）
        // maxDistance: 射线的最大检测距离（default: FLT_MAX）
        // layerMask: 可选，按位掩码过滤图层，默认 ~0（所有层）
        // includeTriggers: 是否也检测 trigger（默认 true）
        bool Raycast(
            const CVector3& origin,
            const CVector3& direction,
            RaycastHit& outHit,
            float maxDistance = FLT_MAX,
            unsigned int layerMask = ~0u,
            bool includeTriggers = true
        );

        // --- OverlapBox API (Unity-like) ---
        // center: 盒子中心（世界坐标）
        // halfExtents: 半尺寸（沿盒子局部 X/Y/Z 的半长）
        // rotation: 盒子的世界旋转（四元数）
        // layerMask: 按位图，用于层过滤（默认所有层）
        // includeTriggers: 是否也包含触发器（默认 true）
        // 返回：与该盒子重叠的 GameObject* 列表（可能包含 nullptr）
        std::vector<GameObject*> OverlapBox(
            const CVector3& center,
            const CVector3& halfExtents,
            const CQuaternion& rotation,
            unsigned int layerMask = ~0u,
            bool includeTriggers = true
        );

    private:
        PhysicsManager();

        ~PhysicsManager() {
            delete mCollisionData;
            delete mContactResolver;
            delete[] mPotentialContacts;
            if (mBVHRoot)
                delete mBVHRoot;
        }

        // 双向索引结构
        std::unordered_map<GameObject*, RigidBodyPrimitive*> GoToRigidBody;
        std::unordered_map<RigidBodyPrimitive*, GameObject*> RigidBodyToGO;

        BVHNode* mBVHRoot = nullptr;
        CollisionData* mCollisionData;
        PotentialContact* mPotentialContacts;
        uint32_t mMaxPotentialContacts = 1000;
        ContactResolver* mContactResolver;
        long long mCurPhyFrame = 0;
    };
}

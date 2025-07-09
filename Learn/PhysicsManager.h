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

            return rigidbody;
        }

        void RemoveGameObject(GameObject* gameobject) {
            auto it = GoToRigidBody.find(gameobject);
            if (it != GoToRigidBody.end()) {
                if (it->second) {
                    RemoveBoundingVolume(it->second); 
                    RigidBodyToGO.erase(it->second);
                }
                GoToRigidBody.erase(it);
            }
        }

        std::string GetGameObjectName(RigidBodyPrimitive* rigidbody) {
            auto it = RigidBodyToGO.find(rigidbody);
            if (it != RigidBodyToGO.end() && it->second) {
                return it->second->name;  
            }
            return "NULL";
        }

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

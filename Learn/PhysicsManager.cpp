#include "stdafx.h"
#include "PhysicsManager.h"
#include "CollisionData.h"
#include "CollisionDetector.h"
#include "ContactResolver.h"
namespace PhysicsLit
{

	PhysicsManager::PhysicsManager()
	{
		mCollisionData = new CollisionData(1000);
		mContactResolver = new ContactResolver(1000);
		mPotentialContacts = new PotentialContact[mMaxPotentialContacts];
	}

	void PhysicsManager::Update() {
		long long targetFrame = TimeManager::curTime_micro / TimeManager::fixedDeltaTime_micro;
		long long deltaFrame = targetFrame - mCurPhyFrame;

		// 当前游戏帧数高于物理引擎的目标帧数，跳过这一帧的物理更新
		if (deltaFrame <= 0)
			return;
		// 当前游戏帧数低于物理引擎的目标帧数，需要补物理帧，但是不能补太多，否则会导致游戏帧率进一步降低，恶性循环然后卡死
		else if (deltaFrame > 10)
			deltaFrame = 10;

		for (long long i = 0; i < deltaFrame; i++)
		{
			BeginFrame();
			GameObjectManager::Instance().Update();
			UpdatePhysics(TimeManager::fixedDeltaTime);
			EndFrame();
		}

		// 无论有没有补帧，或者补了多少帧，都当作已经追上了目标帧数
		mCurPhyFrame = targetFrame;
	}

	void PhysicsManager::BeginFrame()
	{
		if (mBVHRoot == nullptr)
			return;
		// 重置碰撞数据
		mCollisionData->Reset();

		for (auto& iter : mAllRigidBodyGO)
		{
			// 清除刚体在上一帧累计的力和力矩
			iter.second->ClearAccumulators();
			// 更新刚体在这一帧的相关数据
			iter.second->CalculateDerivedData();
		}
	}
	void PhysicsManager::UpdatePhysics(const float deltaTime)
	{
		if (mBVHRoot == nullptr)
			return;
		 //更新刚体的位置和旋转
		for (auto& iter : mAllRigidBodyGO)
		{
			//std::cout << rigidbody->forceAccum.ToString() << std::endl;

			iter.second->Integrate(deltaTime);
		}

		// 生成潜在碰撞
		uint32_t potentialContactCount = mBVHRoot->GetPotentialContacts(mPotentialContacts, mMaxPotentialContacts);
		//std::cout << potentialContactCount << std::endl;
		// 从潜在碰撞中检测碰撞
		uint32_t i = 0;
		while (i < potentialContactCount)
		{
			uint32_t collisionCount = CollisionDetector::Detect(
				mPotentialContacts[i].mRigidBodies[0]->mCollisionVolume,
				mPotentialContacts[i].mRigidBodies[1]->mCollisionVolume,
				mCollisionData
			);

			if (collisionCount > 0)
			{
				std::cout << "有碰撞" << std::endl;
			}
			i++;
		}


		// 处理碰撞
		mContactResolver->ResolveContacts(mCollisionData->mContactArray, mCollisionData->mCurContactCount, deltaTime);
	}
	void PhysicsManager::EndFrame()
	{
		if (mBVHRoot == nullptr)
			return;

		for (auto& iter : mAllRigidBodyGO)
		{
			auto transform = iter.first->GetComponent<Transform>();
			transform->SetPosition(iter.second->GetPosition());
			transform->SetQuaternion(iter.second->GetRotation());
			//std::cout << "GetRotation" << iter.second->GetRotation().ToCMatrix4().ToEuler().ToString() << std::endl;

			// 更新BV的位置
			if (iter.second->mBVHNode)
			{
				iter.second->mBVHNode->mBoundingVolume.mCenter = iter.second->GetPosition();
				iter.second->mBVHNode->UpdateBoundingVolume();
			}
		}
	}

	void PhysicsManager::AddBoundingVolume(const BoundingSphere& boundingVolume, RigidBodyPrimitive* rigidBody)
	{
		if (mBVHRoot)
			mBVHRoot->Insert(boundingVolume, rigidBody);
		else
			mBVHRoot = new BVHNode(nullptr, boundingVolume, rigidBody);
	}
}

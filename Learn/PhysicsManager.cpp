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
			GameScriptManager::Instance().Update();
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

		for (auto& iter : GoToRigidBody)
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
		for (auto& iter : GoToRigidBody)
		{
			//std::cout << rigidbody->forceAccum.ToString() << std::endl;

			iter.second->Integrate(deltaTime);
		}

		// 生成潜在碰撞
		uint32_t potentialContactCount = mBVHRoot->GetPotentialContacts(mPotentialContacts, mMaxPotentialContacts);
		// 从潜在碰撞中检测碰撞
		uint32_t i = 0;
		while (i < potentialContactCount)
		{
			uint32_t collisionCount = CollisionDetector::Detect(
				mPotentialContacts[i].mRigidBodies[0]->mCollisionVolume,
				mPotentialContacts[i].mRigidBodies[1]->mCollisionVolume,
				mCollisionData
			);
			if (collisionCount > 0) {
				GameScriptManager::Instance().ReportCollision(mPotentialContacts[i].mRigidBodies[0], mPotentialContacts[i].mRigidBodies[1]);
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

		for (auto& iter : GoToRigidBody)
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

	void PhysicsLit::PhysicsManager::RemoveBoundingVolume(RigidBodyPrimitive* rigidBody)
	{
		if (!rigidBody || !rigidBody->mBVHNode)
			return;

		BVHNode* node = rigidBody->mBVHNode;

		// 如果是根节点，直接清空整棵树
		if (node == mBVHRoot) {
			delete mBVHRoot;
			mBVHRoot = nullptr;
		}
		else {
			node->Remove();  // 正确方式：由 BVHNode::Remove() 执行结构重组和 delete
		}

		rigidBody->mBVHNode = nullptr;
	}
	void PhysicsManager::CheckImmediateCollision(RigidBodyPrimitive* bullet)
	{
		if (!bullet || !bullet->mCollisionVolume || !bullet->mBVHNode || mBVHRoot == nullptr)
			return;

		std::vector<BVHNode*> candidates;
		mBVHRoot->QueryPotentialContacts(bullet->mBVHNode->mBoundingVolume, candidates);

		for (auto* node : candidates) {
			if (!node || node->mRigidBody == nullptr || node->mRigidBody == bullet)
				continue;

			uint32_t collisionCount = CollisionDetector::Detect(
				bullet->mCollisionVolume,
				node->mRigidBody->mCollisionVolume,
				mCollisionData
			);

			if (collisionCount > 0) {
				GameScriptManager::Instance().ReportCollision(bullet, node->mRigidBody);
			}
		}
	}



}

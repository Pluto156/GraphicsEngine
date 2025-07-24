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

	void PhysicsManager::RemoveGameObject(GameObject* gameobject) {

		auto it = GoToRigidBody.find(gameobject);
		if (it != GoToRigidBody.end()) {
			GameScriptManager::Instance().InvalidateObject(it->second);

			if (it->second) {
				RemoveBoundingVolume(it->second);
				RigidBodyToGO.erase(it->second);
			}
			GoToRigidBody.erase(it);
		}
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

		// 更新刚体的位置和旋转
		for (auto& iter : GoToRigidBody)
		{
			iter.second->Integrate(deltaTime);
		}

		// 生成潜在碰撞
		uint32_t potentialContactCount = mBVHRoot->GetPotentialContacts(mPotentialContacts, mMaxPotentialContacts);

		// 清空当前碰撞数据计数
		mCollisionData->mCurContactCount = 0;

		for (uint32_t i = 0; i < potentialContactCount; ++i)
		{
			auto& contact = mPotentialContacts[i];
			CollisionPrimitive* a = contact.mRigidBodies[0]->mCollisionVolume;
			CollisionPrimitive* b = contact.mRigidBodies[1]->mCollisionVolume;

			if (!CanCollide(a, b))
			{
				continue;
			}

			// 普通碰撞检测，成功则记录碰撞信息
			uint32_t collisionCount = CollisionDetector::Detect(a, b, mCollisionData);
			if (collisionCount > 0)
			{
				GameScriptManager::Instance().ReportCollision(contact.mRigidBodies[0], contact.mRigidBodies[1]);
			}
			
		}

		// 统一处理所有碰撞响应
		mContactResolver->ResolveContacts(mCollisionData->mContactArray, mCollisionData->mCurContactCount, deltaTime);
		for (uint32_t i = 0; i < potentialContactCount; ++i)
		{
			auto& contact = mPotentialContacts[i];
			CollisionPrimitive* a = contact.mRigidBodies[0]->mCollisionVolume;
			CollisionPrimitive* b = contact.mRigidBodies[1]->mCollisionVolume;
			if (!CanCollide(a, b))
			{
				continue;
			}
			/*if (contact.mRigidBodies[0]->GetGameObjectName() == "Car2")
			{
				std::cout<<"After " << contact.mRigidBodies[0]->GetPosition().ToString() << std::endl;

			}*/

			if (a->isTrigger || b->isTrigger)
			{
				// 碰撞检测确认是否真正重叠
				uint32_t triggerCollisionCount = CollisionDetector::Detect(a, b, &CollisionData(1));
				if (triggerCollisionCount > 0) {
					GameScriptManager::Instance().ReportTrigger(contact.mRigidBodies[0], contact.mRigidBodies[1]);
				}
			}
			else
			{
				// 普通碰撞检测，成功则记录碰撞信息
				uint32_t collisionCount = CollisionDetector::Detect(a, b, mCollisionData);
				if (collisionCount > 0)
				{
					GameScriptManager::Instance().ReportCollision(contact.mRigidBodies[0], contact.mRigidBodies[1]);
				}
			}
		}

		
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

		if (bullet->mCollisionVolume->isTrigger)return;

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

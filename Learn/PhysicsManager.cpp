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

		// ��ǰ��Ϸ֡���������������Ŀ��֡����������һ֡���������
		if (deltaFrame <= 0)
			return;
		// ��ǰ��Ϸ֡���������������Ŀ��֡������Ҫ������֡�����ǲ��ܲ�̫�࣬����ᵼ����Ϸ֡�ʽ�һ�����ͣ�����ѭ��Ȼ����
		else if (deltaFrame > 10)
			deltaFrame = 10;

		for (long long i = 0; i < deltaFrame; i++)
		{
			BeginFrame();
			GameObjectManager::Instance().Update();
			UpdatePhysics(TimeManager::fixedDeltaTime);
			EndFrame();
		}

		// ������û�в�֡�����߲��˶���֡���������Ѿ�׷����Ŀ��֡��
		mCurPhyFrame = targetFrame;
	}

	void PhysicsManager::BeginFrame()
	{
		if (mBVHRoot == nullptr)
			return;
		// ������ײ����
		mCollisionData->Reset();

		for (auto& iter : mAllRigidBodyGO)
		{
			// �����������һ֡�ۼƵ���������
			iter.second->ClearAccumulators();
			// ���¸�������һ֡���������
			iter.second->CalculateDerivedData();
		}
	}
	void PhysicsManager::UpdatePhysics(const float deltaTime)
	{
		if (mBVHRoot == nullptr)
			return;
		 //���¸����λ�ú���ת
		for (auto& iter : mAllRigidBodyGO)
		{
			//std::cout << rigidbody->forceAccum.ToString() << std::endl;

			iter.second->Integrate(deltaTime);
		}

		// ����Ǳ����ײ
		uint32_t potentialContactCount = mBVHRoot->GetPotentialContacts(mPotentialContacts, mMaxPotentialContacts);
		//std::cout << potentialContactCount << std::endl;
		// ��Ǳ����ײ�м����ײ
		uint32_t i = 0;
		while (i < potentialContactCount)
		{
			uint32_t collisionCount = CollisionDetector::Detect(
				mPotentialContacts[i].mRigidBodies[0]->mCollisionVolume,
				mPotentialContacts[i].mRigidBodies[1]->mCollisionVolume,
				mCollisionData
			);
			i++;
		}


		// ������ײ
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

			// ����BV��λ��
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

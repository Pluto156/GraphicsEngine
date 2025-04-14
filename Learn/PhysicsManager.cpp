#include "stdafx.h"
#include "PhysicsManager.h"

namespace PhysicsLit
{
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
		//TODO重置碰撞数据

		for (auto& rigidbody : rigidbodys)
		{
			// 清除刚体在上一帧累计的力和力矩
			rigidbody->ClearAccumulator();
			// 更新刚体在这一帧的相关数据
			rigidbody->CalculateDerivedData();
		}
	}
	void PhysicsManager::UpdatePhysics(const float deltaTime)
	{
		if (mBVHRoot == nullptr)
			return;
		// 更新刚体的位置和旋转
		for (auto& rigidbody : rigidbodys)
		{
			//std::cout << rigidbody->forceAccum.ToString() << std::endl;

			rigidbody->Integrate(deltaTime);
		}

		// 生成潜在碰撞
		uint32_t potentialContactCount = mBVHRoot->GetPotentialContacts(mPotentialContacts, mMaxPotentialContacts);
		std::cout << potentialContactCount << std::endl;
		// 从潜在碰撞中检测碰撞


		// 处理关节


		// 处理碰撞
	}
	void PhysicsManager::EndFrame()
	{
		if (mBVHRoot == nullptr)
			return;

		for (auto& iter : rigidbodys)
		{
			// 更新BV的位置
			if (iter->mBVHNode)
			{
				iter->mBVHNode->mBoundingVolume.mCenter = iter->gameObject->transform->GetPosition();
				iter->mBVHNode->UpdateBoundingVolume();
			}
		}
	}
}

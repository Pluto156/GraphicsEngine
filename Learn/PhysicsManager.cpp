#include "stdafx.h"
#include "PhysicsManager.h"
namespace PhysicsLit
{
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
		//TODO������ײ����

		for (auto& rigidbody : rigidbodys)
		{
			// �����������һ֡�ۼƵ���������
			rigidbody->ClearAccumulator();
			// ���¸�������һ֡���������
			rigidbody->CalculateDerivedData();
		}
	}
	void PhysicsManager::UpdatePhysics(const float deltaTime)
	{
		// ���¸����λ�ú���ת
		for (auto& rigidbody : rigidbodys)
		{
			//std::cout << rigidbody->forceAccum.ToString() << std::endl;

			rigidbody->Integrate(deltaTime);
		}

		// ����Ǳ����ײ

		// ��Ǳ����ײ�м����ײ


		// ����ؽ�


		// ������ײ
	}
	void PhysicsManager::EndFrame()
	{

	}
}

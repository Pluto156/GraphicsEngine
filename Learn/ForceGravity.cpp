#include "stdafx.h"
#include "ForceGravity.h"
namespace PhysicsLit
{
	ForceGravity::ForceGravity(const CVector& gravity) : mGravity(gravity)
	{
		mType = ForceGeneratorType::Gravity;
	}

	void ForceGravity::UpdateForce(RigidBody* rigidBody, float duration)
	{
		// ����������޴��ʾ������
		if (rigidBody->IsInfiniteMass())
			return;

		// �������
		rigidBody->AddForce(mGravity * rigidBody->GetMass());
	}
}

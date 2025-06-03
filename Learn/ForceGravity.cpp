#include "stdafx.h"
#include "ForceGravity.h"
namespace PhysicsLit
{
	ForceGravity::ForceGravity(const CVector3& gravity) : mGravity(gravity)
	{
		mType = ForceGeneratorType::Gravity;
	}

	void ForceGravity::UpdateForce(RigidBodyPrimitive* rigidBody, float duration)
	{
		// ����������޴��ʾ������
		if (rigidBody->IsInfiniteMass())
			return;

		// �������
		rigidBody->AddForce(mGravity * rigidBody->GetMass());
	}
}

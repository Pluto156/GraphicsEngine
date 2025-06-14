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
		// 如果质量无限大表示不受力
		if (rigidBody->IsInfiniteMass())
			return;

		// 添加重力
		rigidBody->AddForce(mGravity * rigidBody->GetMass());
	}
}

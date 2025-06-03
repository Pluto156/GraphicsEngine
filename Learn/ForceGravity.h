#pragma once
#include "ForceGenerator.h"
namespace PhysicsLit
{
	class ForceGravity : public ForceGenerator
	{
	public:
		ForceGravity(const CVector3& gravity);

		virtual void UpdateForce(RigidBodyPrimitive* rigidBody, float duration);

	private:
		CVector3 mGravity;
	};
}






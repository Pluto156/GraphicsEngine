#pragma once
#include "ForceGenerator.h"
namespace PhysicsLit
{
	class ForceGravity : public ForceGenerator
	{
	public:
		ForceGravity(const CVector& gravity);

		virtual void UpdateForce(RigidBody* rigidBody, float duration);

	private:
		CVector mGravity;
	};
}






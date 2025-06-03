#pragma once

namespace PhysicsLit
{
	enum class ForceGeneratorType
	{
		None,
		Gravity,
		Spring,
	};

	class RigidBodyPrimitive;
	class ForceGenerator
	{
	public:
		ForceGeneratorType mType;

		virtual ~ForceGenerator() = default;

		virtual void UpdateForce(RigidBodyPrimitive* rigidBody, float duration) = 0;
	};

}



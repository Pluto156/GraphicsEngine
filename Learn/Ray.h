#pragma once
namespace PhysicsLit
{
	class Ray
	{
	public:
		CVector3 mOrigin;
		CVector3 mDirection;

		Ray() = default;
		Ray(const CVector3& origin, const CVector3& direction);

		std::string ToString() const;

		CVector3 GetPoint(float distance) const;
	};
}


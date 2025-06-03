#include "stdafx.h"
#include "Ray.h"
namespace PhysicsLit
{
	Ray::Ray(const CVector3& origin, const CVector3& direction) :
		mOrigin(origin),
		mDirection(direction.GetNormalize())
	{
	}

	std::string Ray::ToString() const
	{
		return "Pos: " + mOrigin.ToString() + " Dir: " + mDirection.ToString();
	}

	CVector3 Ray::GetPoint(float distance) const
	{
		return mOrigin + mDirection * distance;
	}
}
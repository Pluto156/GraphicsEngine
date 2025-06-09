#include "stdafx.h"
#include "CollisionPrimitive.h"
namespace PhysicsLit
{


	CollisionPrimitive::~CollisionPrimitive()
	{
		if (rigidBodyPrimitive && rigidBodyPrimitive->mCollisionVolume == this)
		{
			rigidBodyPrimitive->mCollisionVolume = nullptr;
		}
	}

	void CollisionPrimitive::SynchronizeTransform(const CMatrix4& transform)
	{
		transformMatrix = transform;
	}

	ColliderType CollisionBox::GetType() const
	{
		return ColliderType::Box;
	}

	CMatrix4 CollisionBox::GetInertiaTensor(float mass) const
	{
		// 由于是halfSize，所以要先乘2再平方，简写为*4
		float x2 = mHalfSize.x * mHalfSize.x * 4.0f;
		float y2 = mHalfSize.y * mHalfSize.y * 4.0f;
		float z2 = mHalfSize.z * mHalfSize.z * 4.0f;

		return CMatrix4(
			// 1/12 * m * (y^2 + z^2)
			0.083333f * mass * (y2 + z2), 0.0f, 0.0f,
			// 1/12 * m * (x^2 + z^2)
			0.0f, 0.083333f * mass * (x2 + z2), 0.0f,
			// 1/12 * m * (x^2 + y^2)
			0.0f, 0.0f, 0.083333f * mass * (x2 + y2));
	}

	float CollisionBox::GetHalfProjectedLength(const CVector3& axis) const
	{
		// 先把轴从世界空间变换到Box的局部空间(仅考虑旋转即可)
		CVector3 axisProjected = transformMatrix.GetTranspose().vecMulVector3(axis);
		// 和mHalfSize放到同一个象限内 向量的每个分量取绝对值，确保该向量的每个分量为非负值。这是因为计算碰撞盒在轴上的投影时，我们只关心轴向量的方向，而不关心它的正负，所以对每个分量取绝对值可以保证投影长度是正数。简单来说，这个步骤是为了规范化投影向量。
		axisProjected.Absolutize();
		// 然后计算Box在这个轴上的投影长度
		return mHalfSize.projectLen(axisProjected);
	}

	void CollisionPlane::SynchronizeTransform(const CMatrix4& transform)
	{
		transformMatrix = transform;
		mNormal = transform.GetInverse().GetTranspose().vecMulVector3(mLocalNormal);
	}

	ColliderType CollisionPlane::GetType() const
	{
		return ColliderType::Plane;
	}

	CMatrix4 CollisionPlane::GetInertiaTensor(float mass) const
	{
		// 无限远平面不当作刚体来模拟，惯性张量无意义
		return CMatrix4();
	}

	ColliderType CollisionSphere::GetType() const
	{
		return ColliderType::Sphere;
	}

	CMatrix4 CollisionSphere::GetInertiaTensor(float mass) const
	{
		// 2/5 * m * r^2
		float i = 0.4f * mass * mRadius * mRadius;

		return CMatrix4(
			i, 0.0f, 0.0f,
			0.0f, i, 0.0f,
			0.0f, 0.0f, i);
	}

	void CollisionCircle2D::SynchronizeTransform(const CMatrix4& transform)
	{
		transformMatrix = transform;
		mWorldNormal = transform.GetInverse().GetTranspose().vecMulVector3(mLocalNormal);
	}

	ColliderType CollisionCircle2D::GetType() const
	{
		return ColliderType::Circle2D;
	}

	CMatrix4 CollisionCircle2D::GetInertiaTensor(float mass) const
	{
		// 暂未实现2D碰撞
		return CMatrix4();
	}
}
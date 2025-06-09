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
		// ������halfSize������Ҫ�ȳ�2��ƽ������дΪ*4
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
		// �Ȱ��������ռ�任��Box�ľֲ��ռ�(��������ת����)
		CVector3 axisProjected = transformMatrix.GetTranspose().vecMulVector3(axis);
		// ��mHalfSize�ŵ�ͬһ�������� ������ÿ������ȡ����ֵ��ȷ����������ÿ������Ϊ�Ǹ�ֵ��������Ϊ������ײ�������ϵ�ͶӰʱ������ֻ�����������ķ��򣬶��������������������Զ�ÿ������ȡ����ֵ���Ա�֤ͶӰ����������������˵�����������Ϊ�˹淶��ͶӰ������
		axisProjected.Absolutize();
		// Ȼ�����Box��������ϵ�ͶӰ����
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
		// ����Զƽ�治����������ģ�⣬��������������
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
		// ��δʵ��2D��ײ
		return CMatrix4();
	}
}
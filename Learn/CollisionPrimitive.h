#pragma once
namespace PhysicsLit
{
	class RigidBodyPrimitive;
	class CollisionPrimitive
	{
	public:
		// ��ײ������Ӧ�ĸ���
		RigidBodyPrimitive* rigidBodyPrimitive = nullptr;
		// ��ײ������ڸ����ƫ��(��������ȫ������ʹ�õ�λ���󼴿�)
		CMatrix4 mOffset;
		// ��ײ�����������ı任����
		CMatrix4 transformMatrix;
		// Ħ��ϵ��
		float mFriction = 0.4f;
		// �ص�ϵ��
		float mBounciness = 0.0f;
		// Ħ��ϵ����Ϸ�ʽ
		CombineType mFrictionCombine = CombineType::Average;
		// �ص�ϵ����Ϸ�ʽ
		CombineType mBounceCombine = CombineType::Average;

		virtual ~CollisionPrimitive();

		virtual void SynchronizeTransform(const CMatrix4& transform);

		// ��ȡ��ײ������
		virtual ColliderType GetType() const = 0;
		// ���ɶ�Ӧ��״�ľ��ʹ�������
		virtual CMatrix4 GetInertiaTensor(float mass) const = 0;
	};

	class CollisionBox : public CollisionPrimitive
	{
	public:
		// Box��3�����ϵĳ��Ȱ뾶
		CVector3 mHalfSize;

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;

		// ��ȡBoxͶӰ��ĳ�����ϵĳ��ȵ�һ��
		float GetHalfProjectedLength(const CVector3& axis) const;
	};

	// ��ײƽ��(ͨ����ʾ������һ�������ƶ���ǽ����棬�����Ǹ���)
	class CollisionPlane : public CollisionPrimitive
	{
	public:
		// ��ײƽ�淨��(World)
		CVector3 mNormal;
		// ��ײƽ�淨��(Local)
		CVector3 mLocalNormal;
		// ��ײƽ�浽ԭ��ľ���
		float mDistance;

		virtual void SynchronizeTransform(const CMatrix4& transform);

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;
	};

	class CollisionSphere : public CollisionPrimitive
	{
	public:
		// ��ײ��뾶
		float mRadius;

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;
	};

	struct CollisionVolume
	{
		// ��ײ������
		ColliderType mType;
		// ��ײ��ָ��
		union
		{
			CollisionBox* mBox;
			CollisionPlane* mPlane;
			CollisionSphere* mSphere;
		};
	};

	class CollisionCircle2D : public CollisionPrimitive
	{
	public:
		CVector3 mLocalNormal;
		CVector3 mWorldNormal;
		float mRadius;

		virtual void SynchronizeTransform(const CMatrix4& transform);

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;
	};
}


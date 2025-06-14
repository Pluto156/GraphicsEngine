#pragma once
#include "Collider.h"
namespace PhysicsLit
{
	class RigidBodyPrimitive;
	class CollisionPrimitive
	{
	public:
		// 碰撞体所对应的刚体
		RigidBodyPrimitive* rigidBodyPrimitive = nullptr;
		// 碰撞体相对于刚体的偏移(若二者完全对齐则使用单位矩阵即可)
		CMatrix4 mOffset;
		// 碰撞体相对于世界的变换矩阵
		CMatrix4 transformMatrix;
		// 摩擦系数
		float mFriction = 0.4f;
		// 回弹系数
		float mBounciness = 0.0f;
		// 摩擦系数组合方式
		CombineType mFrictionCombine = CombineType::Average;
		// 回弹系数组合方式
		CombineType mBounceCombine = CombineType::Average;
		virtual ~CollisionPrimitive();

		virtual void SynchronizeTransform(const CMatrix4& transform);

		// 获取碰撞体类型
		virtual ColliderType GetType() const = 0;
		// 生成对应形状的均质惯性张量
		virtual CMatrix4 GetInertiaTensor(float mass) const = 0;
		Collider* colliderComponent;
	};

	class CollisionBox : public CollisionPrimitive
	{
	public:
		// Box在3个轴上的长度半径
		CVector3 mHalfSize;

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;

		// 获取Box投影到某个轴上的长度的一半
		float GetHalfProjectedLength(const CVector3& axis) const;
	};

	// 碰撞平面(通常表示场景中一个不可移动的墙或地面，而不是刚体)
	class CollisionPlane : public CollisionPrimitive
	{
	public:
		// 碰撞平面法线(World)
		CVector3 mNormal;
		// 碰撞平面法线(Local)
		CVector3 mLocalNormal;
		// 碰撞平面到原点的距离
		float mDistance;

		virtual void SynchronizeTransform(const CMatrix4& transform);

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;
	};

	class CollisionSphere : public CollisionPrimitive
	{
	public:
		// 碰撞球半径
		float mRadius;

		virtual ColliderType GetType() const;
		virtual CMatrix4 GetInertiaTensor(float mass) const;
	};

	struct CollisionVolume
	{
		// 碰撞体类型
		ColliderType mType;
		// 碰撞体指针
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


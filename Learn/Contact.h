#pragma once
namespace PhysicsLit
{
	class RigidBodyPrimitive;
	/// <summary>
	/// 表示 两个刚体的碰撞，并通过相关的方法来计算碰撞后的反应，包括碰撞后的速度、位置修正，以及摩擦力、恢复力的计算。它是一个碰撞解析器中非常重要的组成部分，通常在物理引擎中用于处理 碰撞响应。
	/// </summary>
	class Contact
	{
		friend class ContactResolver;
	public:
		// 碰撞点坐标
		CVector3 mContactPoint;
		// 碰撞法线
		CVector3 mContactNormal;
		// 碰撞深度(有碰撞或相交时是正数)
		float mPenetration = 0.0f;
		// 恢复系数
		float mRestitution = 0.0f;
		// 摩擦系数
		float mFriction = 0.0f;

		Contact(RigidBodyPrimitive* rigidBody1 = nullptr, RigidBodyPrimitive* rigidBody2 = nullptr);

		void SetRigidBodies(RigidBodyPrimitive* rigidBody1, RigidBodyPrimitive* rigidBody2);

	private:
		// 碰撞所对应的刚体
		RigidBodyPrimitive* mRigidBodies[2];
		// 从碰撞坐标系到世界坐标系的旋转矩阵
		CMatrix3 mContactToWorld;

		// 碰撞点相对于两个刚体的位置
		CVector3 mRelativeContactPosition[2];
		// 碰撞点的闭合速度(即两个对象相互接近的速度，碰撞空间)
		CVector3 mContactVelocity;
		// 当前碰撞所产生的期望速度变化量(闭合速度在碰撞法线上的变化量)
		float mDesiredDeltaVelocity = 0.0f;

		// 动态更新碰撞系数（摩擦和恢复系数），以确保碰撞响应计算时可以使用正确的物理参数。这些系数对物理引擎中的碰撞反应至关重要，决定了物体碰撞后的行为，如反弹、滑动等
		void UpdateCoefficient();
		// 交换两个刚体，同时将碰撞法线取反(但是不会更新其它相关变量，如果需要更新手动调用UpdateInternalDatas)
		void SwapRigidBodies();
		// 更新碰撞中的刚体状态，如果其中一个刚体是Awake，另一个也必须Awake
		void MatchAwakeState();

		// 处理碰撞穿透，通过移动和旋转两个刚体，尽量让两个对象不交叉
		// 然后通过前两个指针参数返回处理信息
		// PS: 此函数虽然是在处理相交，也会改变刚体的位置和旋转状态，但是并不会直接改变mPenetration，而是由调用的地方去调整mPenetration
		void ResolvePenetration(CVector3 linearChange[2], CVector3 angularChange[2], float penetration);
		// 处理碰撞中的速度变化，计算两个刚体的速度变化量，然后通过参数返回
		void ResolveVelocityChange(CVector3 linearVelocityChange[2], CVector3 angularVelocityChange[2]);

		// 计算有摩擦力情况下的冲量
		CVector3 CalculateFrictionImpulse(CMatrix3* inverseInertiaTensor);
		// 计算无摩擦力情况下的冲量
		CVector3 CalculateFrictionlessImpulse(CMatrix3* inverseInertiaTensor);

		// 更新当前碰撞点的各项内部数据
		void UpdateInternalDatas(float duration);
		// 更新碰撞坐标系到世界坐标系的旋转矩阵
		void UpdateOrthogonalBasis();
		// 计算当前碰撞所产生的期望速度变化量(闭合速度)
		void UpdateDesiredDeltaVelocity(float duration);

		// 计算第index个刚体相对于碰撞点的速度
		CVector3 CalculateLocalVelocity(uint32_t index, float duration);
	};
}


#pragma once
#include "PhysicsLitEnumStruct.h"
namespace PhysicsLit
{
	class BVHNode;
	class ForceGenerator;
	class CollisionPrimitive;
	class RigidBodyPrimitive
	{
	public:
		// 对应的BVH节点
		BVHNode* mBVHNode = nullptr;
		// 对应的碰撞体
		CollisionPrimitive* mCollisionVolume = nullptr;

		RigidBodyPrimitive() {};
		~RigidBodyPrimitive();

		// 更新刚体的位置和旋转
		void Integrate(float duration);
		// 计算需要跟随刚体变化而变化的相关数据
		void CalculateDerivedData();
		// 直接在质心添加一个作用力(世界坐标系)
		void AddForce(const CVector3& force);
		// 在某个点(世界坐标系)上添加作用力(世界坐标系)
		void AddForceAtPoint(const CVector3& force, const CVector3& point);
		// 在某个点(局部坐标系)上添加作用力(世界坐标系)
		void AddForceAtLocalPoint(const CVector3& force, const CVector3& point);
		// 添加一个力矩(世界坐标系)
		void AddTorque(const CVector3& torque);
		// 清除当前累计的作用力和力矩
		void ClearAccumulators();
		// 判断质量是否为无穷大
		bool IsInfiniteMass() const;

		void SetAwake(bool awake);
		bool GetAwake() const;
		void SetCanSleep(bool canSleep);
		bool GetCanSleep() const;

		void SetTransform(const CMatrix4& transform);
		const CMatrix4& GetTransform() const;

		void AddForceGenerator(ForceGenerator* generator);

		// ---------- 线性运动 ----------

		void SetMass(float mass);
		float GetMass() const;
		void SetInverseMass(float inverseMass);
		float GetInverseMass() const;

		void SetLinearDamping(float damping);
		float GetLinearDamping() const;

		void SetPosition(const CVector3& position);
		void GetPosition(CVector3& position) const;
		CVector3 GetPosition() const;


		void SetVelocity(const CVector3& velocity);
		void GetVelocity(CVector3& velocity) const;
		CVector3 GetVelocity() const;
		void AddVelocity(const CVector3& deltaVelocity);

		void SetAcceleration(const CVector3& acceleration);
		void GetAcceleration(CVector3& acceleration) const;
		CVector3 GetAcceleration() const;

		void GetLastAcceleration(CVector3& acceleration) const;
		CVector3 GetLastAcceleration() const;

		// ---------- 旋转运动 ----------

		void SetInertiaTensor(const CMatrix3& inertiaTensor);
		void GetInertiaTensor(CMatrix3& inertiaTensor) const;
		CMatrix3 GetInertiaTensor() const;
		void SetInverseInertiaTensor(const CMatrix3& inverseInertiaTensor);
		void GetInverseInertiaTensor(CMatrix3& inverseInertiaTensor) const;
		CMatrix3 GetInverseInertiaTensor() const;
		void GetInverseInertiaTensorWorld(CMatrix3& inverseInertiaTensor) const;
		CMatrix3 GetInverseInertiaTensorWorld() const;

		void SetAngularDamping(float damping);
		float GetAngularDamping() const;

		void SetRotation(const CQuaternion& rotation);
		void GetRotation(CQuaternion& rotation) const;
		CQuaternion GetRotation() const;

		void SetAngularVelocity(const CVector3& angularVelocity);
		void GetAngularVelocity(CVector3& angularVelocity) const;
		CVector3 GetAngularVelocity() const;
		void AddAngularVelocity(const CVector3& deltaAngularVelocity);

	private:
		// 此刚体上的作用力生成器列表
		std::vector<ForceGenerator*> mForceGenerators;

		// 质量的倒数(0代表质量无穷大，无视任何作用力)
		float mInverseMass = 1.0f;
		// 线性运动阻尼系数(1表示无阻尼)
		float mLinearDamping = 1.0f;
		// 位置
		CVector3 mPosition;
		// 速度
		CVector3 mVelocity;
		// 加速度
		CVector3 mAcceleration;
		// 上一帧的加速度
		CVector3 mLastAcceleration;
		// 累积作用力
		CVector3 mForceAccum;

		// Local惯性张量(以矩阵形式表达,存储逆矩阵方便计算)
		CMatrix3 mLocalInverseInertiaTensor;
		// World惯性张量(以矩阵形式表达,存储逆矩阵方便计算)
		CMatrix3 mWorldInverseInertiaTensor;
		// 旋转运动阻尼系数(1表示无阻尼)
		float mAngularDamping = 0.95f;
		// 旋转
		CQuaternion mRotation;
		// 角速度
		CVector3 mAngularVelocity;
		// 累积力矩
		CVector3 mTorqueAccum;

		// local到world空间的变换
		CMatrix4 mTransform;

		// 睡眠状态的刚体不进行物理更新
		bool mIsAwake = true;
		// 刚体是否可以进入睡眠状态
		// 一般默认为true，某些特殊对象，比如玩家操控的，可能随时手动产生作用力的对象可以设置为false
		bool mCanSleep = true;
		// 刚体当前的运动量，包含线性速度和角速度，无实际物理意义，仅用于评估运动状态
		float mMotion = SleepMotionEpsilon * 2.0f;

		void IntegrateForceGenerators(float duration);
		void UpdateTransform();
		void UpdateWorldInertiaTensor();
	};
}



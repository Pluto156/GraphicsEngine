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
		// ��Ӧ��BVH�ڵ�
		BVHNode* mBVHNode = nullptr;
		// ��Ӧ����ײ��
		CollisionPrimitive* mCollisionVolume = nullptr;

		RigidBodyPrimitive() {};
		~RigidBodyPrimitive();

		// ���¸����λ�ú���ת
		void Integrate(float duration);
		// ������Ҫ�������仯���仯���������
		void CalculateDerivedData();
		// ֱ�����������һ��������(��������ϵ)
		void AddForce(const CVector3& force);
		// ��ĳ����(��������ϵ)�����������(��������ϵ)
		void AddForceAtPoint(const CVector3& force, const CVector3& point);
		// ��ĳ����(�ֲ�����ϵ)�����������(��������ϵ)
		void AddForceAtLocalPoint(const CVector3& force, const CVector3& point);
		// ���һ������(��������ϵ)
		void AddTorque(const CVector3& torque);
		// �����ǰ�ۼƵ�������������
		void ClearAccumulators();
		// �ж������Ƿ�Ϊ�����
		bool IsInfiniteMass() const;

		void SetAwake(bool awake);
		bool GetAwake() const;
		void SetCanSleep(bool canSleep);
		bool GetCanSleep() const;

		void SetTransform(const CMatrix4& transform);
		const CMatrix4& GetTransform() const;

		void AddForceGenerator(ForceGenerator* generator);

		// ---------- �����˶� ----------

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

		// ---------- ��ת�˶� ----------

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
		// �˸����ϵ��������������б�
		std::vector<ForceGenerator*> mForceGenerators;

		// �����ĵ���(0������������������κ�������)
		float mInverseMass = 1.0f;
		// �����˶�����ϵ��(1��ʾ������)
		float mLinearDamping = 1.0f;
		// λ��
		CVector3 mPosition;
		// �ٶ�
		CVector3 mVelocity;
		// ���ٶ�
		CVector3 mAcceleration;
		// ��һ֡�ļ��ٶ�
		CVector3 mLastAcceleration;
		// �ۻ�������
		CVector3 mForceAccum;

		// Local��������(�Ծ�����ʽ���,�洢����󷽱����)
		CMatrix3 mLocalInverseInertiaTensor;
		// World��������(�Ծ�����ʽ���,�洢����󷽱����)
		CMatrix3 mWorldInverseInertiaTensor;
		// ��ת�˶�����ϵ��(1��ʾ������)
		float mAngularDamping = 0.95f;
		// ��ת
		CQuaternion mRotation;
		// ���ٶ�
		CVector3 mAngularVelocity;
		// �ۻ�����
		CVector3 mTorqueAccum;

		// local��world�ռ�ı任
		CMatrix4 mTransform;

		// ˯��״̬�ĸ��岻�����������
		bool mIsAwake = true;
		// �����Ƿ���Խ���˯��״̬
		// һ��Ĭ��Ϊtrue��ĳЩ������󣬱�����Ҳٿصģ�������ʱ�ֶ������������Ķ����������Ϊfalse
		bool mCanSleep = true;
		// ���嵱ǰ���˶��������������ٶȺͽ��ٶȣ���ʵ���������壬�����������˶�״̬
		float mMotion = SleepMotionEpsilon * 2.0f;

		void IntegrateForceGenerators(float duration);
		void UpdateTransform();
		void UpdateWorldInertiaTensor();
	};
}



#include "stdafx.h"
#include "RigidBodyPrimitive.h"
#include "ForceGravity.h"
#include "CollisionPrimitive.h"
namespace PhysicsLit
{
	RigidBodyPrimitive::~RigidBodyPrimitive()
	{
		if (mCollisionVolume && mCollisionVolume->rigidBodyPrimitive == this)
		{
			mCollisionVolume->rigidBodyPrimitive = nullptr;
		}

		for (auto generator : mForceGenerators)
		{
			if (generator->mType == ForceGeneratorType::Gravity)
				delete static_cast<ForceGravity*>(generator);
			else if (generator->mType == ForceGeneratorType::Spring)
			{
				//delete static_cast<FGSpring*>(generator);

			}
			else
			{
				//Debug::LogWarning("Delete unknown force generator type, there may be a memory leak.");
				delete generator;
			}
		}
	}

	void RigidBodyPrimitive::Integrate(float duration)
	{
		if (!mIsAwake)
			return;

		// ���㵱ǰ�������������ĺ���
		IntegrateForceGenerators(duration);

		// ͨ������������ٶ�
		mLastAcceleration = mAcceleration;
		mLastAcceleration += mForceAccum * mInverseMass;

		// ͨ�������ؼ���Ǽ��ٶ�
		CVector3 angularAcceleration = mWorldInverseInertiaTensor * mTorqueAccum;

		// ���ݼ��ٶȸ����ٶ�
		mVelocity += mLastAcceleration * duration;

		// ���ݽǼ��ٶȸ��½��ٶ�
		mAngularVelocity += angularAcceleration * duration;

		// ʩ������Ч��
		mVelocity *= pow(mLinearDamping, duration);
		mAngularVelocity *= pow(mAngularDamping, duration);

		// �����ٶ��ƶ�λ��
		mPosition += mVelocity * duration;

		// ���ݽ��ٶȸ�����ת״̬
		mRotation.Rotate(mAngularVelocity, duration);

		// ������������
		CalculateDerivedData();

		// ����ۻ���������������
		ClearAccumulators();

		if (mCanSleep)
		{
			// ���㵱ǰ���˶�ֵ
			float currentMotion = Math::Dot(mVelocity, mVelocity) + Math::Dot(mAngularVelocity, mAngularVelocity);

			// ��֮ǰ��֡���˶�ֵ�������ۼ�ƽ��
			float bias = powf(0.5f, duration);
			mMotion = bias * mMotion + (1 - bias) * currentMotion;

			// ����ۼ�ƽ���˶�ֵ̫С�˾͵�����̬���󣬽�������״̬����Լ����Ҫ���������
			if (mMotion < SleepMotionEpsilon)
				SetAwake(false);
			// ���ۼ��˶�ֵ��һ�����ֵ���ƣ����һ�������ƶ�����ͻȻͣ�������ᵼ��mMotion�ܴ�Ȼ����Ҫ�ܶ�֡���ܰ�����ƽ��ֵ������
			// ����һ��ʵ���Ѿ���ֹ������Ҫ�Ⱥܾò��ܽ�������״̬�������������ֵ̫���˽�������
			else if (mMotion > 10 * SleepMotionEpsilon)
				mMotion = 10 * SleepMotionEpsilon;
		}
	}

	void RigidBodyPrimitive::CalculateDerivedData()
	{
		UpdateTransform();
		UpdateWorldInertiaTensor();
	}

	void RigidBodyPrimitive::UpdateTransform()
	{
		mTransform = mRotation.ToCMatrix4();
		mTransform = Math::Translate(mTransform, mPosition);

		// �������ײ�壬ͬ����ײ���Transform
		if (mCollisionVolume)
			mCollisionVolume->transformMatrix = mTransform;
	}

	void RigidBodyPrimitive::UpdateWorldInertiaTensor()
	{
		// ���ݵ�ǰ��transform��Ϣ�����¼�����������ϵ�µĹ�������
		CMatrix3 rot(mTransform);
		// ���������ı任��ʽΪ: I' = R * I * R^T��ֻ��Ҫ������ת�任����
		mWorldInverseInertiaTensor = rot * mLocalInverseInertiaTensor * Math::Transpose(rot);
	}

	void RigidBodyPrimitive::AddForce(const CVector3& force)
	{
		mForceAccum += force;
		mIsAwake = true;
	}

	void RigidBodyPrimitive::AddForceAtPoint(const CVector3& force, const CVector3& point)
	{
		// ��������ĵ�λ��
		auto pos = point - mPosition;

		mForceAccum += force;
		mTorqueAccum += Math::Cross(pos, force);

		mIsAwake = true;
	}

	void RigidBodyPrimitive::AddForceAtLocalPoint(const CVector3& force, const CVector3& point)
	{
		AddForceAtPoint(force, mTransform * CVector4(point, 1.0f));
	}

	void RigidBodyPrimitive::AddTorque(const CVector3& torque)
	{
		mTorqueAccum += torque;
		mIsAwake = true;
	}

	void RigidBodyPrimitive::ClearAccumulators()
	{
		mForceAccum.Clear();
		mTorqueAccum.Clear();
	}

	bool RigidBodyPrimitive::IsInfiniteMass() const
	{
		return mInverseMass <= FLT_EPSILON;
	}

	void RigidBodyPrimitive::SetAwake(bool awake)
	{
		if (awake)
		{
			mIsAwake = true;

			// ���Ѻ����һ���˶�������ֹ�����̽���˯��״̬
			mMotion = SleepMotionEpsilon * 2.0f;
		}
		else
		{
			mIsAwake = false;

			// ����״̬�ĸ��岻Ӧ�����κ��ٶ�
			mVelocity.Clear();
			mAngularVelocity.Clear();
		}
	}

	bool RigidBodyPrimitive::GetAwake() const
	{
		return mIsAwake;
	}

	void RigidBodyPrimitive::SetCanSleep(bool canSleep)
	{
		mCanSleep = canSleep;

		if (!canSleep && !mIsAwake)
			SetAwake(true);
	}

	bool RigidBodyPrimitive::GetCanSleep() const
	{
		return mCanSleep;
	}

	const CMatrix4& RigidBodyPrimitive::GetTransform() const
	{
		return mTransform;
	}

	void RigidBodyPrimitive::AddForceGenerator(ForceGenerator* generator)
	{
		mForceGenerators.push_back(generator);
	}

	void RigidBodyPrimitive::IntegrateForceGenerators(float duration)
	{
		for (auto generator : mForceGenerators)
		{
			generator->UpdateForce(this, duration);
		}
	}

	void RigidBodyPrimitive::SetMass(float mass)
	{
		if (mass <= 0.0f)
		{
			// �����������0����������쳣������������Ϊ�����
			//Debug::LogError("Mass must be greater than zero.");
			mInverseMass = 0.0f;
		}
		else
		{
			mInverseMass = 1.0f / mass;
		}
	}

	float RigidBodyPrimitive::GetMass() const
	{
		return 1.0f / mInverseMass;
	}

	void RigidBodyPrimitive::SetInverseMass(float inverseMass)
	{
		mInverseMass = inverseMass;
	}

	float RigidBodyPrimitive::GetInverseMass() const
	{
		return mInverseMass;
	}

	void RigidBodyPrimitive::SetLinearDamping(float damping)
	{
		mLinearDamping = damping;
	}

	float RigidBodyPrimitive::GetLinearDamping() const
	{
		return mLinearDamping;
	}

	void RigidBodyPrimitive::SetPosition(const CVector3& position)
	{
		mPosition = position;
	}

	void RigidBodyPrimitive::GetPosition(CVector3& position) const
	{
		position = mPosition;
	}

	CVector3 RigidBodyPrimitive::GetPosition() const
	{
		return mPosition;
	}

	void RigidBodyPrimitive::SetTransform(const CMatrix4& transform)
	{
		this->mTransform = transform;
		this->SetRotation(transform.ToQuaternion());
		this->SetPosition(transform.GetColumn(3));
	}

	void RigidBodyPrimitive::SetVelocity(const CVector3& velocity)
	{
		mVelocity = velocity;
	}

	void RigidBodyPrimitive::GetVelocity(CVector3& velocity) const
	{
		velocity = mVelocity;
	}

	CVector3 RigidBodyPrimitive::GetVelocity() const
	{
		return mVelocity;
	}

	void RigidBodyPrimitive::AddVelocity(const CVector3& deltaVelocity)
	{
		mVelocity += deltaVelocity;
	}

	void RigidBodyPrimitive::SetAcceleration(const CVector3& acceleration)
	{
		mAcceleration = acceleration;
	}

	void RigidBodyPrimitive::GetAcceleration(CVector3& acceleration) const
	{
		acceleration = mAcceleration;
	}

	CVector3 RigidBodyPrimitive::GetAcceleration() const
	{
		return mAcceleration;
	}

	void RigidBodyPrimitive::GetLastAcceleration(CVector3& acceleration) const
	{
		acceleration = mLastAcceleration;
	}

	CVector3 RigidBodyPrimitive::GetLastAcceleration() const
	{
		return mLastAcceleration;
	}

	void RigidBodyPrimitive::SetInertiaTensor(const CMatrix3& inertiaTensor)
	{
		mLocalInverseInertiaTensor = Math::Inverse(inertiaTensor);
	}

	void RigidBodyPrimitive::GetInertiaTensor(CMatrix3& inertiaTensor) const
	{
		inertiaTensor = Math::Inverse(mLocalInverseInertiaTensor);
	}

	CMatrix3 RigidBodyPrimitive::GetInertiaTensor() const
	{
		return Math::Inverse(mLocalInverseInertiaTensor);
	}

	void RigidBodyPrimitive::SetInverseInertiaTensor(const CMatrix3& inverseInertiaTensor)
	{
		mLocalInverseInertiaTensor = inverseInertiaTensor;
	}

	void RigidBodyPrimitive::GetInverseInertiaTensor(CMatrix3& inverseInertiaTensor) const
	{
		inverseInertiaTensor = mLocalInverseInertiaTensor;
	}

	CMatrix3 RigidBodyPrimitive::GetInverseInertiaTensor() const
	{
		return mLocalInverseInertiaTensor;
	}

	void RigidBodyPrimitive::GetInverseInertiaTensorWorld(CMatrix3& inverseInertiaTensor) const
	{
		inverseInertiaTensor = mWorldInverseInertiaTensor;
	}

	CMatrix3 RigidBodyPrimitive::GetInverseInertiaTensorWorld() const
	{
		return mWorldInverseInertiaTensor;
	}

	void RigidBodyPrimitive::SetAngularDamping(float damping)
	{
		mAngularDamping = damping;
	}

	float RigidBodyPrimitive::GetAngularDamping() const
	{
		return mAngularDamping;
	}

	void RigidBodyPrimitive::SetRotation(const CQuaternion& rotation)
	{
		mRotation = rotation;
	}

	void RigidBodyPrimitive::GetRotation(CQuaternion& rotation) const
	{
		rotation = mRotation;
	}

	CQuaternion RigidBodyPrimitive::GetRotation() const
	{
		return mRotation;
	}

	void RigidBodyPrimitive::SetAngularVelocity(const CVector3& angularVelocity)
	{
		mAngularVelocity = angularVelocity;
	}

	void RigidBodyPrimitive::GetAngularVelocity(CVector3& angularVelocity) const
	{
		angularVelocity = mAngularVelocity;
	}

	CVector3 RigidBodyPrimitive::GetAngularVelocity() const
	{
		return mAngularVelocity;
	}

	void RigidBodyPrimitive::AddAngularVelocity(const CVector3& deltaAngularVelocity)
	{
		mAngularVelocity += deltaAngularVelocity;
	}
}


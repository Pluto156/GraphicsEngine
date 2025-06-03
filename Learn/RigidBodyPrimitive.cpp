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

		// 计算当前作用力生成器的合力
		IntegrateForceGenerators(duration);

		// 通过合力计算加速度
		mLastAcceleration = mAcceleration;
		mLastAcceleration += mForceAccum * mInverseMass;

		// 通过合力矩计算角加速度
		CVector3 angularAcceleration = mWorldInverseInertiaTensor * mTorqueAccum;

		// 根据加速度更新速度
		mVelocity += mLastAcceleration * duration;

		// 根据角加速度更新角速度
		mAngularVelocity += angularAcceleration * duration;

		// 施加阻尼效果
		mVelocity *= pow(mLinearDamping, duration);
		mAngularVelocity *= pow(mAngularDamping, duration);

		// 根据速度移动位置
		mPosition += mVelocity * duration;

		// 根据角速度更新旋转状态
		mRotation.Rotate(mAngularVelocity, duration);

		// 更新其它数据
		CalculateDerivedData();

		// 清除累积的作用力和力矩
		ClearAccumulators();

		if (mCanSleep)
		{
			// 计算当前的运动值
			float currentMotion = Math::Dot(mVelocity, mVelocity) + Math::Dot(mAngularVelocity, mAngularVelocity);

			// 与之前的帧的运动值数据做累计平均
			float bias = powf(0.5f, duration);
			mMotion = bias * mMotion + (1 - bias) * currentMotion;

			// 如果累计平均运动值太小了就当作静态对象，进入休眠状态，节约不必要的物理计算
			if (mMotion < SleepMotionEpsilon)
				SetAwake(false);
			// 对累计运动值做一个最大值限制，如果一个高速移动物体突然停下来，会导致mMotion很大，然后需要很多帧才能把数据平均值拉下来
			// 导致一个实际已经静止的物体要等很久才能进入休眠状态，所以这里如果值太大了进行限制
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

		// 如果有碰撞体，同步碰撞体的Transform
		if (mCollisionVolume)
			mCollisionVolume->transformMatrix = mTransform;
	}

	void RigidBodyPrimitive::UpdateWorldInertiaTensor()
	{
		// 根据当前的transform信息，重新计算世界坐标系下的惯性张量
		CMatrix3 rot(mTransform);
		// 惯性张量的变换公式为: I' = R * I * R^T，只需要考虑旋转变换即可
		mWorldInverseInertiaTensor = rot * mLocalInverseInertiaTensor * Math::Transpose(rot);
	}

	void RigidBodyPrimitive::AddForce(const CVector3& force)
	{
		mForceAccum += force;
		mIsAwake = true;
	}

	void RigidBodyPrimitive::AddForceAtPoint(const CVector3& force, const CVector3& point)
	{
		// 相对于质心的位置
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

			// 唤醒后添加一点运动量，防止又立刻进入睡眠状态
			mMotion = SleepMotionEpsilon * 2.0f;
		}
		else
		{
			mIsAwake = false;

			// 休眠状态的刚体不应该有任何速度
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
			// 质量必须大于0，否则输出异常并将质量设置为无穷大
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


#include "stdafx.h"
#include "Rigidbody.h"
#include "ForceGravity.h"

namespace PhysicsLit
{
	void RigidBody::CalculateDerivedData()
	{
		transformMatrix = gameObject->transform->GetWorldTransformMatrix();
		inverseInertiaTensorWorld = transformMatrix * inverseInertiaTensor * transformMatrix.GetTranspose();

	}

	void RigidBody::SetInertiaTensor(const CMatrix& inertiaTensor)
	{
		CMatrix t = inertiaTensor;
		inverseInertiaTensor = t.GetInverse();
	}

	void RigidBody::AddForce(const CVector& force)
	{
		forceAccum += force;
	}

	void RigidBody::AddForceAtBodyPoint(const CVector& force, const CVector& point)
	{
		CVector pt = GetPointInWorldSpace(point);
		AddForceAtPoint(force, pt);
	}

	void RigidBody::AddForceAtPoint(const CVector& force, const CVector& point)
	{
		CVector pt = point - gameObject->transform->position;
		forceAccum += force;
		torqueAccum += pt.crossMul(force);
	}

	void RigidBody::AddForceGenerator(ForceGenerator* generator)
	{
		mForceGenerators.push_back(generator);
	}

	void RigidBody::IntegrateForceGenerators(float duration)
	{
		for (auto generator : mForceGenerators)
		{
			generator->UpdateForce(this, duration);
		}
	}

	void RigidBody::ClearAccumulator()
	{
		forceAccum.Clear();
		torqueAccum.Clear();
	}

	void RigidBody::Integrate(float duration)
	{
		IntegrateForceGenerators(duration);
		CVector lastFrameAcceleration = acceleration;
		lastFrameAcceleration += forceAccum * inverseMass;

		CVector angularAcceleration = inverseInertiaTensorWorld.vecMul(torqueAccum);

		velocity += lastFrameAcceleration * duration;
		angularVelocity += angularAcceleration * duration;

		velocity *= powf(linearDamping, duration);
		angularVelocity *= powf(angularDamping, duration);

		gameObject->transform->SetPositionDelta(velocity * duration);
		gameObject->transform->quaternion.AddScaledVector(angularVelocity, duration);
		gameObject->transform->SetQuaternion(gameObject->transform->quaternion);

		CalculateDerivedData();
		ClearAccumulator();
	}

	CVector RigidBody::GetPointInWorldSpace(const CVector& point) const
	{
		return gameObject->transform->rotation.vecMul(point);
	}

	bool RigidBody::hasFiniteMass() const
	{
		return inverseMass >= 0.0f;
	}

	float RigidBody::GetMass() const
	{
		if (inverseMass == 0) {
			return FLT_MAX;
		}
		else {
			return ((float)1.0) / inverseMass;
		}
	}

	void RigidBody::SetMass(float mass)
	{
		RigidBody::inverseMass = 1.f / mass;
	}

	bool RigidBody::IsInfiniteMass() const
	{
		return inverseMass <= FLT_EPSILON;
	}

	void RigidBody::SetVelocity(const CVector& velocity)
	{
		this->velocity = velocity;
	}

	CVector RigidBody::GetVelocity() const
	{
		return velocity;
	}

	void RigidBody::SetAngularVelocity(const CVector& angularVelocity)
	{
		this->angularVelocity = angularVelocity;
	}

	CVector RigidBody::GetAngularVelocity() const
	{
		return angularVelocity;
	}

	RigidBody::~RigidBody()
	{
		PhysicsManager::Instance().Unregister(this->gameObject);

		for (auto generator : mForceGenerators)
		{
			if (generator->mType == ForceGeneratorType::Gravity)
				delete static_cast<ForceGravity*>(generator);
			else
			{
				//Debug::LogWarning("Delete unknown force generator type, there may be a memory leak.");
				delete generator;
			}
		}
	}
}

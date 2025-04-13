#include "stdafx.h"
#include "Rigidbody.h"

void RigidBody::CalculateDerivedData()
{
	transformMatrix = gameObject->transform->GetWorldTransformMatrix();
	inverseInertiaTensorWorld = transformMatrix * inverseInertiaTensor * transformMatrix.GetTranspose();

}

void RigidBody::SetInertiaTensor(const CMatrix& inertiaTensor)
{
	CMatrix t = inertiaTensor;
	inverseInertiaTensor= t.GetInverse();
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

void RigidBody::ClearAccumulator()
{
	forceAccum.Clear();
	torqueAccum.Clear();
}

void RigidBody::Integrate(float duration)
{
	CVector lastFrameAcceleration = acceleration;
	lastFrameAcceleration += forceAccum * inverseMass;

	CVector angularAcceleration = inverseInertiaTensorWorld.vecMul(torqueAccum);

	velocity += lastFrameAcceleration * duration;
	angularVelocity += angularAcceleration * duration;

	velocity *= powf(linearDamping, duration);
	angularVelocity *= powf(angularDamping, duration);

	gameObject->transform->SetPositionDelta(velocity * duration);
	gameObject->transform->quaternion.AddScaledVector(angularVelocity,duration);

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

RigidBody::~RigidBody()
{
	PhysicsManager::Instance().Unregister(this->gameObject);
}
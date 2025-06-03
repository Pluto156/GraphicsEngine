#include "stdafx.h"
#include "RigidBody.h"
ComponentType RigidBody::GetType()
{
	return ComponentType::RigidBody;
}
void RigidBody::Start()
{
	rigidBodyPrimitive = new PhysicsLit::RigidBodyPrimitive();
	rigidBodyPrimitive->SetPosition(gameObject->transform->GetPosition());
	rigidBodyPrimitive->SetRotation(gameObject->transform->GetQuaternion());

}
RigidBody::RigidBody()
{
}
RigidBody::~RigidBody()
{
	delete rigidBodyPrimitive;
}

ComponentType RigidBody::GetInsType()
{
	return ComponentType::RigidBody;
}

void RigidBody::AddForce(const CVector3& force)
{
	rigidBodyPrimitive->AddForce(force);
}

void RigidBody::AddForceAtPoint(const CVector3& force, const CVector3& point)
{
	rigidBodyPrimitive->AddForceAtPoint(force, point);
}

void RigidBody::AddForceAtLocalPoint(const CVector3& force, const CVector3& point)
{
	rigidBodyPrimitive->AddForceAtLocalPoint(force, point);
}

CVector3 RigidBody::GetPosition() const
{
	return rigidBodyPrimitive->GetPosition();
}

void RigidBody::SetPosition(const CVector3& position)
{
	rigidBodyPrimitive->SetPosition(position);
}

CVector3 RigidBody::GetVelocity() const
{
	return rigidBodyPrimitive->GetVelocity();
}

void RigidBody::SetVelocity(const CVector3& velocity)
{
	rigidBodyPrimitive->SetVelocity(velocity);
}

void RigidBody::SetTransform(const CMatrix4& transform)
{
	rigidBodyPrimitive->SetTransform(transform);
}



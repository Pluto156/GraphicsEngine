#include "stdafx.h"
#include "BoxCollider.h"
ComponentType BoxCollider::GetType()
{
	return ComponentType::BoxCollider;
}

BoxCollider::BoxCollider(const CVector3& halfSizes)
{
	this->halfSizes = halfSizes;
}

BoxCollider::~BoxCollider()
{
	delete mCollider;
}
void BoxCollider::Start()
{
	mCollider = new PhysicsLit::CollisionBox();
	mCollider->mHalfSize = halfSizes;
	SynchronizeData();
	auto rigidBody = gameObject->GetComponent<RigidBody>();
	if (rigidBody)
	{
		mCollider->rigidBodyPrimitive = rigidBody->rigidBodyPrimitive;
		rigidBody->rigidBodyPrimitive->mCollisionVolume = mCollider;
		rigidBody->rigidBodyPrimitive->SetInertiaTensor(mCollider->GetInertiaTensor(rigidBody->rigidBodyPrimitive->GetMass()));
	}
}

ComponentType BoxCollider::GetInsType()
{
	return ComponentType::BoxCollider;
}

void BoxCollider::SynchronizeData()
{
	mCollider->mFriction = mFriction;
	mCollider->mBounciness = mBounciness;
	mCollider->mFrictionCombine = mFrictionCombine;
	mCollider->mBounceCombine = mBounceCombine;
}

void BoxCollider::SynchronizeTransform(const CMatrix4& transform)
{
	mCollider->SynchronizeTransform(transform);
}

bool BoxCollider::IntersectRay(const PhysicsLit::Ray& ray)
{
	return PhysicsLit::IntersectionDetector::Detect(ray, *mCollider);
}

bool BoxCollider::IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit)
{
	return PhysicsLit::IntersectionDetector::Detect(ray, *mCollider,hit);
}
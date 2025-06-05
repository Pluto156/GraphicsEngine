#include "stdafx.h"
#include "SphereCollider.h"
ComponentType SphereCollider::GetType()
{
	return ComponentType::SphereCollider;
}

SphereCollider::SphereCollider(float mRadius)
{
	mCollider = new 
		PhysicsLit::CollisionSphere();
	mCollider->mRadius = mRadius;
	this->radius = mRadius;
}

SphereCollider::~SphereCollider()
{
	delete mCollider;
}

ComponentType SphereCollider::GetInsType()
{
	return ComponentType::SphereCollider;
}

void SphereCollider::SynchronizeData()
{
	mCollider->mFriction = mFriction;
	mCollider->mBounciness = mBounciness;
	mCollider->mFrictionCombine = mFrictionCombine;
	mCollider->mBounceCombine = mBounceCombine;
}

void SphereCollider::SynchronizeTransform(const CMatrix4& transform)
{
	mCollider->SynchronizeTransform(transform);
}

bool SphereCollider::IntersectRay(const PhysicsLit::Ray& ray)
{
	return PhysicsLit::IntersectionDetector::Detect(ray, *mCollider);
}

bool SphereCollider::IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit)
{
	return PhysicsLit::IntersectionDetector::Detect(ray, *mCollider);
}
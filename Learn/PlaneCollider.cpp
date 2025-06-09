#include "stdafx.h"
#include "PlaneCollider.h"
ComponentType PlaneCollider::GetType()
{
	return ComponentType::PlaneCollider;
}

PlaneCollider::PlaneCollider()
{
	mCollider = new PhysicsLit::CollisionPlane();
	mCollider->colliderComponent = this;
}

PlaneCollider::~PlaneCollider()
{
	delete mCollider;
}

ComponentType PlaneCollider::GetInsType()
{
	return ComponentType::PlaneCollider;
}

void PlaneCollider::SynchronizeData()
{
	mCollider->mFriction = mFriction;
	mCollider->mBounciness = mBounciness;
	mCollider->mFrictionCombine = mFrictionCombine;
	mCollider->mBounceCombine = mBounceCombine;
}

void PlaneCollider::SynchronizeTransform(const CMatrix4& transform)
{
	mCollider->SynchronizeTransform(transform);
}

bool PlaneCollider::IntersectRay(const PhysicsLit::Ray& ray)
{
	return PhysicsLit::IntersectionDetector::Detect(ray, *mCollider);
}

bool PlaneCollider::IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit)
{
	return PhysicsLit::IntersectionDetector::Detect(ray, *mCollider);
}
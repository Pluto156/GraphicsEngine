#include "stdafx.h"
#include "PlaneCollider.h"
void PlaneCollider::RegisterFields(TypeInfo& info) {
	REGISTER_FIELD_CUSTOM(PlaneCollider, mCollider, [](PhysicsLit::CollisionPlane* oldPtr, CloneContext& ctx) {
		if (!oldPtr) return (PhysicsLit::CollisionPlane*)nullptr;

		auto* copy = new PhysicsLit::CollisionPlane(*oldPtr);
		ctx.RegisterPointer(oldPtr, copy);
		return copy;
		});
}
void PlaneCollider::PostClone(CloneContext& ctx) {
	if (mCollider) {
		mCollider->colliderComponent = this;

		// 如果涉及其他引用指针，比如 rigidBodyPrimitive，记得使用 MapPointer 修复
		if (mCollider->rigidBodyPrimitive) {
			mCollider->rigidBodyPrimitive = ctx.MapPointer(mCollider->rigidBodyPrimitive);
		}
	}
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
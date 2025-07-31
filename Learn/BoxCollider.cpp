#include "stdafx.h"
#include "BoxCollider.h"
void BoxCollider::RegisterFields(TypeInfo& info) {
	REGISTER_FIELD(BoxCollider, halfSizes);
	REGISTER_FIELD_CUSTOM(BoxCollider, mCollider, [](PhysicsLit::CollisionBox* oldPtr, CloneContext& ctx) {
		if (!oldPtr) return (PhysicsLit::CollisionBox*)nullptr;

		auto* copy = new PhysicsLit::CollisionBox(*oldPtr);
		ctx.RegisterPointer(oldPtr, copy);
		return copy;
		});
}
void BoxCollider::PostClone(CloneContext& ctx) {
	if (mCollider) {
		mCollider->colliderComponent = this;

		// 如果涉及其他引用指针，比如 rigidBodyPrimitive，记得使用 MapPointer 修复
		if (mCollider->rigidBodyPrimitive) {
			mCollider->rigidBodyPrimitive = ctx.MapPointer(mCollider->rigidBodyPrimitive);
		}
	}
}

BoxCollider::BoxCollider()
{
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
	mCollider->colliderComponent = this;
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
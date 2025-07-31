#include "stdafx.h"
#include "SphereCollider.h"
// 注册字段
void SphereCollider::RegisterFields(TypeInfo& info) {
    REGISTER_FIELD(SphereCollider, radius);

    REGISTER_FIELD_CUSTOM(SphereCollider, mCollider, [](PhysicsLit::CollisionSphere* oldPtr, CloneContext& ctx) {
        if (!oldPtr) return (PhysicsLit::CollisionSphere*)nullptr;

        auto* copy = new PhysicsLit::CollisionSphere(*oldPtr);
        ctx.RegisterPointer(oldPtr, copy);
        return copy;
        });

    // Collider 是父类，mFriction等如果在父类里，就在 Collider::RegisterFields 里注册
}

// 克隆后修复引用
void SphereCollider::PostClone(CloneContext& ctx) {
    if (mCollider) {
        mCollider->colliderComponent = this;

        // 如果涉及其他引用指针，比如 rigidBodyPrimitive，记得使用 MapPointer 修复
        if (mCollider->rigidBodyPrimitive) {
            mCollider->rigidBodyPrimitive = ctx.MapPointer(mCollider->rigidBodyPrimitive);
        }
    }
}
SphereCollider::SphereCollider(float mRadius)
{
	mCollider = new PhysicsLit::CollisionSphere();
	mCollider->colliderComponent = this;
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
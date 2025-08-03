#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
	REGISTER_COMPONENT_DERIVED(BoxCollider, ComponentType::BoxCollider, Collider)
public:
	CVector3 halfSizes;
	PhysicsLit::CollisionBox* mCollider = nullptr;
	BoxCollider();
	BoxCollider(const CVector3& halfSizes);
	~BoxCollider();
	void Awake() override;
	virtual ComponentType GetInsType();
	virtual void SynchronizeData();
	virtual void SynchronizeTransform(const CMatrix4& transform);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit);
	PhysicsLit::CollisionPrimitive* GetDeriveCollisionPrimitive() const override
	{
		return const_cast<PhysicsLit::CollisionBox*>(mCollider);  // 向上转型
	}

private:
};

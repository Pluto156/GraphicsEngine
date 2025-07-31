#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
	REGISTER_COMPONENT_DERIVED(BoxCollider, ComponentType::BoxCollider, Collider)
public:
	CVector3 halfSizes;
	//不需要克隆 在组件start中生成
	PhysicsLit::CollisionBox* mCollider = nullptr;
	BoxCollider();
	BoxCollider(const CVector3& halfSizes);
	~BoxCollider();
	void Start() override;
	virtual ComponentType GetInsType();
	virtual void SynchronizeData();
	virtual void SynchronizeTransform(const CMatrix4& transform);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit);
private:
};

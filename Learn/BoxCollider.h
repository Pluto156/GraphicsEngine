#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
public:
	static ComponentType GetType();
	CVector3 halfSizes;

public:
	PhysicsLit::CollisionBox* mCollider = nullptr;

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

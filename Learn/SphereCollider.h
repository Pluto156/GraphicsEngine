#pragma once
class SphereCollider : public Collider
{
	REGISTER_COMPONENT_DERIVED(SphereCollider, ComponentType::SphereCollider, Collider)
public:
	PhysicsLit::CollisionSphere* mCollider = nullptr;
	SphereCollider(){}
	SphereCollider(float mRadius);
	~SphereCollider();

	virtual ComponentType GetInsType();
	virtual void SynchronizeData();
	virtual void SynchronizeTransform(const CMatrix4& transform);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit);
	float radius;
};


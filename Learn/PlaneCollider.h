#pragma once
class PlaneCollider : public Collider
{
	REGISTER_COMPONENT_DERIVED(PlaneCollider, ComponentType::PlaneCollider, Collider)
public:
	PhysicsLit::CollisionPlane* mCollider = nullptr;

	PlaneCollider();
	~PlaneCollider();

	virtual ComponentType GetInsType();
	virtual void SynchronizeData();
	virtual void SynchronizeTransform(const CMatrix4& transform);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit);
	PhysicsLit::CollisionPrimitive* GetDeriveCollisionPrimitive() const override
	{
		return const_cast<PhysicsLit::CollisionPlane*>(mCollider);  // 向上转型
	}
};


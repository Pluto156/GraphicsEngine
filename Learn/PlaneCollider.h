#pragma once
class PlaneCollider : public Collider
{
public:
	static ComponentType GetType();

public:
	PhysicsLit::CollisionPlane* mCollider = nullptr;

	PlaneCollider();
	~PlaneCollider();

	virtual ComponentType GetInsType();
	virtual void SynchronizeData();
	virtual void SynchronizeTransform(const CMatrix4& transform);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit);
};


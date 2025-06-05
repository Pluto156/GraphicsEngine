#pragma once
class SphereCollider : public Collider
{
public:
	static ComponentType GetType();

public:
	PhysicsLit::CollisionSphere* mCollider = nullptr;

	SphereCollider(float mRadius);
	~SphereCollider();

	virtual ComponentType GetInsType();
	virtual void SynchronizeData();
	virtual void SynchronizeTransform(const CMatrix4& transform);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray);
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit);
	float radius;
};


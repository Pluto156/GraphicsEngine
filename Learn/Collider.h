#pragma once
#include "LayerDef.h"
class Collider : public Component
{
	REGISTER_ABSTRACT_COMPONENT(Collider, ComponentType::Collider, Component)
public:
	float mFriction = 0.4f;
	float mBounciness = 0.0f;
	PhysicsLit::CombineType mFrictionCombine = PhysicsLit::CombineType::Average;
	PhysicsLit::CombineType mBounceCombine = PhysicsLit::CombineType::Average;
	PhysicsLit::CollisionPrimitive* mCollider = nullptr;

	virtual void SynchronizeData() = 0;
	virtual void SynchronizeTransform(const CMatrix4& transform) = 0;
	virtual bool IntersectRay(const PhysicsLit::Ray& ray) = 0;
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit) = 0;

	uint32_t GetLayer();
	void SetLayer(PhysicsLit::Layer layer, uint32_t mask);
	virtual PhysicsLit::CollisionPrimitive* GetDeriveCollisionPrimitive() const = 0;
};



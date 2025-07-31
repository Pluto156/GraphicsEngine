#pragma once
class Collider : public Component
{
	REGISTER_ABSTRACT_COMPONENT(Collider, ComponentType::Collider, Component)
public:
	float mFriction = 0.4f;
	float mBounciness = 0.0f;
	PhysicsLit::CombineType mFrictionCombine = PhysicsLit::CombineType::Average;
	PhysicsLit::CombineType mBounceCombine = PhysicsLit::CombineType::Average;

	virtual void SynchronizeData() = 0;
	virtual void SynchronizeTransform(const CMatrix4& transform) = 0;
	virtual bool IntersectRay(const PhysicsLit::Ray& ray) = 0;
	virtual bool IntersectRay(const PhysicsLit::Ray& ray, PhysicsLit::RayHitInfo& hit) = 0;

};


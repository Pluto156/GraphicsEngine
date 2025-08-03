#pragma once
#include "Component.h"
#include "PhysicsLit.h"
class RigidBody : public Component
{
	REGISTER_COMPONENT_BASELESS(RigidBody, ComponentType::RigidBody)
public:
	bool mUseGravity = true;
	PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive = nullptr;
	RigidBody();
	void Awake() override;
	~RigidBody();

	virtual ComponentType GetInsType();
	void AddForce(const CVector3& force);
	void AddForceAtPoint(const CVector3& force, const CVector3& point);
	void AddForceAtLocalPoint(const CVector3& force, const CVector3& point);
	CVector3 GetPosition() const;
	void SetPosition(const CVector3& position);
	void SetRotation(const CQuaternion& quaternion);
	void SetRotation(const CEuler& eulerAngle);
	CVector3 GetVelocity() const;
	void SetVelocity(const CVector3& velocity);
	void SetTransform(const CMatrix4& transform);
};
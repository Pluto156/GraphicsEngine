#include "stdafx.h"
#include "RigidBody.h"
// ---------- 注册字段 ----------
void RigidBody::RegisterFields(TypeInfo& info) {
	REGISTER_FIELD(RigidBody, mUseGravity);
	REGISTER_FIELD_CUSTOM(RigidBody, rigidBodyPrimitive, [](PhysicsLit::RigidBodyPrimitive* oldPtr, CloneContext& ctx) {
		if (!oldPtr) return (PhysicsLit::RigidBodyPrimitive*)nullptr;
		auto* copy = new PhysicsLit::RigidBodyPrimitive(*oldPtr);  // 假设支持复制构造
		printf("RigidBodyPrimitiveCopy = %lx\n", reinterpret_cast<uintptr_t>(copy));
		ctx.RegisterPointer(oldPtr, copy);
		return copy;
		});
}

// ---------- 克隆后修复 ----------
void RigidBody::PostClone(CloneContext& ctx) {
	if (rigidBodyPrimitive) {
		rigidBodyPrimitive->SetGameObjectName(gameObject->name);
		rigidBodyPrimitive->SetPosition(gameObject->transform->GetPosition());
		rigidBodyPrimitive->SetRotation(gameObject->transform->GetQuaternion());
	}
}
void RigidBody::Awake()
{
	rigidBodyPrimitive = new PhysicsLit::RigidBodyPrimitive();
	printf("Awake rigidBodyPrimitiveCopy = %lx\n", reinterpret_cast<uintptr_t>(rigidBodyPrimitive));

	rigidBodyPrimitive->SetPosition(gameObject->transform->GetPosition());
	rigidBodyPrimitive->SetRotation(gameObject->transform->GetQuaternion());

}

RigidBody::RigidBody()
{
}
RigidBody::~RigidBody()
{
	delete rigidBodyPrimitive;
}

ComponentType RigidBody::GetInsType()
{
	return ComponentType::RigidBody;
}

void RigidBody::AddForce(const CVector3& force)
{
	rigidBodyPrimitive->AddForce(force);
}

void RigidBody::AddForceAtPoint(const CVector3& force, const CVector3& point)
{
	rigidBodyPrimitive->AddForceAtPoint(force, point);
}

void RigidBody::AddForceAtLocalPoint(const CVector3& force, const CVector3& point)
{
	rigidBodyPrimitive->AddForceAtLocalPoint(force, point);
}

CVector3 RigidBody::GetPosition() const
{
	return rigidBodyPrimitive->GetPosition();
}

void RigidBody::SetPosition(const CVector3& position)
{
	rigidBodyPrimitive->SetPosition(position);
}

void RigidBody::SetRotation(const CQuaternion& quaternion)
{
	rigidBodyPrimitive->SetRotation(quaternion);
}
void RigidBody::SetRotation(const CEuler& eulerAngle)
{
	rigidBodyPrimitive->SetRotation(eulerAngle.ToCQuaternion());
}


CVector3 RigidBody::GetVelocity() const
{
	return rigidBodyPrimitive->GetVelocity();
}

void RigidBody::SetVelocity(const CVector3& velocity)
{
	rigidBodyPrimitive->SetVelocity(velocity);
}

void RigidBody::SetTransform(const CMatrix4& transform)
{
	rigidBodyPrimitive->SetTransform(transform);
}



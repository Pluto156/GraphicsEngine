#include "stdafx.h"
#include "Bullet.h"

void Bullet::RegisterFields(TypeInfo& info) {
	REGISTER_FIELD(Bullet, StartPos);
	REGISTER_FIELD(Bullet, MoveDir);
	REGISTER_FIELD(Bullet, MaxFlyDistance);
}

void Bullet::PostClone(CloneContext& ctx) {
	rigidBody = gameObject->GetComponent<RigidBody>();
}

void Bullet::OnColliderEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::OnColliderStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
}
void Bullet::OnColliderExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::OnTriggerEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
	Debug::Log(gameObject->name+" OnTriggerEnter "+ transform->position.ToString()+" "+ PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) + " " + rigidBodyPrimitive->GetGameObjectName());
	GameObjectManager::Instance().Destroy(gameObject);
}
void Bullet::OnTriggerStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::OnTriggerExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::Awake()
{
	rigidBody = gameObject->GetComponent<RigidBody>();
}

void Bullet::Update()
{
	if ((gameObject->transform->position - StartPos).len() > MaxFlyDistance)
	{
		GameObjectManager::Instance().Destroy(gameObject);
	}

	rigidBody->SetPosition(gameObject->transform->position+MoveDir/10);
}
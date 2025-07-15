#include "stdafx.h"
#include "Bullet.h"
void Bullet::OnColliderEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
	//std::cout << gameObject->name << " OnColliderEnter " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;
	//GameObjectManager::Instance().Destroy(gameObject);

}
void Bullet::OnColliderStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
	//std::cout << gameObject->name << " OnColliderStay " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;
}
void Bullet::OnColliderExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::OnTriggerEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
	//std::cout << gameObject->name << " OnTriggerEnter " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;
	GameObjectManager::Instance().Destroy(gameObject);
}
void Bullet::OnTriggerStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
	//std::cout << gameObject->name << " OnTriggerStay " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;

}
void Bullet::OnTriggerExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::Start()
{
	rigidBody = gameObject->GetComponent<RigidBody>()->rigidBodyPrimitive;
}

void Bullet::Update()
{
	if ((gameObject->transform->position - StartPos).len() > MaxFlyDistance)
	{
		GameObjectManager::Instance().Destroy(gameObject);
	}
	//std::cout << gameObject->transform->position.ToString() << std::endl;

	rigidBody->SetPosition(gameObject->transform->position+MoveDir/10);
}
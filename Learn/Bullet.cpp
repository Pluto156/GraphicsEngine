#include "stdafx.h"
#include "Bullet.h"
void Bullet::OnColliderEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
	std::cout << gameObject->name << " OnColliderEnter " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;

	GameObjectManager::Instance().Destroy(gameObject);
}
void Bullet::OnColliderStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Bullet::OnColliderExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
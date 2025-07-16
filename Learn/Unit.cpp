#include "stdafx.h"
#include "Unit.h"

void Unit::OnColliderEnter(PhysicsLit::RigidBodyPrimitive*)
{

}
void Unit::OnColliderStay(PhysicsLit::RigidBodyPrimitive*)
{

}
void Unit::OnColliderExit(PhysicsLit::RigidBodyPrimitive*)
{

}

void Unit::OnTriggerEnter(PhysicsLit::RigidBodyPrimitive*)
{
	--this->Health;
	if (Health <= 0)
	{
		GameObjectManager::Instance().Destroy(gameObject);
	}
}
void Unit::OnTriggerStay(PhysicsLit::RigidBodyPrimitive*)
{

}
void Unit::OnTriggerExit(PhysicsLit::RigidBodyPrimitive*)
{

}

void Unit::Start()
{

}
void Unit::Update()
{

}
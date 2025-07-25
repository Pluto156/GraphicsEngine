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
	--this->CurHealth;

	HealthBar->localScale = CVector3(1.5* CurHealth/FullHealth,0.05,0.05);

	if (CurHealth <= 0)
	{
		CombatManager::Instance().RecycleCar(gameObject);
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
void Unit::ReSet()
{
	this->HealthBar->localScale = CVector3(1.5, 0.05, 0.05);
	this->CurHealth = FullHealth;
}
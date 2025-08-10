#include "stdafx.h"
#include "Unit.h"


void Unit::OnTriggerEnter(Collider* other)
{
	if (other->GetLayer() == PhysicsLit::Layer::BULLET)
	{
		Debug::Log(gameObject->name+" OnTriggerEnter bullet");

		--this->CurHealth;
		RefreshHealthBar();
		if (CurHealth <= 0)
		{
			CombatManager::Instance().RecycleCar(gameObject);
		}
	}
	else if (other->GetLayer() == PhysicsLit::Layer::Item)
	{
		Debug::Log(gameObject->name + " OnTriggerEnter Item");
		AddHealth(other->gameObject->GetComponent<HealthPack>()->addHealth);

		CombatManager::Instance().RecycleHealthPack(other->gameObject);

	}
	

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
	RefreshHealthBar();
}
void Unit::AddHealth(int addnum)
{
	this->CurHealth += addnum;
	this->CurHealth = Math::Clamp(this->CurHealth, 0, FullHealth);
	RefreshHealthBar();
}
void Unit::RefreshHealthBar()
{
	HealthBar->localScale = CVector3(1.5 * CurHealth / FullHealth, 0.05, 0.05);
}
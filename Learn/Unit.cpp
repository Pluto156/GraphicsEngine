#include "stdafx.h"
#include "Unit.h"


void Unit::OnTriggerEnter(Collider* other)
{
	Debug::Log("hit " + other->gameObject->name);
	--this->CurHealth;
	RefreshHealthBar();
	if (CurHealth <= 0)
	{
		CombatManager::Instance().RecycleCar(gameObject);
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
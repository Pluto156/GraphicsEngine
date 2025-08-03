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


void Bullet::OnTriggerEnter(Collider* other)
{
	GameObjectManager::Instance().Destroy(gameObject);
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
#include "stdafx.h"
#include "HealthPack.h"
void HealthPack::RegisterFields(TypeInfo& info) {
	REGISTER_FIELD(HealthPack, rotateSpeed);
	REGISTER_FIELD(HealthPack, Euler_h);
	REGISTER_FIELD(HealthPack, addHealth);
}

void HealthPack::PostClone(CloneContext& ctx) {

}

void HealthPack::OnColliderEnter(Collider* other)
{
	Debug::Log("HealthPack OnColliderEnter "+ other->gameObject->name);
	if (other->GetLayer() == PhysicsLit::Layer::PLAYER)
	{
		other->gameObject->GetComponent<Unit>()->AddHealth(addHealth);
		GameObjectManager::Instance().Destroy(gameObject);

	}
}

void HealthPack::Awake()
{
	rigidBody = gameObject->GetComponent<RigidBody>();
}


void HealthPack::Update()
{
	Euler_h = Math::Clamp(Euler_h+rotateSpeed, -180, 180);
	if (Euler_h >= 180)
	{
		Euler_h = -180;
	}
	rigidBody->SetRotation(CEuler(Euler_h,transform->eulerAngles.p, transform->eulerAngles.b));

}

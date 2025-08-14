#include "stdafx.h"
#include "BreakableWall.h"
#include "stdafx.h"

void BreakableWall::RegisterFields(TypeInfo& info) {
	REGISTER_FIELD(BreakableWall, FullHealth);
	REGISTER_FIELD(BreakableWall, CurHealth);
	REGISTER_FIELD(BreakableWall, isDead);
}

void BreakableWall::PostClone(CloneContext& ctx) {

}
void BreakableWall::OnTriggerEnter(Collider* other)
{
    if (other->GetLayer() == PhysicsLit::Layer::BULLET)
    {
        Debug::Log(gameObject->name + " OnTriggerEnter bullet");

        --this->CurHealth;

        // 计算血量比例
        float healthRatio = Math::Max(0.0f, static_cast<float>(CurHealth) / static_cast<float>(FullHealth));

        // 原始颜色（比如白色）
        CVector3 baseColor(0.5, 0.5, 0.5);
        // 受伤颜色（红色）
        CVector3 damageColor(1.0f, 0.0f, 0.0f);

        // 颜色插值: ratio=1 → baseColor, ratio=0 → damageColor
        CVector3 currentColor = damageColor * (1.0f - healthRatio) + baseColor * healthRatio;

        gameObject->GetComponent<MeshRenderer>()->SetDiffuseColor(currentColor);

        if (CurHealth <= 0)
        {
            CombatManager::Instance().RecycleBreakWall(gameObject);
        }
    }
}



void BreakableWall::Start()
{

}
void BreakableWall::Update()
{

}
void BreakableWall::ReSet()
{
	this->CurHealth = FullHealth;

}
void BreakableWall::AddHealth(int addnum)
{
	this->CurHealth += addnum;
	this->CurHealth = Math::Clamp(this->CurHealth, 0, FullHealth);
}

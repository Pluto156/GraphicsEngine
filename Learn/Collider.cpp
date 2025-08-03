#include "stdafx.h"
#include "Collider.h"


void Collider::RegisterFields(TypeInfo& info) {
    REGISTER_FIELD(Collider, mFriction);
    REGISTER_FIELD(Collider, mBounciness);
    REGISTER_FIELD(Collider, mFrictionCombine);
    REGISTER_FIELD(Collider, mBounceCombine);
}

void Collider::PostClone(CloneContext& ctx) {
    // 一般无需额外操作，留空
}

uint32_t Collider::GetLayer()
{
    if (mCollider == nullptr)
    {
        mCollider = GetDeriveCollisionPrimitive();
    }
    return mCollider->layer;
}
void Collider::SetLayer(PhysicsLit::Layer layer, uint32_t mask)
{
    if (mCollider == nullptr)
    {
        mCollider = GetDeriveCollisionPrimitive();
    }
    mCollider->SetLayer(layer, mask);
}
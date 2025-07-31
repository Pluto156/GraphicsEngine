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
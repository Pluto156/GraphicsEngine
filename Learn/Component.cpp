#include "stdafx.h"
#include "Component.h"

void Component::RegisterFields(TypeInfo& info) {
    REGISTER_FIELD_CUSTOM(Component, gameObject,
        [](GameObject* oldPtr, CloneContext& ctx) {
            return ctx.MapPointer(oldPtr);
        });

    REGISTER_FIELD_CUSTOM(Component, transform,
        [](Transform* oldPtr, CloneContext& ctx) {
            return ctx.MapPointer(oldPtr);
        });
}

void Component::PostClone(CloneContext& ctx) {

}

Component* Component::Clone(CloneContext& ctx) const {
    auto* typeInfo = ReflectionRegistry::Instance().GetTypeInfo(GetType());
    if (typeInfo) {
        return typeInfo->Clone(this, ctx);
    }
    return nullptr;
}

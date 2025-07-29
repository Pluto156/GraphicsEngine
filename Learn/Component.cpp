#include "stdafx.h"
#include "Component.h"
ComponentType Component::GetType()
{
	return ComponentType::Component;
}

Component* Component::Clone() const {
    const TypeInfo* type = ReflectionRegistry::Instance().GetTypeInfo(GetType());
    if (!type) return nullptr;
    return type->Clone(this);
}
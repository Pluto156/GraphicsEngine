#include "stdafx.h"
#include "Reflection.h"

// ---------- ReflectionRegistry ----------
ReflectionRegistry& ReflectionRegistry::Instance() {
    static ReflectionRegistry instance;
    return instance;
}

void ReflectionRegistry::RegisterType(ComponentType type, const TypeInfo& info) {
    types[type] = info;
}

const TypeInfo* ReflectionRegistry::GetTypeInfo(ComponentType type) const {
    auto it = types.find(type);
    return (it != types.end()) ? &it->second : nullptr;
}

// ---------- TypeInfo Clone ----------
Component* TypeInfo::Clone(const Component* src, CloneContext& ctx) const {
    Component* dst = static_cast<Component*>(creator());
    ctx.RegisterPointer(src, dst);

    // ✅ 先克隆父类字段（递归）
    if (base) {
        base->Clone(src, ctx);
    }

    // ✅ 再克隆本类字段
    for (const FieldInfo& f : fields) {
        if (f.copyType == FieldCopyType::RawCopy) {
            std::memcpy(
                reinterpret_cast<char*>(dst) + f.offset,
                reinterpret_cast<const char*>(src) + f.offset,
                f.size
            );
        }
        else if (f.copyType == FieldCopyType::Custom && f.customCopy) {
            f.customCopy(dst, src, ctx);
        }
    }

    dst->PostClone(ctx);
    return dst;
}
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

Component* TypeInfo::Clone(const Component* src, CloneContext& ctx) const {
    if (!creator) {
        // 抽象类或无法创建实例的类型不能 clone
        return nullptr;
    }

    Component* dst = static_cast<Component*>(creator());
    ctx.RegisterPointer(src, dst);

    // 递归拷贝字段，包括父类字段
    const TypeInfo* current = this;
    // 用栈先收集继承链，从 base 到 this
    std::vector<const TypeInfo*> hierarchy;
    for (const TypeInfo* current = this; current != nullptr; current = current->base) {
        hierarchy.push_back(current);
    }
    std::reverse(hierarchy.begin(), hierarchy.end());  // 先处理基类字段

    for (const TypeInfo* type : hierarchy) {
        for (const auto& field : type->fields) {
            void* dstField = reinterpret_cast<char*>(dst) + field.offset;
            const void* srcField = reinterpret_cast<const char*>(src) + field.offset;

            if (field.copyType == FieldCopyType::RawCopy) {
                std::memcpy(dstField, srcField, field.size);
            }
            else if (field.copyType == FieldCopyType::Custom && field.customCopy) {
                field.customCopy(dst, src, ctx);
            }
        }
    }


    dst->PostClone(ctx);
    return dst;
}

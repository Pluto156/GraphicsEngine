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

// ---------- CloneGameObjectTree ----------
GameObject* CloneGameObjectTree(const GameObject* root) {
    if (!root) return nullptr;

    CloneContext ctx;
    GameObject* newRoot = new GameObject();
    ctx.RegisterPointer(root, newRoot);
    ctx.root = newRoot;

    std::vector<std::pair<const GameObject*, GameObject*>> stack;
    stack.emplace_back(root, newRoot);

    while (!stack.empty()) {
        const GameObject* src = stack.back().first;
        GameObject* dst = stack.back().second;
        stack.pop_back();

        // Copy name and transform (assumed to exist)
        dst->name = src->name;
        dst->transform = src->transform;  // Or deep copy if needed
        ctx.RegisterPointer(&src->transform, &dst->transform);

        // Clone components
        for (Component* comp : src->components) {
            if (!comp) continue;
            const TypeInfo* info = ReflectionRegistry::Instance().GetTypeInfo(comp->GetType());
            if (!info) continue;
            Component* newComp = info->Clone(comp, ctx);
            dst->AddComponent(newComp);
        }

        // Recursively clone children
        for (const GameObject* child : src->children) {
            GameObject* newChild = new GameObject();
            ctx.RegisterPointer(child, newChild);
            dst->AddChild(newChild);
            stack.emplace_back(child, newChild);
        }
    }

    return newRoot;
}

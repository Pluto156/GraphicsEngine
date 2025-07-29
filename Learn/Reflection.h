#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cstring>
#include <cassert>
#include <type_traits>

#include "Component.h"
#include "GameObject.h"  // 提前声明或包含 GameObject 定义

// ---------- FieldInfo ----------
enum class FieldCopyType {
    RawCopy,
    Custom
};

struct CloneContext; // 前向声明

struct FieldInfo {
    std::string name;
    size_t offset;
    size_t size;
    FieldCopyType copyType;
    std::function<void(void* dstObj, const void* srcObj, CloneContext&)> customCopy;
};

// ---------- TypeInfo ----------
struct TypeInfo {
    std::string name;
    std::function<void* ()> creator;
    std::vector<FieldInfo> fields;

    Component* Clone(const Component* src, CloneContext& ctx) const;
};

// ---------- CloneContext ----------
struct CloneContext {
    std::unordered_map<const void*, void*> pointerMap;
    GameObject* root = nullptr;

    template<typename T>
    T* MapPointer(const T* oldPtr) const {
        if (!oldPtr) return nullptr;
        auto it = pointerMap.find(oldPtr);
        return it != pointerMap.end() ? static_cast<T*>(it->second) : nullptr;
    }

    template<typename T>
    void RegisterPointer(const T* oldPtr, T* newPtr) {
        pointerMap[oldPtr] = newPtr;
    }
};

// ---------- Reflection Registry ----------
class ReflectionRegistry {
public:
    static ReflectionRegistry& Instance();

    void RegisterType(ComponentType type, const TypeInfo& info);
    const TypeInfo* GetTypeInfo(ComponentType type) const;

private:
    std::unordered_map<ComponentType, TypeInfo> types;
};

// ---------- Macros ----------
#define REGISTER_COMPONENT(CLASS, ENUM_TYPE)                                      \
public:                                                                           \
    static Component* CreateInstance() { return new CLASS(); }                   \
    virtual ComponentType GetType() const override { return ENUM_TYPE; }         \
    virtual void PostClone(CloneContext&) override {}                            \
    static void RegisterFields(TypeInfo& info);                                   \
private:                                                                          \
    static struct AutoRegister_##CLASS {                                          \
        AutoRegister_##CLASS() {                                                  \
            TypeInfo info;                                                        \
            info.name = #CLASS;                                                   \
            info.creator = []() -> void* { return new CLASS(); };                \
            CLASS::RegisterFields(info);                                          \
            ReflectionRegistry::Instance().RegisterType(ENUM_TYPE, info);         \
        }                                                                          \
    } autoRegisterInstance_##CLASS;

#define REGISTER_FIELD(CLASS, field)                                       \
    info.fields.push_back(FieldInfo{                                       \
        #field,                                                            \
        offsetof(CLASS, field),                                            \
        sizeof(((CLASS*)nullptr)->field),                                  \
        FieldCopyType::RawCopy,                                            \
        nullptr                                                            \
    });

#define REGISTER_FIELD_CUSTOM(field, copyFunc)                                     \
    info.fields.push_back(FieldInfo{                                              \
        #field,                                                                   \
        offsetof(std::decay_t<decltype(*this)>, field),                           \
        sizeof(field),                                                            \
        FieldCopyType::Custom,                                                    \
        [](void* dstObj, const void* srcObj, CloneContext& ctx) {                 \
            auto* dst = reinterpret_cast<std::decay_t<decltype(*this)>*>(dstObj); \
            auto* src = reinterpret_cast<const std::decay_t<decltype(*this)>*>(srcObj); \
            dst->field = copyFunc(src->field, ctx);                               \
        }                                                                         \
    });

// ---------- Clone Entry ----------
GameObject* CloneGameObjectTree(const GameObject* root);

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cstring>
#include <cassert>
#include <type_traits>

enum class ComponentType {
    Component,
    Transform,
    RigidBody,
    Collider,
    BoxCollider,
    SphereCollider,
    PlaneCollider,
    MeshFilter,
    MeshRenderer,
    Light,
    GameScript,
    CharacterController,
    Bullet
};

// ---------- FieldInfo ----------
enum class FieldCopyType {
    RawCopy,
    Custom
};

struct CloneContext;

struct FieldInfo {
    std::string name;
    size_t offset;
    size_t size;
    FieldCopyType copyType;
    std::function<void(void* dstObj, const void* srcObj, CloneContext&)> customCopy;
};

// ---------- TypeInfo ----------
class Component;
struct TypeInfo {
    std::string name;
    std::function<void* ()> creator;
    std::vector<FieldInfo> fields;
    const TypeInfo* base = nullptr;

    Component* Clone(const Component* src, CloneContext& ctx) const;

    template<typename T>
    static const TypeInfo* Get() {
        return ReflectionRegistry::Instance().GetTypeInfo(T::StaticType());
    }
};

// ---------- CloneContext ----------
class GameObject;
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
#define DEFINE_COMPONENT_AUTOREGISTER(CLASS) \
    decltype(CLASS::autoRegisterInstance_##CLASS) CLASS::autoRegisterInstance_##CLASS;

#define REGISTER_COMPONENT_BASELESS(CLASS, ENUM_TYPE) \
public: \
    using BaseClassType = void; \
    static Component* CreateInstance() { return new CLASS(); } \
    virtual ComponentType GetType() const { return ENUM_TYPE; } \
    virtual void PostClone(CloneContext&); \
    static void RegisterFields(TypeInfo& info); \
    static constexpr ComponentType StaticType() { return ENUM_TYPE; } \
    virtual const TypeInfo* GetTypeInfo() const { \
        return ReflectionRegistry::Instance().GetTypeInfo(this->GetType()); \
    } \
public: \
    static void _RegisterType() { \
        TypeInfo info; \
        info.name = #CLASS; \
        info.creator = []() -> void* { return new CLASS(); }; \
        CLASS::RegisterFields(info); \
        ReflectionRegistry::Instance().RegisterType(ENUM_TYPE, info); \
        std::cout << "Registering: " << #CLASS << std::endl; \
    } \
    struct AutoRegister_##CLASS { \
        AutoRegister_##CLASS() { CLASS::_RegisterType(); } \
    }; \
    static AutoRegister_##CLASS autoRegisterInstance_##CLASS;

#define REGISTER_COMPONENT_DERIVED(CLASS, ENUM_TYPE, BASE_CLASS) \
public: \
    using BaseClassType = BASE_CLASS; \
    static Component* CreateInstance() { return new CLASS(); } \
    virtual ComponentType GetType() const override { return ENUM_TYPE; } \
    virtual void PostClone(CloneContext&) override; \
    static void RegisterFields(TypeInfo& info); \
    static constexpr ComponentType StaticType() { return ENUM_TYPE; } \
    virtual const TypeInfo* GetTypeInfo() const override { \
        return ReflectionRegistry::Instance().GetTypeInfo(this->GetType()); \
    } \
public: \
    static struct AutoRegister_##CLASS { \
        AutoRegister_##CLASS() { \
            TypeInfo info; \
            info.name = #CLASS; \
            info.creator = []() -> void* { return new CLASS(); }; \
            info.base = ReflectionRegistry::Instance().GetTypeInfo(BASE_CLASS::StaticType()); \
            CLASS::RegisterFields(info); \
            ReflectionRegistry::Instance().RegisterType(ENUM_TYPE, info); \
            std::cout << "Registering: " << #CLASS << std::endl; \
        } \
    } autoRegisterInstance_##CLASS;

#define REGISTER_ABSTRACT_COMPONENT(CLASS, ENUM_TYPE, BASE_CLASS) \
public: \
    using BaseClassType = BASE_CLASS; \
    static constexpr ComponentType StaticType() { return ENUM_TYPE; } \
    virtual ComponentType GetType() const override { return ENUM_TYPE; } \
    virtual void PostClone(CloneContext&) override; \
    static void RegisterFields(TypeInfo& info); \
    virtual const TypeInfo* GetTypeInfo() const override { \
        return ReflectionRegistry::Instance().GetTypeInfo(this->GetType()); \
    } \
public: \
    static struct AutoRegister_##CLASS { \
        AutoRegister_##CLASS() { \
            TypeInfo info; \
            info.name = #CLASS; \
            info.creator = nullptr; \
            info.base = ReflectionRegistry::Instance().GetTypeInfo(BASE_CLASS::StaticType()); \
            CLASS::RegisterFields(info); \
            ReflectionRegistry::Instance().RegisterType(ENUM_TYPE, info); \
            std::cout << "Registering: " << #CLASS << std::endl; \
        } \
    } autoRegisterInstance_##CLASS;

#define REGISTER_FIELD(CLASS, field) \
    info.fields.push_back(FieldInfo{ \
        #field, \
        offsetof(CLASS, field), \
        sizeof(((CLASS*)nullptr)->field), \
        FieldCopyType::RawCopy, \
        nullptr \
    });

#define REGISTER_FIELD_CUSTOM(CLASS, field, copyFunc) \
    info.fields.push_back(FieldInfo{ \
        #field, \
        offsetof(CLASS, field), \
        sizeof(((CLASS*)nullptr)->field), \
        FieldCopyType::Custom, \
        [](void* dstObj, const void* srcObj, CloneContext& ctx) { \
            auto* dst = reinterpret_cast<CLASS*>(dstObj); \
            auto* src = reinterpret_cast<const CLASS*>(srcObj); \
            dst->field = copyFunc(src->field, ctx); \
        } \
    });

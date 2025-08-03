#pragma once
#include "GameObjectManager.h"
#include "CglFont.h"
#include "Transform.h"
#include "GameScript.h"

class Component;
class GameObject {
public:
    std::string name;
    Transform* transform = nullptr;
    GameScript* gameScript = nullptr;
    CglFont infoFont;
    GameObject(const std::string& name, const CVector3& position = CVector3(),
        const CMatrix4& rotation = CMatrix4(),
        const CEuler& eulerAngles = CEuler(), bool isShowLocalAxis = false);

    ~GameObject()
    {
        for (auto comp : components) {
            delete comp;
        }
        components.clear(); 
        transform = nullptr;
    }

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        T* component = new T(std::forward<Args>(args)...);
        component->gameObject = this;
        component->transform = transform;
        components.push_back(component);
        using DecayedT = std::decay_t<T>;
        constexpr bool is_script = std::is_base_of<GameScript, DecayedT>::value;
        AssignIfGameScript(component, std::bool_constant<is_script>{});
        component->Awake();
        return component;
    }

    // 非 GameScript 时：空函数
    template<typename T>
    void AssignIfGameScript(T*, std::false_type) {}

    // 是 GameScript 时
    template<typename T>
    void AssignIfGameScript(T* ptr, std::true_type) {
        this->gameScript = static_cast<GameScript*>(ptr);
    }

    template<typename T>
    T* GetComponent() {
        for (auto comp : components) {
            T* result = dynamic_cast<T*>(comp);
            if (result) return result;
        }
        return nullptr;
    }
    void AddChild(GameObject* child);
    //变换后更新数据
    virtual void UpdateSomeDataAfterTransformChange() {}
    //用于渲染帧更新
    virtual void Update();
    virtual void Draw();

    void SetIsPendingDestroy(bool isPendingDestroy)
    {
        this->isPendingDestroy = isPendingDestroy;
    }

    bool IsPendingDestroy()
    {
        return this->isPendingDestroy;
    }
    GameObject* Clone() const;
    GameObject* Clone(CloneContext& ctx) const;

    void CallAllComponentStart();

private:
    std::vector<Component*> components;
    bool isPendingDestroy = false;

    void AddComponentRaw(Component* comp);


};

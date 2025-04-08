#pragma once
#include "GameObjectManager.h"
#include "Transform.h"
#include "CglFont.h"

class GameObject {
public:
    std::string name;
    Transform* transform = nullptr;
    CglFont infoFont;
    GameObject(const std::string& name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(), bool isShowLocalAxis = false) : name(name)
    {
        GameObjectManager::Instance().Register(this);
        transform = AddComponent<Transform>(position, rotation, eulerAngles, isShowLocalAxis);
        transform->gameObject = this;
        infoFont.SetColor(1.0f, 1.0f, 0.5f);  // 淡黄色
        infoFont.SetPosition(10, 30);
    }
    ~GameObject()
    {
        for (auto comp : components) {
            delete comp;
        }
        components.clear(); 
        transform = nullptr;
        GameObjectManager::Instance().Unregister(this);
    }

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        T* component = new T(std::forward<Args>(args)...);
        component->gameObject = this;
        components.push_back(component);
        component->Start();  
        return component;
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
    virtual void Draw() 
    {
    }


private:
    std::vector<Component*> components;
};

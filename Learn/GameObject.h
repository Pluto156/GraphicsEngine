#pragma once
#include "GameObjectManager.h"
#include "CglFont.h"
#include "Transform.h"


class Component;
class GameObject {
public:
    std::string name;
    Transform* transform = nullptr;
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
    virtual void Draw();



private:
    std::vector<Component*> components;
};

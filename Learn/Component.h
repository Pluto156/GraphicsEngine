#pragma once
#include "CVector2.h"
#include "CVector3.h"
#include "CVector4.h"
#include "CMatrix3.h"
#include "CMatrix4.h"
#include "CEuler.h"
#include "Reflection.h"

class GameObject;
class Transform;
class CloneContext;
class Component {
    REGISTER_COMPONENT_BASELESS(Component, ComponentType::Component)
public:
    GameObject* gameObject = nullptr;  // 反向引用所属对象
    Transform* transform;// 反向引用所属对象Transform


    virtual void Start() {}   // 初始化时调用
    virtual void Update() {}  // 每帧更新
    virtual ~Component() = default;

    // Called after clone + pointer map fix
    Component* Clone(CloneContext& ctx) const;
};




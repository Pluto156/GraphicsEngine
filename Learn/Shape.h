#pragma once
#include "GameObject.h"
#include "PhysicsManager.h"
class Shape : public GameObject
{
public:
    bool isSelect;
    RigidBody* rigidBody;

    Shape(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false) :GameObject(name, position, rotation, eulerAngles, isShowLocalAxis)
    {
        rigidBody = PhysicsManager::Instance().Register(this);
    }

    virtual void Draw() override  {
        
    }

};


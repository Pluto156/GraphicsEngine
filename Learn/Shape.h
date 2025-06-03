#pragma once
#include "GameObject.h"
#include "PhysicsManager.h"
#include "ForceGravity.h"

class Shape : public GameObject
{
public:
    bool isSelect;
    RigidBody* rigidBody;

    Shape(std::string name, const CVector3& position = CVector3(),
        const CMatrix4& rotation = CMatrix4(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false) :GameObject(name, position, rotation, eulerAngles, isShowLocalAxis)
    {
        

    }

    virtual void Draw() override  {
        
    }

};


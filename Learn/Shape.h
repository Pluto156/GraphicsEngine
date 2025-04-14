#pragma once
#include "GameObject.h"
#include "PhysicsManager.h"
#include "ForceGravity.h"

class Shape : public GameObject
{
public:
    bool isSelect;
    PhysicsLit::RigidBody* rigidBody;

    Shape(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false) :GameObject(name, position, rotation, eulerAngles, isShowLocalAxis)
    {
        rigidBody = PhysicsLit::PhysicsManager::Instance().Register(this);
        rigidBody->AddForceGenerator(new PhysicsLit::ForceGravity(CVector(0.0f, -9.8f, 0.0f)));
        rigidBody->SetVelocity(CVector(0, 20, 0));

    }

    virtual void Draw() override  {
        
    }

};


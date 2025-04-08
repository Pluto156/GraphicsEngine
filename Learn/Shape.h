#pragma once
#include "GameObject.h"
class Shape : public GameObject
{
public:
    bool isSelect;

    Shape(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false):GameObject(name, position, rotation, eulerAngles, isShowLocalAxis)
    {
        
    }

    virtual void Draw() override  {
        
    }

};


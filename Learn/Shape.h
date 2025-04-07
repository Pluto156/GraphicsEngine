#pragma once
#include "Transform.h"
class Shape : public Transform
{
public:
    OBB obb;            // �󶨵������Χ��
    bool isSelect;

    Shape(std::string name, const CVector& position = CVector(),
        const CMatrix& rotation = CMatrix(),
        const CEuler& eulerAngles = CEuler(),
        bool isShowLocalAxis = false):Transform(name, position, rotation, eulerAngles, isShowLocalAxis), obb(position, CVector(), rotation),isSelect(false)
    {

    }

    void Draw() override  {
        
    }
    void UpdateSomeDataAfterTransformChange()override
    {
        obb.Update(position, rotation);
    }

};


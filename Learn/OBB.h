#pragma once
#include "CVector3.h"
#include "CMatrix4.h"
#include "PointCollision.h"
class OBB :public Component{
public:
    CVector3 center;      // 盒子中心
    CVector3 halfSizes;   // 半尺寸
    CMatrix4 rotation;    // 旋转矩阵

    OBB(const CVector3& center = CVector3(), const CVector3& halfSizes = CVector3(1, 1, 1), const CMatrix4& rotation = CMatrix4())
        : center(center), halfSizes(halfSizes), rotation(rotation) {
    }

    void Update() override;

    float IntersectWithRayAndOBB(
        const CVector3& origin,
        const CVector3& direct,
        float length,
        PointCollision& intersection) const;
    float IntersectWithRayAndOBB(
        const CVector3& origin,
        const CVector3& direct,
        const CVector3& position,
        const CVector3& halfSize,
        const CMatrix4& orientation,
        PointCollision& intersection) const;
};

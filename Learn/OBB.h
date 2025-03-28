#pragma once
#include "CVector.h"
#include "CMatrix.h"
#include "PointCollision.h"
class OBB {
public:
    CVector center;      // 盒子中心
    CVector halfSizes;   // 半尺寸
    CMatrix rotation;    // 旋转矩阵

    OBB(const CVector& center = CVector(), const CVector& halfSizes = CVector(1, 1, 1), const CMatrix& rotation = CMatrix())
        : center(center), halfSizes(halfSizes), rotation(rotation) {
    }

    void Update(const CVector& newCenter, const CMatrix& newRotation) {
        center = newCenter;
        rotation = newRotation;
    }

    float IntersectWithRayAndOBB(
        const CVector& origin,
        const CVector& direct,
        float length,
        PointCollision& intersection) const;
    float IntersectWithRayAndOBB(
        const CVector& origin,
        const CVector& direct,
        const CVector& position,
        const CVector& halfSize,
        const CMatrix& orientation,
        PointCollision& intersection) const;
};

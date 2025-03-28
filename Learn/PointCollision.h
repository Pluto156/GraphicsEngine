#pragma once
#include "CVector.h"

class PointCollision {
public:
    bool hit;               // 是否发生碰撞
    float t;                // 相交点的参数
    CVector closestPoint;   // 最近的交点
    CVector normal;         // 法线

    PointCollision()
        : hit(false), t(-1.0f), closestPoint(CVector()), normal(CVector()) {
    }

    // 重置碰撞信息
    void Reset() {
        hit = false;
        t = -1.0f;
        closestPoint = CVector();
        normal = CVector();
    }
};
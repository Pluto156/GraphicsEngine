#pragma once
#include "CVector3.h"

class PointCollision {
public:
    bool hit;               // 是否发生碰撞
    float t;                // 相交点的参数
    CVector3 closestPoint;   // 最近的交点
    CVector3 normal;         // 法线

    PointCollision()
        : hit(false), t(-1.0f), closestPoint(CVector3()), normal(CVector3()) {
    }

    // 重置碰撞信息
    void Reset() {
        hit = false;
        t = -1.0f;
        closestPoint = CVector3();
        normal = CVector3();
    }
};
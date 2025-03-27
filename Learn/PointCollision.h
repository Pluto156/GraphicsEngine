#pragma once
#include "CVector.h"

class PointCollision {
public:
    bool hit;               // �Ƿ�����ײ
    float t;                // �ཻ��Ĳ���
    CVector closestPoint;   // ����Ľ���
    CVector normal;         // ����

    PointCollision()
        : hit(false), t(-1.0f), closestPoint(CVector()), normal(CVector()) {
    }

    // ������ײ��Ϣ
    void Reset() {
        hit = false;
        t = -1.0f;
        closestPoint = CVector();
        normal = CVector();
    }
};
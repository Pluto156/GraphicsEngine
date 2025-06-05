#pragma once
#include "Shape.h"
#include "SphereCollider.h"

class Sphere : public Shape {
public:
    SphereCollider* sphereCollider;
    // 构造函数
    Sphere(std::string name, float radius, CVector3 Position, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

    void Draw() override;

    float GetRadius();

private:
    float m_radius;
    float r, g, b;
    int slices = 36;  // 经线数
    int stacks = 18;  // 纬线数
};

#pragma once
#include "Shape.h"
#include <memory>  

class Box : public Shape {
public:
    // 构造函数，初始化盒子的长宽高和中心坐标
    Box(std::string name, float width, float height, float depth, CVector Position, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);
    Box(std::string name, float width, float height, float depth, CVector Position, CMatrix Rotation, CEuler EulerAngles, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

    // 绘制盒子
    void Draw() override;

    float GetWidth();
    float GetHeight();
    float GetDepth();
    std::string ToString();

private:
    float m_width, m_height, m_depth;
    float r, g, b;
};

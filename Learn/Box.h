#pragma once
#include "Shape.h"
#include "BoxCollider.h"


class Box : public Shape {
public:

    BoxCollider* boxCollider;
    //OBB* obb;            // 绑定的有向包围盒

    // 构造函数，初始化盒子的长宽高和中心坐标
    Box(std::string name, float width, float height, float depth, CVector3 Position, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);
    Box(std::string name, float width, float height, float depth, CVector3 Position, CMatrix4 Rotation, CEuler EulerAngles, float r = 1, float g = 1, float b = 1, bool isShowLocalAxis = false);

    // 绘制盒子
    void Draw() override;

    float GetWidth();
    float GetHeight();
    float GetDepth();
    //std::string ToString();

private:
    float m_width, m_height, m_depth;
    float r, g, b;
};

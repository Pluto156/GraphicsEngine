#include "stdafx.h"
#include "Box.h"

// 构造函数，初始化盒子的长宽高和中心坐标
Box::Box(std::string name,float width, float height, float depth, CVector Position, float r, float g, float b, bool isShowLocalAxis)
    : m_width(width), m_height(height), m_depth(depth), Shape(name,Position,CMatrix(),CEuler(), isShowLocalAxis), r(r), g(g), b(b){
    obb.halfSizes = CVector(width / 2, height / 2, depth / 2);
    //Draw();
}

// 构造函数，初始化盒子的长宽高和中心坐标
Box::Box(std::string name,float width, float height, float depth, CVector Position, CMatrix Rotation, CEuler EulerAngles, float r, float g, float b, bool isShowLocalAxis)
    : m_width(width), m_height(height), m_depth(depth), Shape(name,Position, Rotation, EulerAngles, isShowLocalAxis), r(r), g(g), b(b) {
    obb.halfSizes = CVector(width / 2, height / 2, depth / 2);
    //Draw();
}

float Box::GetWidth()
{
    return this->m_width;
}

float Box::GetHeight()
{
    return this->m_height;
}

float Box::GetDepth()
{
    return this->m_depth;
}


void Box::Draw() {
    Shape::Draw();
    // 绘制填充的面
    glBegin(GL_QUADS);
    glColor3f(r, g, b);

    // 前面
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    // 后面
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    // 上面
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    // 下面
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    // 左面
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    // 右面
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glEnd();  // 结束绘制填充面

    // 绘制边界
    if (isSelect)
    {
        glColor3f(1.0f, 1.0f, 1.0f);  // 设置边界为白色
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);  // 设置边界为黑色
    }
    glLineWidth(2);  // 设置线条宽度为2

    glBegin(GL_LINES);

    // 前面
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

    // 后面
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

    // 上面
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    // 下面
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

    // 左面
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

    // 右面
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    glEnd();  // 结束绘制边界

}

std::string Box::ToString()
{
    return "name: " + name + "\n position: " + position.ToString() + "\n eulerAngles: " + eulerAngles.ToString() + "\n rotation: " + rotation.ToString()
        + "\n localposition: " + localPosition.ToString() + "\n localEulerAngles: " + localEulerAngles.ToString() + "\n localRotation: " + localRotation.ToString()+"\n";
}


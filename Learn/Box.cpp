#include "stdafx.h"
#include "Box.h"

// 构造函数，初始化盒子的长宽高和中心坐标
Box::Box(std::string name,float width, float height, float depth, CVector3 Position, float r, float g, float b, bool isShowLocalAxis)
    : m_width(width), m_height(height), m_depth(depth), Shape(name,Position,CMatrix4(),CEuler(), isShowLocalAxis), r(r), g(g), b(b){
    boxCollider = AddComponent<BoxCollider>(CVector3(width / 2, height / 2, depth / 2));
    //rigidBody = PhysicsLit::PhysicsManager::Instance().AddGameObject(this);
    /*rigidBody->rigidBodyPrimitive-> AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
    rigidBody->SetVelocity(CVector3(0, 20, 0));*/

}

// 构造函数，初始化盒子的长宽高和中心坐标
Box::Box(std::string name,float width, float height, float depth, CVector3 Position, CMatrix4 Rotation, CEuler EulerAngles, float r, float g, float b, bool isShowLocalAxis)
    : m_width(width), m_height(height), m_depth(depth), Shape(name,Position, Rotation, EulerAngles, isShowLocalAxis), r(r), g(g), b(b) {
    boxCollider = AddComponent<BoxCollider>(CVector3(width / 2, height / 2, depth / 2));
    //rigidBody = PhysicsLit::PhysicsManager::Instance().AddGameObject(this);
    //rigidBody->rigidBodyPrimitive-> AddForceGenerator(new PhysicsLit::ForceGravity(CVector3(0.0f, -9.8f, 0.0f)));
    //rigidBody->SetVelocity(CVector3(0, 20, 0));

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

    GameObject::Draw();
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




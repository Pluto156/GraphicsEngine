#include "stdafx.h"
#include "Box.h"

// ���캯������ʼ�����ӵĳ���ߺ���������
Box::Box(std::string name,float width, float height, float depth, CVector Position, float r, float g, float b, bool isShowLocalAxis)
    : m_width(width), m_height(height), m_depth(depth), Transform(name,Position,CMatrix(),CEuler(), isShowLocalAxis), r(r), g(g), b(b),isSelect(false) {
    obb.halfSizes = CVector(width / 2, height / 2, depth / 2);
    //Draw();
}

// ���캯������ʼ�����ӵĳ���ߺ���������
Box::Box(std::string name,float width, float height, float depth, CVector Position, CMatrix Rotation, CEuler EulerAngles, float r, float g, float b, bool isShowLocalAxis)
    : m_width(width), m_height(height), m_depth(depth), Transform(name,Position, Rotation, EulerAngles, isShowLocalAxis), r(r), g(g), b(b), isSelect(false) {
    obb.halfSizes = CVector(width / 2, height / 2, depth / 2);
    //Draw();
}

// ��̬�������������ڴ���������һ�� Box ����
Box* Box::CreateBox(std::string name,float width, float height, float depth, CVector Position, CMatrix Rotation, CEuler EulerAngles,float r, float g, float b, bool isShowLocalAxis) {
    return new Box(name,width, height, depth, Position,Rotation, EulerAngles,r,g,b, isShowLocalAxis);  // ����������һ�� Box ����
}
Box* Box::CreateBox(std::string name,float width, float height, float depth, CVector Position, float r, float g, float b, bool isShowLocalAxis) {
    return new Box(name,width, height, depth, Position, r, g, b,isShowLocalAxis);  // ����������һ�� Box ����
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


void Box::Draw() const {
    glPushMatrix();  // ���浱ǰ����״̬
    Transform::Draw();



    // ����������
    glBegin(GL_QUADS);
    glColor3f(r, g, b);

    // ǰ��
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    // ����
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glEnd();  // �������������

    // ���Ʊ߽�
    if (isSelect)
    {
        glColor3f(1.0f, 1.0f, 1.0f);  // ���ñ߽�Ϊ��ɫ
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);  // ���ñ߽�Ϊ��ɫ
    }
    glLineWidth(2);  // �����������Ϊ2

    glBegin(GL_LINES);

    // ǰ��
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

    // ����
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(-m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(-m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(-m_width / 2, -m_height / 2, -m_depth / 2);

    // ����
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, -m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, m_depth / 2);
    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);

    glVertex3f(m_width / 2, m_height / 2, -m_depth / 2);
    glVertex3f(m_width / 2, -m_height / 2, -m_depth / 2);

    glEnd();  // �������Ʊ߽�

    glPopMatrix();  // �ָ�����״̬
}

std::string Box::ToString()
{
    return "name: " + name + "\n position: " + position.ToString() + "\n eulerAngles: " + eulerAngles.ToString() + "\n rotation: " + rotation.ToString()
        + "\n localposition: " + localPosition.ToString() + "\n localEulerAngles: " + localEulerAngles.ToString() + "\n localRotation: " + localRotation.ToString()+"\n";
}


#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(std::string name, float radius, CVector3 Position, float r, float g, float b, bool isShowLocalAxis)
    : m_radius(radius), Shape(name, Position, CMatrix4(), CEuler(), isShowLocalAxis), r(r), g(g), b(b) {
    sphereCollider = AddComponent<SphereCollider>(radius);
}

float Sphere::GetRadius() {
    return m_radius;
}

void Sphere::Draw() {
    GameObject::Draw();
    CVector3 localScale = transform->localScale;
    glScalef(localScale.x, localScale.y, localScale.z);

    glColor3f(r, g, b);
    for (int i = 0; i <= stacks; ++i) {
        float lat0 = Math::PI * (-0.5f + (float)(i - 1) / stacks);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = Math::PI * (-0.5f + (float)i / stacks);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float lng = 2 * Math::PI * (float)(j - 1) / slices;
            float x = cos(lng);
            float y = sin(lng);

            glVertex3f(m_radius * x * zr0, m_radius * y * zr0, m_radius * z0);
            glVertex3f(m_radius * x * zr1, m_radius * y * zr1, m_radius * z1);
        }
        glEnd();
    }

    // 绘制边界线（选中状态下）
    if (isSelect) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2);
        glutWireSphere(m_radius, slices, stacks);  // 使用 GLUT 绘制线框球
    }
}

#include "stdafx.h"
#include "CglFont.h"

CglFont::CglFont()
    : m_posX(10), m_posY(20), m_font(GLUT_BITMAP_9_BY_15) {
    m_color[0] = m_color[1] = m_color[2] = 1.0f; // 默认白色
}

void CglFont::SetColor(float r, float g, float b) {
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
}

void CglFont::SetPosition(int x, int y) {
    m_posX = x;
    m_posY = y;
}

void CglFont::SetFont(void* glutFont) {
    m_font = glutFont;
}

void CglFont::DrawString(const std::string& text) const {
    // 保存当前矩阵状态
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // 获取视口尺寸并设置正交投影
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(0, viewport[2], viewport[3], 0); // 左上角为原点

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 设置颜色和位置
    glColor3fv(m_color);
    glRasterPos2i(m_posX, m_posY);

    // 绘制每个字符
    for (const char c : text) {
        glutBitmapCharacter(m_font, c);
    }

    // 恢复矩阵状态
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
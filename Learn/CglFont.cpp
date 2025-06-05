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

    // 设置颜色
    glColor3fv(m_color);

    // 计算行高 (根据字体和需要调整)
    float lineHeight = 20.0f; // 设置每行之间的高度

    // 绘制每个字符
    float cursorX = m_posX;
    float cursorY = m_posY;

    for (const char c : text) {
        if (c == '\n') {
            // 遇到换行符，移动到下一行
            cursorY += lineHeight; // 调整 Y 坐标，表示换行
            cursorX = m_posX;      // 重置 X 坐标
        }
        else {
            // 设置字符绘制的坐标
            glRasterPos2f(cursorX, cursorY);
            glutBitmapCharacter(m_font, c);
            cursorX += glutBitmapWidth(m_font, c); // 增加 X 坐标，为下一个字符留出空间
        }
    }

    // 恢复矩阵状态
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

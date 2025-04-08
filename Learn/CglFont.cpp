#include "stdafx.h"
#include "CglFont.h"

CglFont::CglFont()
    : m_posX(10), m_posY(20), m_font(GLUT_BITMAP_9_BY_15) {
    m_color[0] = m_color[1] = m_color[2] = 1.0f; // Ĭ�ϰ�ɫ
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
    // ���浱ǰ����״̬
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // ��ȡ�ӿڳߴ粢��������ͶӰ
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(0, viewport[2], viewport[3], 0); // ���Ͻ�Ϊԭ��

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // ������ɫ
    glColor3fv(m_color);

    // �����и� (�����������Ҫ����)
    float lineHeight = 20.0f; // ����ÿ��֮��ĸ߶�

    // ����ÿ���ַ�
    float cursorX = m_posX;
    float cursorY = m_posY;

    for (const char c : text) {
        if (c == '\n') {
            // �������з����ƶ�����һ��
            cursorY += lineHeight; // ���� Y ���꣬��ʾ����
            cursorX = m_posX;      // ���� X ����
        }
        else {
            // �����ַ����Ƶ�����
            glRasterPos2f(cursorX, cursorY);
            glutBitmapCharacter(m_font, c);
            cursorX += glutBitmapWidth(m_font, c); // ���� X ���꣬Ϊ��һ���ַ������ռ�
        }
    }

    // �ָ�����״̬
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

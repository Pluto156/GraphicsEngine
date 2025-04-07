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

    // ������ɫ��λ��
    glColor3fv(m_color);
    glRasterPos2i(m_posX, m_posY);

    // ����ÿ���ַ�
    for (const char c : text) {
        glutBitmapCharacter(m_font, c);
    }

    // �ָ�����״̬
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
#include "stdafx.h"
#include "GameObjectManager.h"
void GameObjectManager::Update() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    camera->LookAt();
    glGetDoublev(GL_MODELVIEW_MATRIX, stage->modelViewMatrix);
    drawCoordinateAxes();
    for (auto obj : gameObjects) {
        if (obj) obj->Update();
    }
    glPopMatrix();
    glutSwapBuffers();
}

void GameObjectManager::drawCoordinateAxes()
{
    // X�ᣨ��ɫ��
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);  // ��ɫ
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);  // X��������
    glEnd();

    // Y�ᣨ��ɫ��
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);  // ��ɫ
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);  // Y��������
    glEnd();

    // Z�ᣨ��ɫ��
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);  // ��ɫ
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);  // Z��������
    glEnd();
}



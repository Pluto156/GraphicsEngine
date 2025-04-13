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
    // X轴（红色）
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);  // 红色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);  // X轴正方向
    glEnd();

    // Y轴（绿色）
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);  // 绿色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);  // Y轴正方向
    glEnd();

    // Z轴（蓝色）
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);  // 蓝色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);  // Z轴正方向
    glEnd();
}



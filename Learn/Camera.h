#pragma once
#include "CVector.h"
#include "InputManager.h"
#include "glut.h"
#include "math.h"
#include "corecrt_math_defines.h"
#include <iostream>

class Camera:public GameObject,public InputManager::IInputControl
{
public:
    // 摄像机目标
    CVector camTarget;

    float prevMouseX;
    float prevMouseY;

    // 摄像机旋转的角度
    float camAngleX;
    float camAngleY;

    // 摄像机的运动速度和旋转速度
    float camMoveSpeed;
    float camRotateSpeed;

    bool isControlView;
    int ControlViewMode;//0为欧拉角 1为子坐标

    // 构造函数，初始化摄像机参数
    Camera();
    ~Camera();

    void processKeyboard(unsigned char key, int x, int y)override;
    void processSpecialKeys(int key, int x, int y) override;
    void processMouse(int button, int state, int x, int y)override;
    void processMouseMotion(int x, int y)override;

    void LookAt();

};

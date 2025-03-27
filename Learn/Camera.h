#pragma once
#include "CVector.h"
#include "glut.h"
#include "math.h"
#include "corecrt_math_defines.h"
#include <iostream>

class Camera:public Transform
{
public:
    // 摄像机位置、目标和上向量
    CVector camTarget;
    CVector camUp;
    CVector camForward;
    CVector camRight;

    float prevMouseX;
    float prevMouseY;

    // 摄像机旋转的角度
    float camAngleX;
    float camAngleY;

    // 摄像机的运动速度和旋转速度
    float camMoveSpeed;
    float camRotateSpeed;

    bool isControlView;

    // 构造函数，初始化摄像机参数
    Camera();
    ~Camera();

    CVector GetCamForward();
    CVector GetCamRight();
    CVector GetCamUp();


    void MoveCamera(CVector dir);

    // 摄像机键盘控制
    void processKeyboard(unsigned char key, int x, int y);

    // 摄像机鼠标控制
    void processMouse(int button, int state, int x, int y);
    void processMouseMotion(int x, int y);

    // 更新摄像机视角
    void updateCameraView();

    void CameraDebug();

    void LookAt();
};

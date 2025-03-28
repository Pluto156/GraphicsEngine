#pragma once
#include "CVector.h"
#include "InputManager.h"
#include "glut.h"
#include "math.h"
#include "corecrt_math_defines.h"
#include <iostream>

class Camera:public Transform,public InputManager::IInputControl
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

    void processKeyboard(unsigned char key, int x, int y)override;
    void processMouse(int button, int state, int x, int y)override;
    void processMouseMotion(int x, int y)override;

    // 更新摄像机视角
    void updateCameraView();

    void CameraDebug();

    void LookAt();
};

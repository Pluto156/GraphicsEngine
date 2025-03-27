#pragma once
#include "CVector.h"
#include "glut.h"
#include "math.h"
#include "corecrt_math_defines.h"
#include <iostream>

class Camera:public Transform
{
public:
    // �����λ�á�Ŀ���������
    CVector camTarget;
    CVector camUp;
    CVector camForward;
    CVector camRight;

    float prevMouseX;
    float prevMouseY;

    // �������ת�ĽǶ�
    float camAngleX;
    float camAngleY;

    // ��������˶��ٶȺ���ת�ٶ�
    float camMoveSpeed;
    float camRotateSpeed;

    bool isControlView;

    // ���캯������ʼ�����������
    Camera();
    ~Camera();

    CVector GetCamForward();
    CVector GetCamRight();
    CVector GetCamUp();


    void MoveCamera(CVector dir);

    // ��������̿���
    void processKeyboard(unsigned char key, int x, int y);

    // �����������
    void processMouse(int button, int state, int x, int y);
    void processMouseMotion(int x, int y);

    // ����������ӽ�
    void updateCameraView();

    void CameraDebug();

    void LookAt();
};

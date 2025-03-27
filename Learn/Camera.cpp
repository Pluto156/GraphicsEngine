#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
    : Transform("Camera",CVector(0, 30, 30)), camTarget(0, 0, 0), camUp(0, 1, 0),
    camAngleX(0.0f), camAngleY(0.0f),
    camMoveSpeed(0.1f), camRotateSpeed(0.5f),isControlView(false)
{
    //z
    camForward = camTarget - position;
    camForward.Normalize();
    //x
    camRight = camForward.crossMul(CVector(0, 1, 0));
    camRight.Normalize();
    //y
    camUp = camRight.crossMul(camForward);
    camUp.Normalize();
    // 创建旋转矩阵
    float m[16] = {
        camRight.x, camRight.y, camRight.z, 0,
        camUp.x, camUp.y, camUp.z, 0,
        -camForward.x, -camForward.y, -camForward.z, 0,
        0,          0,          0,          1
    };

    // 初始化旋转矩阵
    rotation.Set(m);
    eulerAngles = (camForward*(-1)).ToEuler();
}


Camera::~Camera()
{
}
CVector Camera::GetCamForward(){
    // 计算并更新前向向量
    camForward = rotation.GetForward()*(-1);
    return camForward;  
}

CVector Camera::GetCamRight() {
    // 计算并更新前向向量
    camRight = rotation.GetRight();
    return camRight;
}

CVector Camera::GetCamUp()
{
    camUp = rotation.GetUp();
    return camUp;
}
void Camera::MoveCamera(CVector dir)
{
    CVector camforward = GetCamForward();
}

void Camera::processKeyboard(unsigned char key, int x, int y)
{
    if (key == 'w')  // 向前
    {
        position = position + camForward*camMoveSpeed;
    }
    if (key == 's')  // 向后
    {
        position = position - camForward * camMoveSpeed;
    }
    if (key == 'a')  // 向左
    {
        position = position - camRight * camMoveSpeed;
    }
    if (key == 'd')  // 向右
    {
        position = position + camRight * camMoveSpeed;
    }
}

void Camera::processMouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            prevMouseX = x;
            prevMouseY = y;
            isControlView = true;
        }
        else
        {
            isControlView = false;
        }
    }
    else
    {
        isControlView = false;
    }
}

void Camera::processMouseMotion(int x, int y)
{
    if (!isControlView)return;

    // 根据鼠标移动调整视角
    camAngleY = (x - prevMouseX) * camRotateSpeed;  // 水平方向旋转
    camAngleX = (y - prevMouseY) * camRotateSpeed;  // 垂直方向旋转

    eulerAngles.h -= camAngleY;
    eulerAngles.p -= camAngleX ;


    // 限制垂直视角的范围，避免摄像机完全翻转
    if (eulerAngles.p > 89.0f) eulerAngles.p = 89.0f;
    if (eulerAngles.p < -89.0f) eulerAngles.p = -89.0f;

    // 更新摄像机目标
    updateCameraView();

    prevMouseX = x;
    prevMouseY = y;
}

void Camera::updateCameraView()
{
    rotation = eulerAngles.ToCMatrix();
    GetCamForward();
    GetCamRight();
    GetCamUp();
}

void Camera::CameraDebug()
{
    std::cout << "CamPos: " << position.ToString() << " CamTarget: " << camTarget.ToString() << " CamForward: " << GetCamForward().ToString() << " CamUp: " << camUp.ToString() << std::endl;
}

void Camera::LookAt()
{
    glRotatef(-eulerAngles.b, 0, 0, 1);
    glRotatef(-eulerAngles.p, 1, 0, 0);
    glRotatef(-eulerAngles.h, 0, 1, 0);
    glTranslatef(-position.x, -position.y, -position.z);
}

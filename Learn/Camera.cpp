#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
    : GameObject("Camera",CVector3(0, 28, 10)), camTarget(0, 0, 0),
    camAngleX(0.0f), camAngleY(0.0f),
    camMoveSpeed(0.1f), camRotateSpeed(0.5f),isControlView(false),ControlViewMode(0)
{
    //z
    transform->Forward = camTarget - transform->position;
    transform->Forward.Normalize();
    //x
    transform->Right = Math::Cross(transform->Forward,CVector3(0, 1, 0));
    transform->Right.Normalize();
    //y
    transform->Up = Math::Cross(transform->Right,transform->Forward);
    transform->Up.Normalize();
    // 创建旋转矩阵
    float m[16] = {
        transform->Right.x, transform->Right.y, transform->Right.z, 0,
        transform->Up.x, transform->Up.y, transform->Up.z, 0,
        -transform->Forward.x, -transform->Forward.y, -transform->Forward.z, 0,
        0,          0,          0,          1
    };

    transform->SetRotation(m);
    transform->SetEulerAngles((transform->Forward).ToEuler());
}


Camera::~Camera()
{
}

void Camera::processKeyboard(unsigned char key, int x, int y)
{
    if (key == 'w')  // 向前
    {
        transform->SetPositionDelta(-transform->Forward * camMoveSpeed);
    }
    if (key == 's')  // 向后
    {
        transform->SetPositionDelta(transform->Forward * camMoveSpeed);
    }
    if (key == 'a')  // 向左
    {
        transform->SetPositionDelta(-transform->Right * camMoveSpeed);
    }
    if (key == 'd')  // 向右
    {
        transform->SetPositionDelta(transform->Right * camMoveSpeed);
    }
    if (key == '1')
    {
        /*if (ControlViewMode == 1)
        {
            SetEulerAngles(eulerAngles.h,eulerAngles.p,0);
        }
        ControlViewMode = ControlViewMode == 0 ? 1 : 0;*/
    }
}

void Camera::processSpecialKeys(int key, int x, int y)
{
    
}

void Camera::processMouse(int button, int state, int x, int y)
{
    if (button == GLUT_MIDDLE_BUTTON)
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
    
    if (ControlViewMode == 0)
    {
        transform->SetEulerAnglesDelta(-camAngleY, -camAngleX, 0);
    }
    else if (ControlViewMode == 1)
    {
        transform->SetRotationDelta(CMatrix4::CreateRotationMatrix(camAngleY, CVector3::Up()));
        transform->SetRotationDelta(CMatrix4::CreateRotationMatrix(camAngleX, CVector3::Right()));
    }

    prevMouseX = x;
    prevMouseY = y;
}

void Camera::LookAt()
{
    if (ControlViewMode == 0)
    {
        glRotatef(-transform->eulerAngles.b, 0, 0, 1);
        glRotatef(-transform->eulerAngles.p, 1, 0, 0);
        glRotatef(-transform->eulerAngles.h, 0, 1, 0);
    }
    else if (ControlViewMode == 1)
    {
        //glMultMatrixf(rotation);
        glRotatef(-transform->eulerAngles.b, 0, 0, 1);
        glRotatef(-transform->eulerAngles.p, 1, 0, 0);
        glRotatef(-transform->eulerAngles.h, 0, 1, 0);
    }
    glTranslatef(-transform->position.x, -transform->position.y, -transform->position.z);
}

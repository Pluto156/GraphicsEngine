#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
    : Transform("Camera",CVector(0, 30, 30)), camTarget(0, 0, 0),
    camAngleX(0.0f), camAngleY(0.0f),
    camMoveSpeed(0.1f), camRotateSpeed(0.5f),isControlView(false),ControlViewMode(0)
{
    //z
    Forward = camTarget - position;
    Forward.Normalize();
    //x
    Right = Forward.crossMul(CVector(0, 1, 0));
    Right.Normalize();
    //y
    Up = Right.crossMul(Forward);
    Up.Normalize();
    // 创建旋转矩阵
    float m[16] = {
        Right.x, Right.y, Right.z, 0,
        Up.x, Up.y, Up.z, 0,
        -Forward.x, -Forward.y, -Forward.z, 0,
        0,          0,          0,          1
    };

    SetRotation(m);
    SetEulerAngles((-Forward).ToEuler());
}


Camera::~Camera()
{
}
void Camera::UpdateFRU(){
    Forward = rotation.GetForward()*(-1);
    Right = rotation.GetRight();
    Up = rotation.GetUp();
}
void Camera::Update() {
    CameraDebug();
}

void Camera::processKeyboard(unsigned char key, int x, int y)
{
    if (key == 'w')  // 向前
    {
        SetPositionDelta(Forward * camMoveSpeed);
    }
    if (key == 's')  // 向后
    {
        SetPositionDelta(-Forward * camMoveSpeed);
    }
    if (key == 'a')  // 向左
    {
        SetPositionDelta(-Right * camMoveSpeed);
    }
    if (key == 'd')  // 向右
    {
        SetPositionDelta(Right * camMoveSpeed);
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
        SetEulerAnglesDelta(-camAngleY, -camAngleX, 0);
    }
    else if (ControlViewMode == 1)
    {
        SetRotationDelta(CMatrix::CreateRotationMatrix(camAngleY, CVector::Up()));
        SetRotationDelta(CMatrix::CreateRotationMatrix(camAngleX, CVector::Right()));
    }

    prevMouseX = x;
    prevMouseY = y;
}

void Camera::CameraDebug()
{
    // 绘制文本
    infoFont.DrawString(cameraInfo);
    // 构建显示信息
    sprintf_s(cameraInfo, "Camera Position: (%.1f, %.1f, %.1f) Camera ControlViewMode:(%d)",
        position.x, position.y, position.z, ControlViewMode);
    //std::cout << "CamPos: " << position.ToString() << " CamTarget: " << camTarget.ToString() << " CamForward: " << Forward.ToString() << " CamUp: " << Up.ToString() << std::endl;
}

void Camera::LookAt()
{
    if (ControlViewMode == 0)
    {
        glRotatef(-eulerAngles.b, 0, 0, 1);
        glRotatef(-eulerAngles.p, 1, 0, 0);
        glRotatef(-eulerAngles.h, 0, 1, 0);
    }
    else if (ControlViewMode == 1)
    {
        //glMultMatrixf(rotation);
        glRotatef(-eulerAngles.b, 0, 0, 1);
        glRotatef(-eulerAngles.p, 1, 0, 0);
        glRotatef(-eulerAngles.h, 0, 1, 0);
    }
    glTranslatef(-position.x, -position.y, -position.z);
}

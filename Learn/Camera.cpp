#include "stdafx.h"
#include "Camera.h"

void Camera::Awake()
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
        transform->Forward.x, transform->Forward.y, transform->Forward.z, 0,
        0,          0,          0,          1
    };

    transform->SetRotation(m);
    transform->SetEulerAngles((transform->Forward).ToEuler());
}


Camera::~Camera()
{
}

void Camera::Update()
{
    auto& input = InputManager::Instance();

    if (input.GetKey(KeyCode::W))  // 向前
    {
        transform->SetPositionDelta(-transform->Forward * camMoveSpeed);
    }
    if (input.GetKey(KeyCode::S))  // 向后
    {
        transform->SetPositionDelta(transform->Forward * camMoveSpeed);
    }
    if (input.GetKey(KeyCode::A))  // 向左
    {
        transform->SetPositionDelta(-transform->Right * camMoveSpeed);
    }
    if (input.GetKey(KeyCode::D))  // 向右
    {
        transform->SetPositionDelta(transform->Right * camMoveSpeed);
    }
    if (input.GetKeyDown(KeyCode::Alpha1))
    {
        /*if (ControlViewMode == 1)
        {
            SetEulerAngles(eulerAngles.h,eulerAngles.p,0);
        }
        ControlViewMode = ControlViewMode == 0 ? 1 : 0;*/
    }

    if (input.GetKey(KeyCode::Mouse2))
    {
        isControlView = true;
    }
    else
    {
        isControlView = false;
    }

    if (!isControlView)return;

    // 根据鼠标移动调整视角
    camAngleY = input.GetAxis("Mouse X") * camRotateSpeed;  // 水平方向旋转
    camAngleX = input.GetAxis("Mouse Y") * camRotateSpeed;  // 垂直方向旋转

    //std::cout << input.GetAxis("Mouse X") <<" "<< input.GetAxis("Mouse Y") << std::endl;

    if (ControlViewMode == 0)
    {
        transform->SetEulerAnglesDelta(-camAngleY, -camAngleX, 0);
    }
    else if (ControlViewMode == 1)
    {
        transform->SetRotationDelta(CMatrix4::CreateRotationMatrix(camAngleY, CVector3::Up()));
        transform->SetRotationDelta(CMatrix4::CreateRotationMatrix(camAngleX, CVector3::Right()));
    }
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

// 把 screenPoint (x,y) 视为窗口像素坐标，screenPoint.z ∈ [0,1] (0=near,1=far)
CVector3 Camera::ScreenToWorldPoint(const CVector3& screenPoint) const
{
    // 获取当前 OpenGL 矩阵与视口
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // OpenGL 的窗口坐标系原点在左下，通常屏幕 y 是从上到下
    double winX = static_cast<double>(screenPoint.x);
    double winY = static_cast<double>(viewport[3] - screenPoint.y); // 翻转 Y
    double winZ = static_cast<double>(screenPoint.z); // 深度 [0,1]

    GLdouble outX, outY, outZ;
    // 使用 GLU 反投影
    if (gluUnProject(winX, winY, winZ, modelview, projection, viewport, &outX, &outY, &outZ) == GL_TRUE)
    {
        return CVector3(static_cast<float>(outX), static_cast<float>(outY), static_cast<float>(outZ));
    }
    // 失败则返回原点（或你可以返回一个标志值）
    return CVector3(0.0f, 0.0f, 0.0f);
}

// 只给出屏幕 XY（像素）。若 useDepthBuffer 为 true，会读取深度缓冲像素得到 screenZ，然后调用上面的函数。
// 如果你想读取鼠标当前位置，可以先通过 InputManager::Instance().GetMousePosition() 得到 CVector2。
CVector3 Camera::ScreenToWorldPoint(const CVector2& screenXY, bool useDepthBuffer) const
{
    float depth = 0.5f; // 默认使用中间深度
    if (useDepthBuffer)
    {
        // 读取深度缓冲中该像素的深度（范围 0..1）
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        // OpenGL 读取的 Y 需要翻转
        GLint readX = static_cast<GLint>(screenXY.x);
        GLint readY = static_cast<GLint>(viewport[3] - screenXY.y);

        GLfloat depthValue = 1.0f;
        // 注意：glReadPixels 在某些上下文中可能很慢，尽量避免每帧多次调用
        glReadPixels(readX, readY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);
        depth = static_cast<float>(depthValue);
    }

    CVector3 sp(screenXY.x, screenXY.y, depth);
    return ScreenToWorldPoint(sp);
}



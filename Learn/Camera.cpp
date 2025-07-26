#include "stdafx.h"
#include "Camera.h"

void Camera::Start()
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

void Camera::OnColliderEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
    //std::cout << gameObject->name << " OnColliderEnter " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive)<< std::endl;

}
void Camera::OnColliderStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
    //std::cout << gameObject->name << " OnColliderStay " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;

}
void Camera::OnColliderExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{
    //std::cout << gameObject->name << " OnColliderExit " << PhysicsLit::PhysicsManager::Instance().GetGameObjectName(rigidBodyPrimitive) << std::endl;

}

void Camera::OnTriggerEnter(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Camera::OnTriggerStay(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}
void Camera::OnTriggerExit(PhysicsLit::RigidBodyPrimitive* rigidBodyPrimitive)
{

}

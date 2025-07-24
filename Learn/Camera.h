#pragma once
#include "CVector3.h"
#include "GameScript.h"
#include "InputManager.h"
#include "glut.h"
#include "math.h"
#include "corecrt_math_defines.h"
#include <iostream>

class Camera:public GameScript
{
public:
    // 摄像机目标
    CVector3 camTarget = CVector3();

    float prevMouseX;
    float prevMouseY;

    // 摄像机旋转的角度
    float camAngleX;
    float camAngleY;

    // 摄像机的运动速度和旋转速度
    float camMoveSpeed = 0.1f;
    float camRotateSpeed = 0.5f;

    bool isControlView = false;
    int ControlViewMode = 0;//0为欧拉角 1为子坐标

    // 构造函数，初始化摄像机参数
    void Start()override;

    ~Camera();
    void Update()override;
    void OnColliderEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderStay(PhysicsLit::RigidBodyPrimitive*);
    void OnColliderExit(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerEnter(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerStay(PhysicsLit::RigidBodyPrimitive*);
    void OnTriggerExit(PhysicsLit::RigidBodyPrimitive*);

    void LookAt();

};

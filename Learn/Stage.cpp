#include "stdafx.h"
#include "Stage.h"
#include "InputManager.h"
#include "LightManager.h"
#include "DebugManager.h"
#include "Math.h"
#include <algorithm> // std::max

// 绘制舞台中的所有区域
void Stage::Update() {

    auto& input = InputManager::Instance();
    if (input.GetKeyDown(KeyCode::L))
    {
        LightManager::Instance().EnableLighting = !LightManager::Instance().EnableLighting;
        LightManager::Instance().InitLighting();  // 切换光照时重新初始化
    }

    if (input.GetKeyDown(KeyCode::Semicolon))
    {
        DebugManager::Instance().EnableGizmos = !DebugManager::Instance().EnableGizmos;
    }

    // 首次有摄像机时保存初始 transform（程序一开始的位置）
    if (!hasSavedInitial && camera != nullptr && camera->transform != nullptr)
    {
        savedInitialPos = camera->transform->position;
        savedInitialQuat = camera->transform->quaternion;
        hasSavedInitial = true;
    }

    if (input.GetKeyDown(KeyCode::Mouse0))
    {
        CVector3 dir = camera->ScreenToWorldPoint(InputManager::Instance().GetMousePosition())-camera->transform->position;
        PhysicsLit::RaycastHit raycastHit;
        if (PhysicsLit::PhysicsManager::Instance().Raycast(camera->transform->position, dir, raycastHit))
        {
            Debug::Log("raycast hit "+ raycastHit.gameObject->name);
            curSelectShape = raycastHit.gameObject;
        }
    }

    // ---------- F1: 移动到 Car ----------
    if (input.GetKeyDown(KeyCode::F1))
    {
        if (camera != nullptr && camera->transform != nullptr && Car != nullptr && Car->transform != nullptr)
        {
            StartMove(Car->transform->position, Car->transform->quaternion);
        }
    }

    // ---------- F2: 移动到 Car2 ----------
    if (input.GetKeyDown(KeyCode::F2))
    {
        if (camera != nullptr && camera->transform != nullptr && Car2 != nullptr && Car2->transform != nullptr)
        {
            StartMove(Car2->transform->position, Car2->transform->quaternion);
        }
    }

    // ---------- F3: 移动回程序开始时的初始摄像机 transform ----------
    if (input.GetKeyDown(KeyCode::F3))
    {
        if (camera != nullptr && camera->transform != nullptr && hasSavedInitial)
        {
            StartMove(savedInitialPos, savedInitialQuat);
        }
    }

    // ---------- 推进插值动画 ----------
    if (isMovingToTarget)
    {
        // 使用工程的 deltaTime（如果命名不同，请替换）
        float deltaTime = TimeManager::deltaTime;
        moveElapsed += deltaTime;
        float t = moveElapsed / std::max(0.000001f, moveDuration);
        if (t >= 1.0f) t = 1.0f;

        // 位置线性插值
        CVector3 newPos = Math::Lerp(moveStartPos, moveTargetPos, t);

        // 旋转 SLERP
        CQuaternion newQuat = moveStartQuat.Slerp(moveTargetQuat, t);

        // 写回摄像机
        camera->transform->SetPosition(newPos);
        camera->transform->SetQuaternion(newQuat);

        // 到达目标时的处理
        if (t >= 1.0f)
        {
            // 精确对齐最终值
            camera->transform->SetPosition(moveTargetPos);
            camera->transform->SetQuaternion(moveTargetQuat);

            // 停止移动
            isMovingToTarget = false;
        }
    }

    StageDebug();
}


void Stage::StageDebug()
{
    // 构建显示信息
    std::ostringstream oss;
    if (camera && camera->transform) {
        oss << "Camera Position: ("
            << camera->transform->position.x << ", "
            << camera->transform->position.y << ", "
            << camera->transform->position.z << ")\n"
            << "Camera ControlViewMode:(" << camera->ControlViewMode << ")\n"
            << "Camera EulerAngles(h,p,b): ("
            << camera->transform->eulerAngles.h << ", "
            << camera->transform->eulerAngles.p << ", "
            << camera->transform->eulerAngles.b << ")\n";
    }
    else {
        oss << "Camera: null\n";
    }

    oss << "Select Shape: ("
        << (curSelectShape ? curSelectShape->name : "null") << ")\n";

    if (Car && Car->transform) {
        oss << "Car Position: ("
            << Car->transform->position.x << ", "
            << Car->transform->position.y << ", "
            << Car->transform->position.z << ")\n";
    }
    if (Car2 && Car2->transform) {
        oss << "Car2 Position: ("
            << Car2->transform->position.x << ", "
            << Car2->transform->position.y << ", "
            << Car2->transform->position.z << ")\n";
    }

    if (curSelectShape)
    {
        oss << "position: ("
            << curSelectShape->transform->position.ToString() << ")\n"
            << "eulerAngles: ("
            << curSelectShape->transform->eulerAngles.ToString() << ")\n"
            << "local position: ("
            << curSelectShape->transform->localPosition.ToString() << ")\n"
            << "local eulerAngles: ("
            << curSelectShape->transform->localEulerAngles.ToString() << ")\n"
            << "parent name: ("
            << (curSelectShape->transform->parent ? curSelectShape->transform->parent->gameObject->name : "null") << ")\n";

        if (curSelectShape->transform->parent != nullptr && curSelectShape->transform->parent->gameObject->GetComponent<RigidBody>() != nullptr)
        {
            auto rigidbody = curSelectShape->GetComponent<RigidBody>()->rigidBodyPrimitive;
            oss << "Velocity: ("
                << rigidbody->GetVelocity().ToString() << ")\n"
                << "AngularVelocity: ("
                << rigidbody->GetAngularVelocity().ToString() << ")\n"
                << "rigidbody Position: ("
                << rigidbody->GetPosition().ToString() << ")\n"
                << "rigidbody Rotation: ("
                << rigidbody->GetRotation().ToCMatrix4().ToEuler().ToString() << ")\n";
        }
    }

    // 绘制文本
    gameObject->infoFont.DrawString(oss.str());

}

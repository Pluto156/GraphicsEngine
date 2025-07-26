#include "stdafx.h"
#include "LightController.h"
void LightController::Update()
{
    auto& input = InputManager::Instance();
    if (input.GetKey(KeyCode::LeftBracket))
    {
        Light->intensity -= 0.02f;
    }
    if (input.GetKey(KeyCode::RightBracket))
    {
        Light->intensity += 0.02f;
    }

    // 控制光源绕 Y 轴旋转
    static float angle = 0.0f;
    angle += 0.5f; // 旋转速度（度/帧），可根据需要调节

    const float radius = 5.0f;    // 绕圈半径
    const float height = 5.0f;    // 光源的高度
    float radians = angle * 3.1415926f / 180.0f;

    // 在 XZ 平面做圆周运动
    transform->position.x = std::cos(radians) * radius;
    transform->position.y = height;
    transform->position.z = std::sin(radians) * radius;
}


#include "stdafx.h"
#include "CEuler.h"
#include <cmath>

void CEuler::Set(float fh, float fp, float fb) {
    this->h = fh;
    this->p = fp;
    this->b = fb;
    Normal();
}

CEuler::CEuler() : h(0), p(0), b(0) {}

CEuler::CEuler(float fh, float fp, float fb) {
    this->h = fh;
    this->p = fp;
    this->b = fb;
    Normal();
}

CVector3 CEuler::ToCVector() {

    float yaw = h * Math::DEG2RAD;
    float pitch = p * Math::DEG2RAD;

    float x = -cos(pitch) * sin(yaw);
    float y = sin(pitch);
    float z = -cos(pitch) * cos(yaw);  // 这里Z轴方向取反

    return CVector3(x, y, z);
}


CMatrix4 CEuler::ToCMatrix() const {
    CMatrix4 mat = CMatrix4::Identity();

    // 检测万向锁：pitch 约等于 ±90°
    if (std::fabs(p) >= 89.999f) {
        // 万向锁：忽略 b，将 h 用作唯一的旋转（绕Y轴）
        mat = CMatrix4::CreateRotationMatrix(h, CVector3::Up());
    }
    else {
        // 正常旋转顺序 Y(h) * X(p) * Z(b)
        CMatrix4 rotationY = CMatrix4::CreateRotationMatrix(h, CVector3::Up());
        CMatrix4 rotationX = CMatrix4::CreateRotationMatrix(p, CVector3::Right());
        CMatrix4 rotationZ = CMatrix4::CreateRotationMatrix(b, CVector3::Forward());
        mat = rotationY * rotationX * rotationZ;
    }

    return mat;
}

CQuaternion CEuler::ToCQuaternion() const {
    float hRad = h * Math::DEG2RAD;  // Yaw
    float pRad = p * Math::DEG2RAD;  // Pitch
    float bRad = b * Math::DEG2RAD;  // Roll

    float ch = cos(hRad * 0.5f);
    float sh = sin(hRad * 0.5f);
    float cp = cos(pRad * 0.5f);
    float sp = sin(pRad * 0.5f);
    float cb = cos(bRad * 0.5f);
    float sb = sin(bRad * 0.5f);

    // 以下为 Yaw-Pitch-Roll（Y-X-Z）顺序的四元数组合
    CQuaternion q;
    q.w = cb * cp * ch + sb * sp * sh;
    q.x = cb * sp * ch + sb * cp * sh;
    q.y = cb * cp * sh - sb * sp * ch;
    q.z = sb * cp * ch - cb * sp * sh;

    return q;
}
void CEuler::Normal() {
    // 将角度规范化到 [-180, 180]
    auto NormalizeAngle = [](float angle) -> float {
        while (angle > 180.0f) angle -= 360.0f;
        while (angle < -180.0f) angle += 360.0f;
        return angle;
        };

    // 步骤 1：标准化每个角度
    h = NormalizeAngle(h);
    p = NormalizeAngle(p);
    b = NormalizeAngle(b);

    // 步骤 2：限制 pitch 到 [-90, 90]
    if (p > 90.0f) {
        p = 180.0f - p;
        h += 180.0f;
        b += 180.0f;
    }
    else if (p < -90.0f) {
        p = -180.0f - p;
        h += 180.0f;
        b += 180.0f;
    }

    // 再次标准化 h 和 b（防止上面加减180后超出范围）
    h = NormalizeAngle(h);
    b = NormalizeAngle(b);

    // 步骤 3：处理万向锁（pitch 接近 ±90°）
    const float gimbalLockEpsilon = 0.001f; // 可调整
    if (fabsf(fabsf(p) - 90.0f) < gimbalLockEpsilon) {
        // pitch 在 ±90 附近，发生万向锁
        // 把 b 的值吸收到 h 中
        h += b;
        h = NormalizeAngle(h);
        b = 0.0f;
    }
}

CEuler CEuler::GetNormal()
{
    CEuler t = CEuler(this->h, this->p, this->b);
    t.Normal();
    return t;
}



std::string CEuler::ToString() {
    return std::to_string(h) + " " + std::to_string(p) + " " + std::to_string(b);
}

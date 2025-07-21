#include "stdafx.h"
#include "CQuaternion.h"

// 设置四元数的值
void CQuaternion::Set(float fw, float fx, float fy, float fz)
{
    w = fw;
    x = fx;
    y = fy;
    z = fz;
}

// 设置四元数为旋转角度和旋转轴
void CQuaternion::SetAngle(float angle, CVector3 axis)
{
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sin(halfAngle);

    w = cos(halfAngle);
    x = axis.x * sinHalfAngle;
    y = axis.y * sinHalfAngle;
    z = axis.z * sinHalfAngle;
}

// 赋值操作符重载
CQuaternion& CQuaternion::operator=(const CQuaternion& p)
{
    if (this != &p)
    {
        w = p.w;
        x = p.x;
        y = p.y;
        z = p.z;
    }
    return *this;
}

// 四元数加法
CQuaternion CQuaternion::operator+(const CQuaternion& p)const
{
    CQuaternion result;
    result.w = w + p.w;
    result.x = x + p.x;
    result.y = y + p.y;
    result.z = z + p.z;
    return result;
}

CQuaternion CQuaternion::operator+=(const CQuaternion& p)
{
    w += p.w;
    x += p.x;
    y += p.y;
    z += p.z;
    return *this;
}

// 四元数减法操作符重载
CQuaternion CQuaternion::operator-(const CQuaternion& p) const
{
    CQuaternion result;
    result.w = w - p.w;
    result.x = x - p.x;
    result.y = y - p.y;
    result.z = z - p.z;
    return result;
}

CQuaternion CQuaternion::operator-=(const CQuaternion& p)
{
    w -= p.w;
    x -= p.x;
    y -= p.y;
    z -= p.z;
    return *this;
}

void CQuaternion::AddScaledVector(const CVector3& vector, float scale)
{
    CQuaternion q(0,
        vector.x * scale,
        vector.y * scale,
        vector.z * scale);
    q *= *this;
    w += q.w * 0.5f;
    x += q.x * 0.5f;
    y += q.y * 0.5f;
    z += q.z * 0.5f;
}


// 四元数与标量乘法
CQuaternion CQuaternion::operator*(float data)
{
    CQuaternion result;
    result.w = w * data;
    result.x = x * data;
    result.y = y * data;
    result.z = z * data;
    return result;
}

// 四元数乘法
CQuaternion CQuaternion::operator*(const CQuaternion& p)
{
    CQuaternion result;
    result.w = w * p.w - x * p.x - y * p.y - z * p.z;
    result.x = w * p.x + x * p.w + y * p.z - z * p.y;
    result.y = w * p.y + y * p.w + z * p.x - x * p.z;
    result.z = w * p.z + z * p.w + x * p.y - y * p.x;
    return result;
}

CQuaternion CQuaternion::operator*=(const CQuaternion& p)
{
    w = w * p.w - x * p.x - y * p.y - z * p.z;
    x = w * p.x + x * p.w + y * p.z - z * p.y;
    y = w * p.y + y * p.w + z * p.x - x * p.z;
    z = w * p.z + z * p.w + x * p.y - y * p.x;
    return *this;
}

// 四元数点乘
float CQuaternion::dotMul(const CQuaternion& p)
{
    return w * p.w + x * p.x + y * p.y + z * p.z;
}

// 计算四元数模
float CQuaternion::len()
{
    return sqrt(w * w + x * x + y * y + z * z);
}

// 标准化四元数
BOOL CQuaternion::Normalize()
{
    float length = len();
    if (length == 0)
    {
        return FALSE;
    }

    float invLen = 1.0f / length;
    w *= invLen;
    x *= invLen;
    y *= invLen;
    z *= invLen;
    return TRUE;
}

CQuaternion CQuaternion::GetNormalize()
{
    CQuaternion t = *this;
    t.Normalize();
    return t;
}

CQuaternion& CQuaternion::Inverse()
{
    float normSquared = w * w + x * x + y * y + z * z;
    if (normSquared > 1e-6f) // 防止除以 0
    {
        float invNorm = 1.0f / normSquared;
        w *= invNorm;
        x *= -invNorm;
        y *= -invNorm;
        z *= -invNorm;
    }
    else
    {
        // 长度太小，不可逆，设为单位四元数或其他默认处理
        w = 1.0f;
        x = y = z = 0.0f;
    }
    return *this;
}


// 获取四元数的逆（不改变当前四元数）
CQuaternion CQuaternion::GetInverse() const
{
    CQuaternion result = *this;
    result.Inverse();
    return result;
}

// 求四元数的差：a - b
CQuaternion CQuaternion::Div(const CQuaternion& b)
{
    CQuaternion t = b;
    CQuaternion aInverse = GetInverse();
    return t * aInverse;
}

void CQuaternion::GetAngle(float& angle, CVector3& axis)
{
    angle = 2.0f * acos(w);  // 以弧度计算
    float s = sqrt(1.0f - w * w);

    if (s < 0.001f)
    {
        // 旋转角度非常小，任意指定一个轴
        axis.x = 1.0f; axis.y = 0.0f; axis.z = 0.0f;
    }
    else
    {
        axis.x = x / s;
        axis.y = y / s;
        axis.z = z / s;
    }

    // 将角度从弧度转换为角度制（度）
    constexpr float RAD2DEG = 180.0f / 3.14159265358979323846f;
    angle *= RAD2DEG;
}


void CQuaternion::Rotate(const CVector3& axis, float angle)
{
    if (axis.x <= 0.01 && axis.y <= 0.01 && axis.z <= 0.01)return;
    float halfAngle = angle * 0.5f;
    float s = sin(halfAngle);

    w = cos(halfAngle);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
}


CQuaternion CQuaternion::Slerp(const CQuaternion& Vend, float t)
{

    CQuaternion q1 = *this;
    CQuaternion q2 = Vend;
    q1.Normalize();
    q2.Normalize();

    float dot = q1.dotMul(q2);
    CQuaternion v1 = q2;

    // 如果dot < 0，则取反目标四元数，保证短路径插值
    if (dot < 0.0f)
    {
        v1 = -q2;
        dot = -dot;
    }

    const float THRESHOLD = 0.9995f;

    if (dot > THRESHOLD)
    {
        // 如果两四元数非常接近，退化为线性插值
        CQuaternion result = q1 + (v1 - q1) * t;
        result.Normalize();
        return result;
    }

    // 标准SLERP公式
    dot = fmin(fmax(dot, -1.0f), 1.0f);  // clamp到[-1,1]
    float theta_0 = acos(dot);          // 起始角度

    CQuaternion result = (q1) * (theta_0 *sin(1-t)/sin(theta_0)) + v1 * (sin(t* theta_0)/ sin(theta_0));
    result.Normalize(); // 最后归一化
    return result;
}


// 四元数插值，生成n个数据
void CQuaternion::Slerp(const CQuaternion& Vend, int n, float* t, CQuaternion* Result)
{
    for (int i = 0; i < n; ++i)
    {
        Result[i] = Slerp(Vend, t[i]);
    }
}

CMatrix4 CQuaternion::ToCMatrix4()
{
    CMatrix4 t;
    t.m00 = 1 - 2 * y * y - 2 * z * z;
    t.m01 = 2 * x * y - 2 * w * z;
    t.m02 = 2 * x * z + 2 * w * y;

    t.m10 = 2 * x * y + 2 * w * z;
    t.m11 = 1 - 2 * x * x - 2 * z * z;
    t.m12 = 2 * y * z - 2 * w * x;

    t.m20 = 2 * x * z - 2 * w * y;
    t.m21 = 2 * y * z + 2 * w * x;
    t.m22 = 1 - 2 * x * x - 2 * y * y;
    return t;
}

CEuler CQuaternion::ToCEuler()
{
    CEuler euler;

    float sinp = 2.0f * (w * x - y * z);
    // Clamp sinp to [-1,1] to avoid NaNs from asin
    sinp = Math::Clamp(sinp, -1.0f, 1.0f);
    euler.p = Math::Rad2Deg(std::asin(sinp));  // p

    float cosp = std::cos(euler.p);

    if (std::abs(cosp) > 1e-6f) // avoid gimbal lock
    {
        euler.h = Math::Rad2Deg(std::atan2(2.0f * (z * x + w * y),
            1.0f - 2.0f * (x * x + y * y)));  // h
        euler.b = Math::Rad2Deg(std::atan2(2.0f * (x * y + w * z),
            1.0f - 2.0f * (z * z + x * x)));     // b
    }
    else
    {
        euler.h = Math::Rad2Deg(std::atan2(2.0f * (w * y - z * x),
            1.0f - 2.0f * (y * y + z * z)));  // simplified h
        euler.b = 0.0f;
    }

    return euler;
}




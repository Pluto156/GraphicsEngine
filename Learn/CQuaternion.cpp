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
void CQuaternion::SetAngle(float angle, CVector axis)
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

void CQuaternion::AddScaledVector(const CVector& vector, float scale)
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

// 计算四元数的逆（改变当前四元数）
CQuaternion& CQuaternion::Inverse()
{
    float lengthSquared = w * w + x * x + y * y + z * z;
    if (lengthSquared > 0)
    {
        float invLen = 1.0f / lengthSquared;
        w = w * invLen;
        x = -x * invLen;
        y = -y * invLen;
        z = -z * invLen;
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
    CQuaternion bInverse = b.GetInverse();
    return *this * bInverse;
}

// 获取旋转角度和旋转轴
void CQuaternion::GetAngle(float& angle, CVector& axis)
{
    angle = 2.0f * acos(w);
    float s = sqrt(1.0f - w * w);
    if (s < 0.001f)
    {
        axis.x = 1.0f; axis.y = 0.0f; axis.z = 0.0f;
    }
    else
    {
        axis.x = x / s;
        axis.y = y / s;
        axis.z = z / s;
    }
}

// 四元数的Slerp插值
CQuaternion CQuaternion::Slerp(const CQuaternion& Vend, float t)
{
    // 计算四元数之间的夹角
    float dot = dotMul(Vend);
    const float THRESHOLD = 0.9995f;

    if (dot > THRESHOLD)
    {
        CQuaternion result = *this + (Vend - *this) * t;
        result.Normalize();
        return result;
    }

    dot = fmin(fmax(dot, -1.0f), 1.0f);
    float theta_0 = acos(dot);
    float theta = theta_0 * t;

    CQuaternion VendTemp = Vend - *this * dot;
    VendTemp.Normalize();

    CQuaternion result = *this * cos(theta) + VendTemp * sin(theta);
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

CMatrix CQuaternion::ToMatrix()
{
    CMatrix t;
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


#include "stdafx.h"
#include "CVector4.h"

// 默认构造函数，初始化向量为(0, 0, 0, 0)
CVector4::CVector4() : x(0), y(0), z(0), w(0) {}

// 构造函数，初始化向量为指定的x, y, z, w
CVector4::CVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

CVector4::CVector4(const CVector3& v3)
{
    this->x = v3.x;
    this->y = v3.y;
    this->z = v3.z;
    this->w = 0.0f;
}
CVector4::CVector4(const CVector3& v3, float w)
{
    this->x = v3.x;
    this->y = v3.y;
    this->z = v3.z;
    this->w = w;
}
// 析构函数
CVector4::~CVector4() {}

// 向量减法
CVector4 CVector4::operator-(const CVector4& v) const {
    return CVector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

CVector4 CVector4::operator-=(const CVector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

// 向量加法
CVector4 CVector4::operator+(const CVector4& v) const {
    return CVector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

CVector4 CVector4::operator+=(const CVector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

// 赋值操作符重载
CVector4& CVector4::operator=(const CVector4& v)
{
    if (this != &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }
    return *this;
}

// 向量数乘操作
CVector4 CVector4::operator*(float scalar) const {
    return CVector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

CVector4 CVector4::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

CVector4 CVector4::operator/(float scalar) const {
    return CVector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

CVector4 CVector4::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

float& CVector4::operator[] (int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        //Debug::LogError("Invalid index to Vector4 !");
        //assert(false);
        return x;
        break;
    }
}

const float& CVector4::operator[] (int i) const
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        //Debug::LogError("Invalid index to Vector4 !");
        //assert(false);
        return x;
        break;
    }
}

// 友元函数：支持 100 * CVector4 的形式
CVector4 operator*(float scalar, const CVector4& v) {
    return CVector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

// 向量单位化
void CVector4::Normalize()
{
    float length = len();
    if (length > 0)
    {
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }
}

CVector4 CVector4::GetNormalize() const
{
    CVector4 t = *this;
    t.Normalize();
    return t;
}

// 向量求模
float CVector4::len() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

float CVector4::lenSquared() const
{
    return x * x + y * y + z * z + w * w;
}

// 向量取绝对值
void CVector4::Absolutize()
{
    x = fabsf(x);
    y = fabsf(y);
    z = fabsf(z);
    w = fabsf(w);
}

std::string CVector4::ToString() const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
}

// 转换为欧拉角
CEuler CVector4::ToEuler() const
{
    CEuler euler;

    // 计算 pitch
    euler.h = atan2(x, z);

    // 计算 roll (绕 X 轴的旋转角)
    euler.p = -atan2(y, sqrt(x * x + z * z));

    // 假设 yaw (绕 Z 轴的旋转角) 为 0
    euler.b = 0;
    euler.h = euler.h * 180 / M_PI;
    euler.p = euler.p * 180 / M_PI;
    euler.b = euler.b * 180 / M_PI;
    return euler;
}

// 转换为矩阵
CMatrix4 CVector4::ToCMatrix() const
{
    return ToEuler().ToCMatrix();
}

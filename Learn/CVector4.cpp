#include "stdafx.h"
#include "CVector4.h"

// Ĭ�Ϲ��캯������ʼ������Ϊ(0, 0, 0, 0)
CVector4::CVector4() : x(0), y(0), z(0), w(0) {}

// ���캯������ʼ������Ϊָ����x, y, z, w
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
// ��������
CVector4::~CVector4() {}

// ��������
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

// �����ӷ�
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

// ��ֵ����������
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

// �������˲���
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

// ��Ԫ������֧�� 100 * CVector4 ����ʽ
CVector4 operator*(float scalar, const CVector4& v) {
    return CVector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

// ������λ��
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

// ������ģ
float CVector4::len() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

float CVector4::lenSquared() const
{
    return x * x + y * y + z * z + w * w;
}

// ����ȡ����ֵ
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

// ת��Ϊŷ����
CEuler CVector4::ToEuler() const
{
    CEuler euler;

    // ���� pitch
    euler.h = atan2(x, z);

    // ���� roll (�� X �����ת��)
    euler.p = -atan2(y, sqrt(x * x + z * z));

    // ���� yaw (�� Z �����ת��) Ϊ 0
    euler.b = 0;
    euler.h = euler.h * 180 / M_PI;
    euler.p = euler.p * 180 / M_PI;
    euler.b = euler.b * 180 / M_PI;
    return euler;
}

// ת��Ϊ����
CMatrix4 CVector4::ToCMatrix() const
{
    return ToEuler().ToCMatrix();
}

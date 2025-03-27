#include "stdafx.h"
#include "CQuaternion.h"

// ������Ԫ����ֵ
void CQuaternion::Set(float fw, float fx, float fy, float fz)
{
    w = fw;
    x = fx;
    y = fy;
    z = fz;
}

// ������Ԫ��Ϊ��ת�ǶȺ���ת��
void CQuaternion::SetAngle(float angle, CVector axis)
{
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sin(halfAngle);

    w = cos(halfAngle);
    x = axis.x * sinHalfAngle;
    y = axis.y * sinHalfAngle;
    z = axis.z * sinHalfAngle;
}

// ��ֵ����������
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

// ��Ԫ���ӷ�
CQuaternion CQuaternion::operator+(const CQuaternion& p)
{
    CQuaternion result;
    result.w = w + p.w;
    result.x = x + p.x;
    result.y = y + p.y;
    result.z = z + p.z;
    return result;
}

// ��Ԫ����������������
CQuaternion CQuaternion::operator-(const CQuaternion& p) const
{
    CQuaternion result;
    result.w = w - p.w;
    result.x = x - p.x;
    result.y = y - p.y;
    result.z = z - p.z;
    return result;
}


// ��Ԫ��������˷�
CQuaternion CQuaternion::operator*(float data)
{
    CQuaternion result;
    result.w = w * data;
    result.x = x * data;
    result.y = y * data;
    result.z = z * data;
    return result;
}

// ��Ԫ���˷�
CQuaternion CQuaternion::operator*(const CQuaternion& p)
{
    CQuaternion result;
    result.w = w * p.w - x * p.x - y * p.y - z * p.z;
    result.x = w * p.x + x * p.w + y * p.z - z * p.y;
    result.y = w * p.y + y * p.w + z * p.x - x * p.z;
    result.z = w * p.z + z * p.w + x * p.y - y * p.x;
    return result;
}

// ��Ԫ�����
float CQuaternion::dotMul(const CQuaternion& p)
{
    return w * p.w + x * p.x + y * p.y + z * p.z;
}

// ������Ԫ��ģ
float CQuaternion::len()
{
    return sqrt(w * w + x * x + y * y + z * z);
}

// ��׼����Ԫ��
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

// ������Ԫ�����棨�ı䵱ǰ��Ԫ����
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

// ��ȡ��Ԫ�����棨���ı䵱ǰ��Ԫ����
CQuaternion CQuaternion::GetInverse() const
{
    CQuaternion result = *this;
    result.Inverse();
    return result;
}

// ����Ԫ���Ĳa - b
CQuaternion CQuaternion::Div(const CQuaternion& b)
{
    CQuaternion bInverse = b.GetInverse();
    return *this * bInverse;
}

// ��ȡ��ת�ǶȺ���ת��
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

// ��Ԫ����Slerp��ֵ
CQuaternion CQuaternion::Slerp(const CQuaternion& Vend, float t)
{
    // ������Ԫ��֮��ļн�
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

// ��Ԫ����ֵ������n������
void CQuaternion::Slerp(const CQuaternion& Vend, int n, float* t, CQuaternion* Result)
{
    for (int i = 0; i < n; ++i)
    {
        Result[i] = Slerp(Vend, t[i]);
    }
}


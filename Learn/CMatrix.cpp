#include "stdafx.h"
#include "CMatrix.h"


// 默认构造函数，初始化为单位矩阵
CMatrix::CMatrix()
{
    m00 = 1.0f; m10 = 0.0f; m20 = 0.0f; m30 = 0.0f;
    m01 = 0.0f; m11 = 1.0f; m21 = 0.0f; m31 = 0.0f;
    m02 = 0.0f; m12 = 0.0f; m22 = 1.0f; m32 = 0.0f;
    m03 = 0.0f; m13 = 0.0f; m23 = 0.0f; m33 = 1.0f;
}

CMatrix::CMatrix(float* val)
{
    CMatrix::Set(val);
}

// 设置矩阵的16个元素
void CMatrix::Set(float* p)
{
    m00 = p[0]; m10 = p[1]; m20 = p[2]; m30 = p[3];
    m01 = p[4]; m11 = p[5]; m21 = p[6]; m31 = p[7];
    m02 = p[8]; m12 = p[9]; m22 = p[10]; m32 = p[11];
    m03 = p[12]; m13 = p[13]; m23 = p[14]; m33 = p[15];
}

// 矩阵赋值
CMatrix& CMatrix::operator=(const CMatrix& p)
{
    if (this != &p)
    {
        m00 = p.m00; m10 = p.m10; m20 = p.m20; m30 = p.m30;
        m01 = p.m01; m11 = p.m11; m21 = p.m21; m31 = p.m31;
        m02 = p.m02; m12 = p.m12; m22 = p.m22; m32 = p.m32;
        m03 = p.m03; m13 = p.m13; m23 = p.m23; m33 = p.m33;
    }
    return *this;
}

// 矩阵与标量相乘
CMatrix CMatrix::operator*(float d)
{
    CMatrix result;
    result.m00 = m00 * d; result.m10 = m10 * d; result.m20 = m20 * d; result.m30 = m30 * d;
    result.m01 = m01 * d; result.m11 = m11 * d; result.m21 = m21 * d; result.m31 = m31 * d;
    result.m02 = m02 * d; result.m12 = m12 * d; result.m22 = m22 * d; result.m32 = m32 * d;
    result.m03 = m03 * d; result.m13 = m13 * d; result.m23 = m23 * d; result.m33 = m33 * d;
    return result;
}

// 矩阵相乘（列优先）
CMatrix CMatrix::operator*(const CMatrix& p)
{
    CMatrix result;
    result.m00 = m00 * p.m00 + m01 * p.m10 + m02 * p.m20 + m03 * p.m30;
    result.m10 = m10 * p.m00 + m11 * p.m10 + m12 * p.m20 + m13 * p.m30;
    result.m20 = m20 * p.m00 + m21 * p.m10 + m22 * p.m20 + m23 * p.m30;
    result.m30 = m30 * p.m00 + m31 * p.m10 + m32 * p.m20 + m33 * p.m30;

    result.m01 = m00 * p.m01 + m01 * p.m11 + m02 * p.m21 + m03 * p.m31;
    result.m11 = m10 * p.m01 + m11 * p.m11 + m12 * p.m21 + m13 * p.m31;
    result.m21 = m20 * p.m01 + m21 * p.m11 + m22 * p.m21 + m23 * p.m31;
    result.m31 = m30 * p.m01 + m31 * p.m11 + m32 * p.m21 + m33 * p.m31;

    result.m02 = m00 * p.m02 + m01 * p.m12 + m02 * p.m22 + m03 * p.m32;
    result.m12 = m10 * p.m02 + m11 * p.m12 + m12 * p.m22 + m13 * p.m32;
    result.m22 = m20 * p.m02 + m21 * p.m12 + m22 * p.m22 + m23 * p.m32;
    result.m32 = m30 * p.m02 + m31 * p.m12 + m32 * p.m22 + m33 * p.m32;

    result.m03 = m00 * p.m03 + m01 * p.m13 + m02 * p.m23 + m03 * p.m33;
    result.m13 = m10 * p.m03 + m11 * p.m13 + m12 * p.m23 + m13 * p.m33;
    result.m23 = m20 * p.m03 + m21 * p.m13 + m22 * p.m23 + m23 * p.m33;
    result.m33 = m30 * p.m03 + m31 * p.m13 + m32 * p.m23 + m33 * p.m33;

    return result;
}

// 矩阵与向量相乘（只考虑旋转部分）
CVector CMatrix::vecMul(const CVector& p)
{
    CVector result;
    result.x = m00 * p.x + m01 * p.y + m02 * p.z;
    result.y = m10 * p.x + m11 * p.y + m12 * p.z;
    result.z = m20 * p.x + m21 * p.y + m22 * p.z;
    return result;
}

// 矩阵与位置向量相乘（考虑平移）
CVector CMatrix::posMul(const CVector& p)
{
    CVector result;
    result.x = m00 * p.x + m01 * p.y + m02 * p.z + m03;
    result.y = m10 * p.x + m11 * p.y + m12 * p.z + m13;
    result.z = m20 * p.x + m21 * p.y + m22 * p.z + m23;
    return result;
}

void CMatrix::SetRotate(float angle, const CVector& axis)
{
    CVector t = axis;
    t.Normalize();
    angle = (angle)*M_PI / 180;
    float costheta = cos(angle);
    float sintheta = sin(angle);

    m00 = t.x* t.x*(1-costheta)+ costheta; m10 = t.x * t.y * (1 - costheta) + t.z*sintheta; m20 = t.x * t.z * (1 - costheta) - t.y * sintheta; m30 = 0;
    m01 = t.x * t.y * (1 - costheta) - t.z * sintheta; m11 = t.y * t.y * (1 - costheta) + costheta; m21 = t.y * t.z * (1 - costheta) + t.x * sintheta; m31 = 0;
    m02 = t.x * t.z * (1 - costheta) + t.y * sintheta; m12 = t.y * t.z * (1 - costheta) - t.x * sintheta; m22 = t.z * t.z * (1 - costheta) + costheta; m32 = 0;
    m03 = 0; m13 =0; m23 = 0; m33 = 1;
}

void CMatrix::SetTrans(const CVector& trans) {
    m00 = 1.0f; m01 = 0.0f; m02 = 0.0f; m03 = trans.x;
    m10 = 0.0f; m11 = 1.0f; m12 = 0.0f; m13 = trans.y;
    m20 = 0.0f; m21 = 0.0f; m22 = 1.0f; m23 = trans.z;
    m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
}

void CMatrix::SetScale(const CVector& scale) {
    m00 = scale.x; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
    m10 = 0.0f; m11 = scale.y; m12 = 0.0f; m13 = 0.0f;
    m20 = 0.0f; m21 = 0.0f; m22 = scale.z; m23 = 0.0f;
    m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
}

float CMatrix::Inverse()
{
    float det = Determinant();
    if (det == 0) {
        return 0;
    }
    this->operator=(Adjoint() * (1.0f / det));
    return det;
}

// 计算矩阵行列式（列优先存储）
float CMatrix::Determinant()
{
    return m00 * (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31)) -
        m01 * (m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30)) +
        m02 * (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30)) -
        m03 * (m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30));
}

// 计算伴随矩阵（列优先）
CMatrix CMatrix::Adjoint()
{
    CMatrix adj;

    adj.m00 = (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31));
    adj.m01 = -(m01 * (m22 * m33 - m23 * m32) - m02 * (m21 * m33 - m23 * m31) + m03 * (m21 * m32 - m22 * m31));
    adj.m02 = (m01 * (m12 * m33 - m13 * m32) - m02 * (m11 * m33 - m13 * m31) + m03 * (m11 * m32 - m12 * m31));
    adj.m03 = -(m01 * (m12 * m23 - m13 * m22) - m02 * (m11 * m23 - m13 * m21) + m03 * (m11 * m22 - m12 * m21));

    adj.m10 = -(m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30));
    adj.m11 = (m00 * (m22 * m33 - m23 * m32) - m02 * (m20 * m33 - m23 * m30) + m03 * (m20 * m32 - m22 * m30));
    adj.m12 = -(m00 * (m12 * m33 - m13 * m32) - m02 * (m10 * m33 - m13 * m30) + m03 * (m10 * m32 - m12 * m30));
    adj.m13 = (m00 * (m12 * m23 - m13 * m22) - m02 * (m10 * m23 - m13 * m20) + m03 * (m10 * m22 - m12 * m20));

    adj.m20 = (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30));
    adj.m21 = -(m00 * (m21 * m33 - m23 * m31) - m01 * (m20 * m33 - m23 * m30) + m03 * (m20 * m31 - m21 * m30));
    adj.m22 = (m00 * (m11 * m33 - m13 * m31) - m01 * (m10 * m33 - m13 * m30) + m03 * (m10 * m31 - m11 * m30));
    adj.m23 = -(m00 * (m11 * m23 - m13 * m21) - m01 * (m10 * m23 - m13 * m20) + m03 * (m10 * m21 - m11 * m20));

    adj.m30 = -(m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30));
    adj.m31 = (m00 * (m21 * m32 - m22 * m31) - m01 * (m20 * m32 - m22 * m30) + m02 * (m20 * m31 - m21 * m30));
    adj.m32 = -(m00 * (m11 * m32 - m12 * m31) - m01 * (m10 * m32 - m12 * m30) + m02 * (m10 * m31 - m11 * m30));
    adj.m33 = (m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20));

    return adj;
}

// 求逆矩阵
CMatrix CMatrix::GetInverse()
{
    float det = Determinant();
    if (det == 0) {
        throw std::runtime_error("Matrix is singular and cannot be inverted");
    }
    return Adjoint() * (1.0f / det);
}

void CMatrix::Transpose()
{
    std::swap(m10, m01);
    std::swap(m20, m02);
    std::swap(m30, m03);
    std::swap(m21, m12);
    std::swap(m31, m13);
    std::swap(m32, m23);


}
CMatrix CMatrix::GetTranspose()
{
    CMatrix t = *this;
    std::swap(t.m10, t.m01);
    std::swap(t.m20, t.m02);
    std::swap(t.m30, t.m03);
    std::swap(t.m21, t.m12);
    std::swap(t.m31, t.m13);
    std::swap(t.m32, t.m23);
    return t;
}


CMatrix CMatrix::CreateRotationMatrix(float angle, const CVector& axis) {
    CVector t = axis;
    t.Normalize();
    CMatrix mat;
    angle = (angle)*M_PI / 180;
    float costheta = cos(angle);
    float sintheta = sin(angle);

    mat.m00 = t.x * t.x * (1 - costheta) + costheta; mat.m10 = t.x * t.y * (1 - costheta) + t.z * sintheta; mat.m20 = t.x * t.z * (1 - costheta) - t.y * sintheta; mat.m30 = 0;
    mat.m01 = t.x * t.y * (1 - costheta) - t.z * sintheta; mat.m11 = t.y * t.y * (1 - costheta) + costheta; mat.m21 = t.y * t.z * (1 - costheta) + t.x * sintheta; mat.m31 = 0;
    mat.m02 = t.x * t.z * (1 - costheta) + t.y * sintheta; mat.m12 = t.y * t.z * (1 - costheta) - t.x * sintheta; mat.m22 = t.z * t.z * (1 - costheta) + costheta; mat.m32 = 0;
    mat.m03 = 0; mat.m13 = 0; mat.m23 = 0; mat.m33 = 1;

    return mat;
}

CMatrix CMatrix::CreateRotationMatrix(const CVector& lookDir)
{
    return lookDir.ToCMatrix();
}


CVector CMatrix::GetForward()
{
    return CVector(m02, m12, m22);
}
CVector CMatrix::GetUp()
{
    return CVector(m01, m11, m21);
}
CVector CMatrix::GetRight()
{
    return CVector(m00, m10, m20);
}
CEuler CMatrix::ToEuler()
{
    CEuler euler;
    euler.p = asin(-m12);  
    euler.h = atan2(m02,m22);
    euler.b = atan2(m10, m11);
    euler.h = euler.h * 180 / M_PI;
    euler.p = euler.p * 180 / M_PI;
    euler.b = euler.b * 180 / M_PI;
    return euler;
}
CQuaternion CMatrix::ToQuaternion()
{
    CQuaternion quaternion;
    float w, x, y, z;
    w = sqrtf(m00 + m11 + m22+1) /2;
    x = (m21 - m12) / 4 * w;
    y = (m02 - m20) / 4 * w;
    z = (m10 - m01) / 4 * w;
    quaternion.w = w;
    quaternion.x = x;
    quaternion.y = y;
    quaternion.z = z;
    return quaternion;
}


std::string CMatrix::ToString()
{
    return "\n"+std::to_string(m00) + " " + std::to_string(m01) + " " + std::to_string(m02) + " " + std::to_string(m03) + "\n" +
        std::to_string(m10) + " " + std::to_string(m11) + " " + std::to_string(m12) + " " + std::to_string(m13) + "\n" +
        std::to_string(m20) + " " + std::to_string(m21) + " " + std::to_string(m22) + " " + std::to_string(m23) + "\n" +
        std::to_string(m30) + " " + std::to_string(m31) + " " + std::to_string(m32) + " " + std::to_string(m33) + "\n";
}

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

CMatrix CMatrix::Identity() {
    float m[16] = {
            1,       0,        0,      0,
            0,       1,        0,      0,
            0,       0,        1,      0,
            0,       0,        0,      1
        };
    CMatrix identity(m);
    return identity;
}

// 零矩阵
CMatrix CMatrix::Zero() {
    CMatrix zero;
    return zero;
}

CMatrix CMatrix::CreateRotationMatrixX(double angleInRadians) {
    CMatrix mat;

    // 填充绕X轴的旋转矩阵（列优先）
    mat.m00 = 1; mat.m10 = 0;                      mat.m20 = 0;                     mat.m30 = 0;
    mat.m01 = 0; mat.m11 = cos(angleInRadians);   mat.m21 = sin(angleInRadians);  mat.m31 = 0;
    mat.m02 = 0; mat.m12 = -sin(angleInRadians);  mat.m22 = cos(angleInRadians);  mat.m32 = 0;
    mat.m03 = 0; mat.m13 = 0;                      mat.m23 = 0;                     mat.m33 = 1;

    return mat;
}

CMatrix CMatrix::CreateRotationMatrixY(double angleInRadians) {
    CMatrix mat;

    // 填充绕Y轴的旋转矩阵（列优先）
    mat.m00 = cos(angleInRadians);  mat.m10 = 0; mat.m20 = -sin(angleInRadians); mat.m30 = 0;
    mat.m01 = 0;                    mat.m11 = 1; mat.m21 = 0;                      mat.m31 = 0;
    mat.m02 = sin(angleInRadians);  mat.m12 = 0; mat.m22 = cos(angleInRadians);  mat.m32 = 0;
    mat.m03 = 0;                    mat.m13 = 0; mat.m23 = 0;                      mat.m33 = 1;

    return mat;
}

CMatrix CMatrix::CreateRotationMatrixZ(double angleInRadians) {
    CMatrix mat;

    // 填充绕Z轴的旋转矩阵（列优先）
    mat.m00 = cos(angleInRadians);  mat.m10 = sin(angleInRadians); mat.m20 = 0; mat.m30 = 0;
    mat.m01 = -sin(angleInRadians); mat.m11 = cos(angleInRadians); mat.m21 = 0; mat.m31 = 0;
    mat.m02 = 0;                    mat.m12 = 0;                    mat.m22 = 1; mat.m32 = 0;
    mat.m03 = 0;                    mat.m13 = 0;                    mat.m23 = 0; mat.m33 = 1;

    return mat;
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
    // 计算 pitch (俯仰角)
    euler.p = asin(-m12);  // atan2 returns the angle in radians

    // 计算 yaw (偏航角)
    //float sin_pitch = -m20;
    //if (sin_pitch < -1.0f) sin_pitch = -1.0f;  // 防止出界
    //if (sin_pitch > 1.0f) sin_pitch = 1.0f;
    //euler.h = asin(sin_pitch);  // asin returns the angle in radians
    euler.h = atan2(m02,m22);
    //// 计算 roll (滚转角)
    //float cos_pitch = cos(euler.p);
    //if (cos_pitch > 1e-6) {  // 防止除以 0
    //    euler.b = atan2(m21, m22);  // atan2 returns the angle in radians
    //}
    //else {  // 如果 cos_pitch 接近 0，使用更稳定的计算方式
    //    euler.b = atan2(-m12, m11);
    //}
    euler.b = atan2(m10, m11);
    euler.h = euler.h * 180 / M_PI;
    euler.p = euler.p * 180 / M_PI;
    euler.b = euler.b * 180 / M_PI;

    return euler;
}

std::string CMatrix::ToString()
{
    return "\n"+std::to_string(m00) + " " + std::to_string(m01) + " " + std::to_string(m02) + " " + std::to_string(m03) + "\n" +
        std::to_string(m10) + " " + std::to_string(m11) + " " + std::to_string(m12) + " " + std::to_string(m13) + "\n" +
        std::to_string(m20) + " " + std::to_string(m21) + " " + std::to_string(m22) + " " + std::to_string(m23) + "\n" +
        std::to_string(m30) + " " + std::to_string(m31) + " " + std::to_string(m32) + " " + std::to_string(m33) + "\n";
}

#include "stdafx.h"
#include "CEuler.h"

void CEuler::Set(float fh, float fp, float fb)
{
    this->h = fh;
    this->p = fp;
    this->b = fb;
}

CEuler::CEuler()
{
    this->h = 0;
    this->p = 0;
    this->b = 0;
}

CEuler::CEuler(float fh, float fp, float fb)
{
    this->h = fh;
    this->p = fp;
    this->b = fb;
}

// 将欧拉角转换为 CVector 向量
CVector CEuler::ToCVector()
{
    return CVector(h, p, b);  // 将欧拉角直接转换为向量
}

// 将欧拉角转换为 CMatrix 矩阵
CMatrix CEuler::ToCMatrix()const
{
    float rad_h = h * (M_PI / 180.0f);  // Yaw
    float rad_p = p * (M_PI / 180.0f);  // Pitch
    float rad_b = b * (M_PI / 180.0f);  // Roll
    CMatrix mat = CMatrix::Identity();
    CMatrix rotationY = CMatrix::CreateRotationMatrixY(rad_h);
    CMatrix rotationX = CMatrix::CreateRotationMatrixX(rad_p);
    CMatrix rotationZ = CMatrix::CreateRotationMatrixZ(rad_b);
    mat = rotationY*rotationX*rotationZ;
    return mat;
}

std::string CEuler::ToString()
{
    return std::to_string(h) + " " + std::to_string(p) + " " + std::to_string(b);
}

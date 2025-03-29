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
    CMatrix mat = CMatrix::Identity();
    CMatrix rotationY = CMatrix::CreateRotationMatrix(h,CVector::Up());
    CMatrix rotationX = CMatrix::CreateRotationMatrix(p, CVector::Right());
    CMatrix rotationZ = CMatrix::CreateRotationMatrix(b, CVector::Forward());
    mat = rotationY*rotationX*rotationZ;
    return mat;
}

std::string CEuler::ToString()
{
    return std::to_string(h) + " " + std::to_string(p) + " " + std::to_string(b);
}

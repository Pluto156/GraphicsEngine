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
CVector3 CEuler::ToCVector()
{
    return CVector3(h, p, b);  // 将欧拉角直接转换为向量
}

// 将欧拉角转换为 CMatrix 矩阵
CMatrix4 CEuler::ToCMatrix()const
{
    CMatrix4 mat = CMatrix4::Identity();
    CMatrix4 rotationY = CMatrix4::CreateRotationMatrix(h,CVector3::Up());
    CMatrix4 rotationX = CMatrix4::CreateRotationMatrix(p, CVector3::Right());
    CMatrix4 rotationZ = CMatrix4::CreateRotationMatrix(b, CVector3::Forward());
    mat = rotationY*rotationX*rotationZ;
    return mat;
}

std::string CEuler::ToString()
{
    return std::to_string(h) + " " + std::to_string(p) + " " + std::to_string(b);
}

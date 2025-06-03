#pragma once
#include "CEuler.h"
#include "CMatrix4.h"

class CVector4
{
public:
    float x, y, z, w;

    CVector4();
    CVector4(float x, float y, float z, float w);
    CVector4(const CVector3& v3);
    CVector4(const CVector3& v3, float w);
    ~CVector4();

    CVector4 operator-(const CVector4& v) const;
    CVector4 operator-=(const CVector4& v);
    CVector4 operator+(const CVector4& v) const;
    CVector4 operator+=(const CVector4& v);
    CVector4& operator=(const CVector4& v);
    CVector4 operator*(float scalar) const;
    CVector4 operator*=(float scalar);
    CVector4 operator/(float scalar) const;
    CVector4 operator/=(float scalar);
    float& operator[] (int i);
    const float& operator[] (int i) const;

    friend CVector4 operator*(float scalar, const CVector4& v);

    void Normalize();
    CVector4 GetNormalize() const;

    float len() const;
    float lenSquared() const;

    void Absolutize();
    operator float* () { return &x; }

    std::string ToString() const;

    CEuler ToEuler() const;
    CMatrix4 ToCMatrix() const;

    void Clear()
    {
        x = y = z = w = 0;
    }

    const static CVector4& Up()
    {
        const static CVector4 Up(0, 1, 0, 0);
        return Up;
    }

    const static CVector4& Right()
    {
        const static CVector4 Right(1, 0, 0, 0);
        return Right;
    }

    const static CVector4& Forward()
    {
        const static CVector4 Forward(0, 0, 1, 0);
        return Forward;
    }
};

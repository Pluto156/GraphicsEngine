#pragma once
#include <iostream>
#include <string>

class CEuler;
class CMatrix4;
class CVector4;


class CVector3
{
public:
    float x, y, z;

    CVector3();
    CVector3(float x, float y, float z);
    CVector3(const CVector4& v);
    ~CVector3();


    CVector3 operator-(const CVector3& v) const;
    CVector3 operator-=(const CVector3& v) ;
    CVector3 operator+(const CVector3& v) const;
    CVector3 operator+=(const CVector3& v) ;
    CVector3& operator=(const CVector3& v);
    CVector3 operator*(float scalar) const;
    CVector3 operator*=(float scalar);
    CVector3 operator/(float scalar) const;
    CVector3 operator/=(float scalar);
    CVector3 operator* (const CVector3& v) const;
    CVector3 operator/ (const CVector3& v) const;

    float& operator[] (int i);
    const float& operator[] (int i) const;


    friend CVector3 operator*(float scalar, const CVector3& v); 

    void Normalize();
    CVector3 GetNormalize()const;

    float len() const;
    float lenSquared() const;

    CVector3 project(const CVector3& v) const;
    float projectLen(const CVector3& v) const;


    float distanceTo(const CVector3& v) const;  
    CVector3 operator-() const;
    void Absolutize();
    operator float* () { return &x; }

    std::string ToString()const;

    CEuler ToEuler() const;
    CMatrix4 ToCMatrix() const;
    CVector4 ToPosVec4() const;
    CVector4 ToDirVec4() const;

    void Clear()
    {
        x = y = z = 0;
    }
    const static CVector3& Up()
    {
        const static CVector3 Up(0, 1, 0);
        return Up;
    }

    const static CVector3& Right()
    {
        const static CVector3 Right(1, 0, 0);
        return Right;
    }

    const static CVector3& Forward()
    {
        const static CVector3 Forward(0, 0, 1);
        return Forward;
    }
};

bool Calculate();

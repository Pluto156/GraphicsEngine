#pragma once
#include <iostream>
#include <string>

class CEuler;
class CMatrix;

class CVector
{
public:
    float x, y, z;

    // 默认构造函数，初始化向量为(0, 0, 0)
    CVector();
    CVector(float x, float y, float z);
    // 析构函数
    ~CVector();

    // 向量减法
    CVector operator-(const CVector& v) const;
    CVector operator-=(const CVector& v) ;


    // 向量加法
    CVector operator+(const CVector& v) const;
    CVector operator+=(const CVector& v) ;


    // 赋值操作符重载
    CVector& operator=(const CVector& v);

    // 向量数乘操作
    CVector operator*(float scalar) const;
    CVector operator*=(float scalar);
    CVector operator/(float scalar) const;
    CVector operator/=(float scalar);

    friend CVector operator*(float scalar, const CVector& v); // 支持100 * CVector

    // 向量点积
    float dotMul(const CVector& v) const;

    // 向量叉积
    CVector crossMul(const CVector& v) const;

    // 向量单位化
    void Normalize();

    // 向量求模
    float len() const;

    // 向量投影
    CVector project(const CVector& v) const;

    // 向量距离
    float distanceTo(const CVector& v) const;  // 新增的距离函数

    // 向量取反（通过重载 - 运算符）
    CVector operator-() const;

    // 转换为float*类型，用于GL传递数据
    operator float* () { return &x; }

    std::string ToString();

    CEuler ToEuler() const;
    CMatrix ToCMatrix() const;

    void Clear()
    {
        x = y = z = 0;
    }
    const static CVector& Up()
    {
        const static CVector Up(0, 1, 0);
        return Up;
    }

    const static CVector& Right()
    {
        const static CVector Right(1, 0, 0);
        return Right;
    }

    const static CVector& Forward()
    {
        const static CVector Forward(0, 0, 1);
        return Forward;
    }
};

// 计算两向量间的距离
bool Calculate();

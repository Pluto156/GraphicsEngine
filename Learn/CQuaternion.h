#pragma once
#include "CVector3.h"
class CQuaternion
{
public:
    float w, x, y, z;

    // 构造函数，初始化四元数
    CQuaternion() : w(1), x(0), y(0), z(0) {}
    CQuaternion(float fw, float fx, float fy, float fz) : w(fw), x(fx), y(fy), z(fz) {}

    // 设置四元数的值
    void Set(float fw, float fx, float fy, float fz);

    // 四元数转化为 float 数组，支持使用四元数数据
    operator float* () { return &w; }

    // 设置四元数值为旋转角度和旋转轴
    void SetAngle(float angle, CVector3 axis);

    // 赋值操作符重载
    CQuaternion& operator=(const CQuaternion& p);

    // 加法操作符重载
    CQuaternion operator+(const CQuaternion& p)const;
    CQuaternion operator+=(const CQuaternion& p);
    CQuaternion operator-(const CQuaternion& p) const;
    CQuaternion operator-=(const CQuaternion& p);

    void AddScaledVector(const CVector3& vector, float scale);

    // 数量乘操作符重载
    CQuaternion operator*(float data);

    // 四元数乘法操作符重载
    CQuaternion operator*(const CQuaternion& p);
    CQuaternion operator*=(const CQuaternion& p);


    // 点乘操作
    float dotMul(const CQuaternion& p);

    // 求模
    float len();

    // 求标准化
    BOOL Normalize();

    // 求逆四元数，会改变当前四元数
    CQuaternion& Inverse();

    // 获取逆四元数，不改变当前四元数
    CQuaternion GetInverse() const;

    // 求差：当前为a，求c=a-b
    CQuaternion Div(const CQuaternion& b);

    // 获取旋转角度和旋转轴
    void GetAngle(float& angle, CVector3& axis);

    /// <summary>
/// 绕axis轴旋转angle度
/// </summary>
/// <param name="axis">旋转轴(必须是单位向量)</param>
/// <param name="angle">旋转角度(弧度制)</param>
    void Rotate(const CVector3& axis, float angle);

    // 四元数的Slerp插值
    CQuaternion Slerp(const CQuaternion& Vend, float t);

    // 四元数插值，生成n个数据
    void Slerp(const CQuaternion& Vend, int n, float* t, CQuaternion* Result);

    CMatrix4 CQuaternion::ToCMatrix4();
};


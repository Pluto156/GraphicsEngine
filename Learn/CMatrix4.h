#pragma once
class CEuler;
class CQuaternion;
class CMatrix4
{
public:
    // 矩阵元素按列优先排列
    float m00, m10, m20, m30;
    float m01, m11, m21, m31;
    float m02, m12, m22, m32;
    float m03, m13, m23, m33;

    CMatrix4();
    CMatrix4(float* val);
    CMatrix4(float n);
    CMatrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);
    CMatrix4(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);

    void Set(float* val);
    operator const float* () const { return &m00; }
    CMatrix4 CMatrix4::operator-() const
    {
        float t[16] = { -m00, -m01, -m02, -m03,
            -m10, -m11, -m12, -m13,
            -m20, -m21, -m22, -m23,
            -m30, -m31, -m32, -m33 };
        return CMatrix4(t);
    }

    CMatrix4& operator=(const CMatrix4& p);
    CMatrix4 operator*(float d);
    CMatrix4 operator*(const CMatrix4& p)const;
    CMatrix4 operator*=(const CMatrix4& p);
    CMatrix4 operator+(const CMatrix4& v) const;
    CMatrix4 operator+=(const CMatrix4& v);
    CMatrix4 operator-(const CMatrix4& v) const;
    CMatrix4 operator-=(const CMatrix4& v);
    CVector4 operator* (const CVector4& v) const;

    CVector4 GetRow(uint32_t index) const;
    CVector4 GetColumn(uint32_t index) const;
    CVector3 vecMulVector3(const CVector3& p);
    CVector3 posMulVector3(const CVector3& p);
    void SetRotate(float angle, const CVector3& axis);
    void SetTrans(const CVector3& trans);
    void SetScale(const CVector3& scale);
    float Inverse();  // 求矩阵的逆，成功返回行列式的值，否则返回0
    CMatrix4 GetInverse()const;  // 返回逆矩阵

    void Transpose();
    CMatrix4 GetTranspose()const;  // 返回转置矩阵


    // 添加单位矩阵和零矩阵
    static const CMatrix4& Identity()
    {
        static CMatrix4 Identity;
        return Identity;
    }
    static const CMatrix4& Zero()
    {
        static CMatrix4 Zero;
        Zero.m00 = 0;Zero.m11 = 0;Zero.m22 = 0;Zero.m33 = 0;
        return Zero;
    }
    static CMatrix4 CreateRotationMatrix(float angle, const CVector3& axis);
    static CMatrix4 CreateRotationMatrix(const CVector3& lookDir);

    CVector3 GetForward();
    CVector3 GetUp();
    CVector3 GetRight();
    CEuler ToEuler();
    CQuaternion ToQuaternion()const;
    void Translate(const CVector3& v);
    std::string ToString();

private:
    float Determinant()const;  // 计算矩阵行列式
    CMatrix4 Adjoint()const;  // 计算伴随矩阵
};

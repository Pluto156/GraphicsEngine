#pragma once
class CEuler;
class CQuaternion;
class CMatrix
{
public:
    // 矩阵元素按列优先排列
    float m00, m10, m20, m30;
    float m01, m11, m21, m31;
    float m02, m12, m22, m32;
    float m03, m13, m23, m33;

    CMatrix();
    CMatrix(float* val);

    void Set(float* val);
    operator const float* () const { return &m00; }
    CMatrix CMatrix::operator-() const
    {
        float t[16] = { -m00, -m01, -m02, -m03,
            -m10, -m11, -m12, -m13,
            -m20, -m21, -m22, -m23,
            -m30, -m31, -m32, -m33 };
        return CMatrix(t);
    }

    CMatrix& operator=(const CMatrix& p);
    CMatrix operator*(float d);
    CMatrix operator*(const CMatrix& p);
    CVector vecMul(const CVector& p);
    CVector posMul(const CVector& p);
    void SetRotate(float angle, const CVector& axis);
    void SetTrans(const CVector& trans);
    void SetScale(const CVector& scale);
    float Inverse();  // 求矩阵的逆，成功返回行列式的值，否则返回0
    CMatrix GetInverse();  // 返回逆矩阵

    void Transpose();
    CMatrix GetTranspose();  // 返回转置矩阵


    // 添加单位矩阵和零矩阵
    static const CMatrix& Identity()
    {
        static CMatrix Identity;
        return Identity;
    }
    static const CMatrix& Zero()
    {
        static CMatrix Zero;
        Zero.m00 = 0;Zero.m11 = 0;Zero.m22 = 0;Zero.m33 = 0;
        return Zero;
    }
    static CMatrix CreateRotationMatrix(float angle, const CVector& axis);
    static CMatrix CreateRotationMatrix(const CVector& lookDir);

    CVector GetForward();
    CVector GetUp();
    CVector GetRight();
    CEuler ToEuler();
    CQuaternion ToQuaternion();
    std::string ToString();

private:
    float Determinant();  // 计算矩阵行列式
    CMatrix Adjoint();  // 计算伴随矩阵
};

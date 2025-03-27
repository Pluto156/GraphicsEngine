#pragma once
class CEuler;
class CMatrix
{
public:
    // ����Ԫ�ذ�����������
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    CMatrix();
    CMatrix(float* val);

    void Set(float* val);
    operator const float* () const { return &m00; }
    CMatrix& operator=(const CMatrix& p);
    CMatrix operator*(float d);
    CMatrix operator*(const CMatrix& p);
    CVector vecMul(const CVector& p);
    CVector posMul(const CVector& p);
    //void SetRotate(float angle, const CVector& axis);
    //void SetTrans(const CVector& trans);
    //void SetScale(const CVector& scale);
    //float Inverse();  // �������棬�ɹ���������ʽ��ֵ�����򷵻�0
    CMatrix GetInverse();  // ���������

    // ��ӵ�λ����������
    static CMatrix Identity();
    static CMatrix Zero();
    static CMatrix CreateRotationMatrixX(double angleInRadians);
    static CMatrix CreateRotationMatrixY(double angleInRadians);
    static CMatrix CreateRotationMatrixZ(double angleInRadians);

    CVector GetForward();
    CVector GetUp();
    CVector GetRight();
    CEuler ToEuler();

    std::string ToString();

private:
    float Determinant();  // �����������ʽ
    CMatrix Adjoint();  // ����������
};

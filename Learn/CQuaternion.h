#pragma once
#include "CVector.h"
class CQuaternion
{
public:
    float w, x, y, z;

    // ���캯������ʼ����Ԫ��
    CQuaternion() : w(1), x(0), y(0), z(0) {}

    // ������Ԫ����ֵ
    void Set(float fw, float fx, float fy, float fz);

    // ��Ԫ��ת��Ϊ float ���飬֧��ʹ����Ԫ������
    operator float* () { return &w; }

    // ������Ԫ��ֵΪ��ת�ǶȺ���ת��
    void SetAngle(float angle, CVector axis);

    // ��ֵ����������
    CQuaternion& operator=(const CQuaternion& p);

    // �ӷ�����������
    CQuaternion operator+(const CQuaternion& p);

    // ��Ԫ����������������
    CQuaternion operator-(const CQuaternion& p) const;


    // �����˲���������
    CQuaternion operator*(float data);

    // ��Ԫ���˷�����������
    CQuaternion operator*(const CQuaternion& p);

    // ��˲���
    float dotMul(const CQuaternion& p);

    // ��ģ
    float len();

    // ���׼��
    BOOL Normalize();

    // ������Ԫ������ı䵱ǰ��Ԫ��
    CQuaternion& Inverse();

    // ��ȡ����Ԫ�������ı䵱ǰ��Ԫ��
    CQuaternion GetInverse() const;

    // ����ǰΪa����c=a-b
    CQuaternion Div(const CQuaternion& b);

    // ��ȡ��ת�ǶȺ���ת��
    void GetAngle(float& angle, CVector& axis);

    // ��Ԫ����Slerp��ֵ
    CQuaternion Slerp(const CQuaternion& Vend, float t);

    // ��Ԫ����ֵ������n������
    void Slerp(const CQuaternion& Vend, int n, float* t, CQuaternion* Result);
};


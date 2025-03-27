#pragma once
#include <iostream>
#include <string>

class CEuler;
class CVector
{
public:
    float x, y, z;

    // Ĭ�Ϲ��캯������ʼ������Ϊ(0, 0, 0)
    CVector();
    CVector(float x, float y, float z);
    // ��������
    ~CVector();

    // ��������
    CVector operator-(const CVector& v) const;

    // �����ӷ�
    CVector operator+(const CVector& v) const;

    // ��ֵ����������
    CVector& operator=(const CVector& v);

    // �������˲���
    CVector operator*(float scalar) const;
    friend CVector operator*(float scalar, const CVector& v); // ֧��100 * CVector

    // �������
    float dotMul(const CVector& v) const;

    // �������
    CVector crossMul(const CVector& v) const;

    // ������λ��
    void Normalize();

    // ������ģ
    float len() const;

    // ����ͶӰ
    CVector project(const CVector& v) const;

    // ��������
    float distanceTo(const CVector& v) const;  // �����ľ��뺯��

    // ת��Ϊfloat*���ͣ�����GL��������
    operator float* () { return &x; }

    std::string ToString();

    CEuler ToEuler();
};

// ������������ľ���
bool Calculate();

#pragma once
class CEuler
{
public:
    float h, p, b; // ŷ���ǣ�������ƫ������ת

    // ����ŷ����
    void Set(float fh, float fp, float fb);

    // ת��Ϊ CVector ����
    CVector ToCVector();

    // ת��Ϊ CMatrix ����
    CMatrix ToCMatrix()const;

    operator float* () { return &h; }
    // ���ؼӷ������
    CEuler operator+(const CEuler& other) const {
        return CEuler(h + other.h, p + other.p, b + other.b);
    }
    // ���ؼ��������
    CEuler operator-(const CEuler& other) const {
        return CEuler(h - other.h, p - other.p, b - other.b);
    }

    CEuler();
    CEuler(float fh, float fp, float fb);
    std::string ToString();
};

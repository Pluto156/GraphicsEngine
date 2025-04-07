#pragma once
class CEuler
{
public:
    float h, p, b; // 欧拉角：俯仰、偏航、滚转

    // 设置欧拉角
    void Set(float fh, float fp, float fb);

    // 转换为 CVector 向量
    CVector ToCVector();

    // 转换为 CMatrix 矩阵
    CMatrix ToCMatrix()const;

    operator float* () { return &h; }
    // 重载加法运算符
    CEuler operator+(const CEuler& other) const {
        return CEuler(h + other.h, p + other.p, b + other.b);
    }
    // 重载减法运算符
    CEuler operator-(const CEuler& other) const {
        return CEuler(h - other.h, p - other.p, b - other.b);
    }
    CEuler operator-() const
    {
        return CEuler(-h, -p, -b);
    }
    CEuler();
    CEuler(float fh, float fp, float fb);
    std::string ToString();
};

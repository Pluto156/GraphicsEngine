#include "stdafx.h"
#include "CVector3.h"
#include "CVector4.h"

// 默认构造函数，初始化向量为(0, 0, 0)
CVector3::CVector3() : x(0), y(0), z(0) {}

CVector3::CVector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

CVector3::CVector3(const CVector4& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

// 析构函数
CVector3::~CVector3() {}

// 向量减法
CVector3 CVector3::operator-(const CVector3& v) const {
    return CVector3(x - v.x, y - v.y, z - v.z);
}

CVector3 CVector3::operator-=(const CVector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}


// 向量加法
CVector3 CVector3::operator+(const CVector3& v) const {
    return CVector3(x + v.x, y + v.y, z + v.z);
}

CVector3 CVector3::operator+=(const CVector3& v)  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

// 赋值操作符重载
CVector3& CVector3::operator=(const CVector3& v) {
    if (this != &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}

// 向量数乘操作
CVector3 CVector3::operator*(float scalar) const {
    return CVector3(x * scalar, y * scalar, z * scalar);
}

CVector3 CVector3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

CVector3 CVector3::operator/(float scalar) const
{
    return CVector3(x / scalar, y / scalar, z / scalar);
}


CVector3 CVector3::operator* (const CVector3& v) const
{
    return CVector3(x * v.x, y * v.y, z * v.z);
}

CVector3 CVector3::operator/ (const CVector3& v) const
{
    return CVector3(x / v.x, y / v.y, z / v.z);
}

CVector3 CVector3::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}
float& CVector3::operator[] (int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        //Debug::LogError("Invalid index to Vector3 !");
        //assert(false);
        return x;
        break;
    }
}

const float& CVector3::operator[] (int i) const
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        //Debug::LogError("Invalid index to Vector3 !");
        //assert(false);
        return x;
        break;
    }
}

// 友元函数：支持 100 * CVector 的形式
CVector3 operator*(float scalar, const CVector3& v) {
    return CVector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

// 向量单位化
void CVector3::Normalize() {
    float length = len();
    if (length > 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}
CVector3 CVector3::GetNormalize()const
{
    CVector3 t = *this;
    t.Normalize();
    return t;
}



// 向量求模
float CVector3::len() const {
    return sqrt(x * x + y * y + z * z);
}

float CVector3::lenSquared() const
{
    return x * x + y * y + z * z;
}

// 向量投影
CVector3 CVector3::project(const CVector3& v) const {
    float dotProduct = Math::Dot(*this,v);
    float lengthSquared = v.len() * v.len();
    return v * (dotProduct / lengthSquared);
}

float CVector3::projectLen(const CVector3& v) const
{
    return project(v).len();
}

// 向量距离
float CVector3::distanceTo(const CVector3& v) const {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

// 向量取反（通过重载 - 运算符）
CVector3 CVector3::operator-() const {
    return CVector3(-x, -y, -z);
}

void CVector3::Absolutize()
{
    x = fabsf(x);
    y = fabsf(y);
    z = fabsf(z);
}
std::string CVector3::ToString()const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

CEuler CVector3::ToEuler() const
{
    CEuler euler;

    // 归一化方向向量，避免影响角度计算
    CVector3 dir = this->GetNormalize();

    // 偏航角 (Yaw) - 绕 Y 轴
    euler.h = atan2(dir.x, dir.z);

    // 俯仰角 (Pitch) - 绕 X 轴
    euler.p = -atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z));

    // 翻滚角 (Roll) - 无法从方向向量唯一确定，设为 0
    euler.b = 0;

    // 转换为角度
    constexpr float RAD2DEG = 180.0f / M_PI;
    euler.h *= RAD2DEG;
    euler.p *= RAD2DEG;
    euler.b *= RAD2DEG;

    return euler;
}

CMatrix4 CVector3::ToCMatrix()const
{
    return ToEuler().ToCMatrix();
}

CVector4 CVector3::ToPosVec4() const
{
    return CVector4(x, y, z, 1.0f);
}

CVector4 CVector3::ToDirVec4() const
{
    return CVector4(x, y, z, 0.0f);
}

// Calculate 函数实现
#include <cmath>
#include <iostream>

bool Calculate() {
    FILE* fp, * fpOut;
    char str[1024];
    fopen_s(&fp, "test.txt", "rt");
    fopen_s(&fpOut, "out.txt", "wt");

    while (!feof(fp)) {
        fscanf_s(fp, "%s\n", str, 1024);

        if (strcmp(str, "向量相加") == 0) {
            CVector3 v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = v1 + v2;
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "向量点乘") == 0) {
            CVector3 v1, v2;
            float vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = Math::Dot(v1,v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout);
        }
        else if (strcmp(str, "向量叉乘") == 0) {
            CVector3 v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = Math::Cross(v1,v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "向量标准化") == 0) {
            CVector3 v1, v2;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            v2 = v1;
            v2.Normalize();
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
        }
        else if (strcmp(str, "向量求模") == 0) {
            CVector3 v1;
            float vout;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            vout = v1.len();
            fprintf(fpOut, "%s\n%g,%g,%g %g\n", str, v1.x, v1.y, v1.z, vout);
        }
        else if (strcmp(str, "向量投影") == 0) {
            CVector3 v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = v1.project(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "矩阵相乘") == 0) {
            CMatrix4 a, b, c;
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]
            );
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &b[0], &b[1], &b[2], &b[3], &b[4], &b[5], &b[6], &b[7], &b[8], &b[9], &b[10], &b[11], &b[12], &b[13], &b[14], &b[15]
            );
            c = a * b;
            fprintf(fpOut, "%s\n%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g  \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n", str
                , a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]
                , b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15]
                , c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );

        }
        else if (strcmp(str, "矩阵乘向量") == 0) {
            CMatrix4 a;
            CVector3 b,c;
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]
            );
            fscanf_s(fp, "%f,%f,%f", &b.x, &b.y, &b.z);
            c = a.vecMulVector3(b);
            fprintf(fpOut, "%s\n%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str
                , a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]
                , b.x, b.y, b.z
                , c.x, c.y, c.z);
        }
        else if (strcmp(str, "矩阵乘位置") == 0) {
            CMatrix4 a;
            CVector3 b, c;
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]
            );
            fscanf_s(fp, "%f,%f,%f", &b.x, &b.y, &b.z);
            c = a.posMulVector3(b);
            fprintf(fpOut, "%s\n%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str
                , a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]
                , b.x, b.y, b.z
                , c.x, c.y, c.z);
        }
        else if (strcmp(str, "矩阵设置旋转") == 0) {
            float angle;
            CVector3 v1;
            CMatrix4 c;
            fscanf_s(fp, "%f", &angle);
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            c.SetRotate(angle, v1);
            fprintf(fpOut, "%s\n%g\t%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n ", str, angle, v1.x, v1.y, v1.z, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );

        }
        else if (strcmp(str, "矩阵设置平移") == 0) {
            CVector3 v1;
            CMatrix4 c;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            c.SetTrans(v1);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n ", str, v1.x, v1.y, v1.z, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );
        }
        else if (strcmp(str, "矩阵设置缩放") == 0) {
            CVector3 v1;
            CMatrix4 c;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            c.SetScale(v1);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n ", str, v1.x, v1.y, v1.z, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );
        }
        else if (strcmp(str, "矩阵求逆") == 0) {
            CMatrix4 a, b, c;
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]
            );
            c = a .GetInverse();
            fprintf(fpOut, "%s\n%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n", str
                , a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]
                , c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );
        }
        else {
            fgets(str, 1024, fp);
        }
    }

    fclose(fp);
    fclose(fpOut);
    return true;
}


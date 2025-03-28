#include "stdafx.h"
#include "CVector.h"

// 默认构造函数，初始化向量为(0, 0, 0)
CVector::CVector() : x(0), y(0), z(0) {}

CVector::CVector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// 析构函数
CVector::~CVector() {}

// 向量减法
CVector CVector::operator-(const CVector& v) const {
    return CVector(x - v.x, y - v.y, z - v.z);
}

// 向量加法
CVector CVector::operator+(const CVector& v) const {
    return CVector(x + v.x, y + v.y, z + v.z);
}

// 赋值操作符重载
CVector& CVector::operator=(const CVector& v) {
    if (this != &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}

// 向量数乘操作
CVector CVector::operator*(float scalar) const {
    return CVector(x * scalar, y * scalar, z * scalar);
}

// 友元函数：支持 100 * CVector 的形式
CVector operator*(float scalar, const CVector& v) {
    return CVector(v.x * scalar, v.y * scalar, v.z * scalar);
}

// 向量点积
float CVector::dotMul(const CVector& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// 向量叉积
CVector CVector::crossMul(const CVector& v) const {
    return CVector(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

// 向量单位化
void CVector::Normalize() {
    float length = len();
    if (length > 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

// 向量求模
float CVector::len() const {
    return sqrt(x * x + y * y + z * z);
}

// 向量投影
CVector CVector::project(const CVector& v) const {
    float dotProduct = dotMul(v);
    float lengthSquared = v.len() * v.len();
    return v * (dotProduct / lengthSquared);
}

// 向量距离
float CVector::distanceTo(const CVector& v) const {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

std::string CVector::ToString()
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

CEuler CVector::ToEuler()
{
    CEuler euler;

    // 计算 pitch
    euler.h = atan2(x, z);

    // 计算 roll (绕 X 轴的旋转角)
    euler.p = -atan2(y, sqrt(x * x + z * z));

    // 假设 yaw (绕 Z 轴的旋转角) 为 0
    euler.b = 0;
    euler.h = euler.h * 180 / M_PI;
    euler.p = euler.p * 180 / M_PI;
    euler.b = euler.b * 180 / M_PI;
    return euler;
}

// Calculate 函数实现
bool Calculate() {
    FILE* fp, * fpOut;
    char str[1024];
    fopen_s(&fp, "test.txt", "rt");
    fopen_s(&fpOut, "out.txt", "wt");

    while (!feof(fp)) {
        fscanf_s(fp, "%s\n", str, 1024);

        if (strcmp(str, "向量相加") == 0) {
            CVector v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            // 计算向量相加
            vout = v1 + v2;
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "向量点乘") == 0)
        {
            CVector v1, v2;
            float vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            // 计算向量点积
            vout = v1.dotMul(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout);
        }
        else if (strcmp(str, "向量叉乘") == 0)
        {
            CVector v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            // 计算向量叉积
            vout = v1.crossMul(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "向量标准化") == 0)
        {
            CVector v1, v2;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            v2 = v1;
            // 计算向量标准化
            v2.Normalize();
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
        }
        else if (strcmp(str, "向量求模") == 0)
        {
            CVector v1;
            float vout;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            vout = v1.len();
            fprintf(fpOut, "%s\n%g,%g,%g %g\n", str, v1.x, v1.y, v1.z, vout);
        }
        else if (strcmp(str, "向量投影") == 0)
        {
            CVector v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            // 计算向量投影
            vout = v1.project(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else {
            fgets(str, 1024, fp);
        }
    }

    fclose(fp);
    fclose(fpOut);
    return true;
}

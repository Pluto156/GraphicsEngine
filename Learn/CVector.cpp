#include "stdafx.h"
#include "CVector.h"

// Ĭ�Ϲ��캯������ʼ������Ϊ(0, 0, 0)
CVector::CVector() : x(0), y(0), z(0) {}

CVector::CVector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// ��������
CVector::~CVector() {}

// ��������
CVector CVector::operator-(const CVector& v) const {
    return CVector(x - v.x, y - v.y, z - v.z);
}

// �����ӷ�
CVector CVector::operator+(const CVector& v) const {
    return CVector(x + v.x, y + v.y, z + v.z);
}

// ��ֵ����������
CVector& CVector::operator=(const CVector& v) {
    if (this != &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}

// �������˲���
CVector CVector::operator*(float scalar) const {
    return CVector(x * scalar, y * scalar, z * scalar);
}

// ��Ԫ������֧�� 100 * CVector ����ʽ
CVector operator*(float scalar, const CVector& v) {
    return CVector(v.x * scalar, v.y * scalar, v.z * scalar);
}

// �������
float CVector::dotMul(const CVector& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// �������
CVector CVector::crossMul(const CVector& v) const {
    return CVector(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

// ������λ��
void CVector::Normalize() {
    float length = len();
    if (length > 0) {
        x /= length;
        y /= length;
        z /= length;
    }
}

// ������ģ
float CVector::len() const {
    return sqrt(x * x + y * y + z * z);
}

// ����ͶӰ
CVector CVector::project(const CVector& v) const {
    float dotProduct = dotMul(v);
    float lengthSquared = v.len() * v.len();
    return v * (dotProduct / lengthSquared);
}

// ��������
float CVector::distanceTo(const CVector& v) const {
    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

// ����ȡ����ͨ������ - �������
CVector CVector::operator-() const {
    return CVector(-x, -y, -z);
}

std::string CVector::ToString()
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

CEuler CVector::ToEuler() const
{
    CEuler euler;

    // ���� pitch
    euler.h = atan2(x, z);

    // ���� roll (�� X �����ת��)
    euler.p = -atan2(y, sqrt(x * x + z * z));

    // ���� yaw (�� Z �����ת��) Ϊ 0
    euler.b = 0;
    euler.h = euler.h * 180 / M_PI;
    euler.p = euler.p * 180 / M_PI;
    euler.b = euler.b * 180 / M_PI;
    return euler;
}

CMatrix CVector::ToCMatrix()const
{
    return ToEuler().ToCMatrix();
}

// Calculate ����ʵ��
#include <cmath>
#include <iostream>

bool Calculate() {
    FILE* fp, * fpOut;
    char str[1024];
    fopen_s(&fp, "test.txt", "rt");
    fopen_s(&fpOut, "out.txt", "wt");

    while (!feof(fp)) {
        fscanf_s(fp, "%s\n", str, 1024);

        if (strcmp(str, "�������") == 0) {
            CVector v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = v1 + v2;
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "�������") == 0) {
            CVector v1, v2;
            float vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = v1.dotMul(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout);
        }
        else if (strcmp(str, "�������") == 0) {
            CVector v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = v1.crossMul(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "������׼��") == 0) {
            CVector v1, v2;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            v2 = v1;
            v2.Normalize();
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
        }
        else if (strcmp(str, "������ģ") == 0) {
            CVector v1;
            float vout;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            vout = v1.len();
            fprintf(fpOut, "%s\n%g,%g,%g %g\n", str, v1.x, v1.y, v1.z, vout);
        }
        else if (strcmp(str, "����ͶӰ") == 0) {
            CVector v1, v2, vout;
            fscanf_s(fp, "%f,%f,%f %f,%f,%f", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z);
            vout = v1.project(v2);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vout.x, vout.y, vout.z);
        }
        else if (strcmp(str, "�������") == 0) {
            CMatrix a, b, c;
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
        else if (strcmp(str, "���������") == 0) {
            CMatrix a;
            CVector b,c;
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]
            );
            fscanf_s(fp, "%f,%f,%f", &b.x, &b.y, &b.z);
            c = a.vecMul(b);
            fprintf(fpOut, "%s\n%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str
                , a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]
                , b.x, b.y, b.z
                , c.x, c.y, c.z);
        }
        else if (strcmp(str, "�����λ��") == 0) {
            CMatrix a;
            CVector b, c;
            fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]
            );
            fscanf_s(fp, "%f,%f,%f", &b.x, &b.y, &b.z);
            c = a.posMul(b);
            fprintf(fpOut, "%s\n%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g \t%g,%g,%g \t%g,%g,%g\n", str
                , a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]
                , b.x, b.y, b.z
                , c.x, c.y, c.z);
        }
        else if (strcmp(str, "����������ת") == 0) {
            float angle;
            CVector v1;
            CMatrix c;
            fscanf_s(fp, "%f", &angle);
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            c.SetRotate(angle, v1);
            fprintf(fpOut, "%s\n%g\t%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n ", str, angle, v1.x, v1.y, v1.z, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );

        }
        else if (strcmp(str, "��������ƽ��") == 0) {
            CVector v1;
            CMatrix c;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            c.SetTrans(v1);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n ", str, v1.x, v1.y, v1.z, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );
        }
        else if (strcmp(str, "������������") == 0) {
            CVector v1;
            CMatrix c;
            fscanf_s(fp, "%f,%f,%f", &v1.x, &v1.y, &v1.z);
            c.SetScale(v1);
            fprintf(fpOut, "%s\n%g,%g,%g \t%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n ", str, v1.x, v1.y, v1.z, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
            );
        }
        else if (strcmp(str, "��������") == 0) {
            CMatrix a, b, c;
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


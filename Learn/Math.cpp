#include "stdafx.h"
#include "Math.h"
float Math::PI = 3.141592653f;
float Math::PIx2 = 6.283185306f;
float Math::SQRT2 = 1.414213562f;
float Math::INV_SQRT2 = 0.707106781f;
bool Math::Approximately(float a, float b, float eps)
{
	return fabs(a - b) <= eps;
}

float Math::Dot(const CVector3& left, const CVector3& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

float Math::Dot(const CVector4& left, const CVector4& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}

float Math::Dot(const CQuaternion& left, const CQuaternion& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}

CVector3 Math::Cross(const CVector3& left, const CVector3& right)
{
	return CVector3(
		left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);
}

CMatrix4 Math::Translate(const CMatrix4& oriMat, const CVector3& v)
{
	CMatrix4 resMat(oriMat);
	resMat.Translate(v);
	return resMat;
}

float Math::Deg2Rad(float degree)
{
	return Math::PI / 180.f * degree;
}

float Math::Rad2Deg(float radian)
{
	return radian / Math::PI * 180.f;
}

CMatrix3 Math::Inverse(const CMatrix3& mat)
{
	// 伴随矩阵法
	float oneOverDeterminant = 1.0f / mat.GetDeterminant();

	CMatrix3 resMat = CMatrix3();
	resMat.m00 = +(mat.m11 * mat.m22 - mat.m12 * mat.m21) * oneOverDeterminant;
	resMat.m01 = -(mat.m10 * mat.m22 - mat.m12 * mat.m20) * oneOverDeterminant;
	resMat.m02 = +(mat.m10 * mat.m21 - mat.m11 * mat.m20) * oneOverDeterminant;

	resMat.m10 = -(mat.m01 * mat.m22 - mat.m02 * mat.m21) * oneOverDeterminant;
	resMat.m11 = +(mat.m00 * mat.m22 - mat.m02 * mat.m20) * oneOverDeterminant;
	resMat.m12 = -(mat.m00 * mat.m21 - mat.m01 * mat.m20) * oneOverDeterminant;

	resMat.m20 = +(mat.m01 * mat.m12 - mat.m02 * mat.m11) * oneOverDeterminant;
	resMat.m21 = -(mat.m00 * mat.m12 - mat.m02 * mat.m10) * oneOverDeterminant;
	resMat.m22 = +(mat.m00 * mat.m11 - mat.m01 * mat.m10) * oneOverDeterminant;

	return resMat;
}

CMatrix3 Math::Transpose(const CMatrix3& mat)
{
	return CMatrix3(
		mat.m00, mat.m10, mat.m20,
		mat.m01, mat.m11, mat.m21,
		mat.m02, mat.m12, mat.m22);
}
#pragma once
#include <cfloat>
#include "CVector3.h"
#include "CVector4.h"
#include "CQuaternion.h"
#include "CMatrix3.h"



class Math
{
public:

	static float PI;
	static float PIx2;
	static float SQRT2;
	static float INV_SQRT2;
	static bool Approximately(float a, float b, float eps = FLT_EPSILON);
	static float Dot(const CVector3& left, const CVector3& right);
	static float Dot(const CVector4& left, const CVector4& right);
	static float Dot(const CQuaternion& left, const CQuaternion& right);
	static CVector3 Cross(const CVector3& left, const CVector3& right);
	static CMatrix4 Translate(const CMatrix4& oriMat, const CVector3& v);
	static float Deg2Rad(float degree);
	static float Rad2Deg(float radian);
	static CMatrix3 Inverse(const CMatrix3& mat);
	static CMatrix3 Transpose(const CMatrix3& mat);

	template<class T>
	static constexpr T Min(T num1, T num2);
	template<class T>
	static constexpr T Max(T num1, T num2);
	template<class T>
	static constexpr T Clamp(T num, T min, T max);
	template<class T>
	static constexpr T AlignUp(T num, T alignment);
	template<class T>
	static constexpr T AlignDown(T num, T alignment);
	template<class T>
	static constexpr T AlignUpPOT(T num, T alignment);
	template<class T>
	static constexpr T AlignDownPOT(T num, T alignment);
};

template<class T>
constexpr T Math::Min(T num1, T num2)
{
	return num1 < num2 ? num1 : num2;
}

template<class T>
constexpr T Math::Max(T num1, T num2)
{
	return num1 > num2 ? num1 : num2;
}

template<class T>
constexpr T Math::Clamp(T num, T min, T max)
{
	return Math::Min(Math::Max(num, min), max);
}

template<class T>
constexpr T Math::AlignUp(T num, T alignment)
{
	return (num + alignment - 1) / alignment * alignment;
}

template<class T>
constexpr T Math::AlignDown(T num, T alignment)
{
	return num / alignment * alignment;
}

template<class T>
constexpr T Math::AlignUpPOT(T num, T alignment)
{
	return (num + alignment - 1) & ~(alignment - 1);
}

template<class T>
constexpr T Math::AlignDownPOT(T num, T alignment)
{
	return num & ~(alignment - 1);
}
#pragma once
#include <cfloat>
#include <type_traits> // for std::common_type_t
#include <random>
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
	static float DEG2RAD;
	static float RAD2DEG;

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

	// ---------- Random Functions ----------
	static void SetRandomSeed(unsigned int seed);
	static float RandomFloat(float min, float max);
	static int RandomInt(int min, int max);
	static bool RandomBool(float probabilityTrue = 0.5f);
	static float RandomRange01();
	static CVector3 RandomVector3(const CVector3& min, const CVector3& max);
	static CVector3 RandomUnitVector3();

	// ---------- Template Functions ----------
	template<typename T1, typename T2>
	static constexpr auto Min(T1 a, T2 b) -> std::common_type_t<T1, T2>
	{
		using Common = std::common_type_t<T1, T2>;
		return static_cast<Common>(a) < static_cast<Common>(b) ? static_cast<Common>(a) : static_cast<Common>(b);
	}

	template<typename T1, typename T2>
	static constexpr auto Max(T1 a, T2 b) -> std::common_type_t<T1, T2>
	{
		using Common = std::common_type_t<T1, T2>;
		return static_cast<Common>(a) > static_cast<Common>(b) ? static_cast<Common>(a) : static_cast<Common>(b);
	}

	template<typename T1, typename T2, typename T3>
	static constexpr auto Clamp(T1 num, T2 min, T3 max) -> std::common_type_t<T1, T2, T3>
	{
		using Common = std::common_type_t<T1, T2, T3>;
		return Math::Min(
			Math::Max(static_cast<Common>(num), static_cast<Common>(min)),
			static_cast<Common>(max)
		);
	}

	template<typename T1, typename T2>
	static constexpr auto AlignUp(T1 num, T2 alignment) -> std::common_type_t<T1, T2>
	{
		using Common = std::common_type_t<T1, T2>;
		return ((static_cast<Common>(num) + static_cast<Common>(alignment) - 1) /
			static_cast<Common>(alignment)) * static_cast<Common>(alignment);
	}

	template<typename T1, typename T2>
	static constexpr auto AlignDown(T1 num, T2 alignment) -> std::common_type_t<T1, T2>
	{
		using Common = std::common_type_t<T1, T2>;
		return (static_cast<Common>(num) / static_cast<Common>(alignment)) * static_cast<Common>(alignment);
	}

	template<typename T1, typename T2>
	static constexpr auto AlignUpPOT(T1 num, T2 alignment) -> std::common_type_t<T1, T2>
	{
		using Common = std::common_type_t<T1, T2>;
		return (static_cast<Common>(num) + static_cast<Common>(alignment) - 1) &
			~(static_cast<Common>(alignment) - 1);
	}

	template<typename T1, typename T2>
	static constexpr auto AlignDownPOT(T1 num, T2 alignment) -> std::common_type_t<T1, T2>
	{
		using Common = std::common_type_t<T1, T2>;
		return static_cast<Common>(num) & ~(static_cast<Common>(alignment) - 1);
	}

private:
	static std::mt19937 rng; // random engine
};

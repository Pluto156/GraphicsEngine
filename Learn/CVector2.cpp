#include "stdafx.h"
#include "Math.h"
#include "CVector2.h"
#include "CVector3.h"
#include "CVector4.h"

const CVector2 CVector2::Zero = CVector2(0.0f, 0.0f);
const CVector2 CVector2::One = CVector2(1.0f, 1.0f);
const CVector2 CVector2::Up = CVector2(0.0f, 1.0f);
const CVector2 CVector2::Down = CVector2(0.0f, -1.0f);
const CVector2 CVector2::Left = CVector2(-1.0f, 0.0f);
const CVector2 CVector2::Right = CVector2(1.0f, 0.0f);

CVector2::CVector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

CVector2::CVector2(float n)
{
	this->x = n;
	this->y = n;
}

CVector2::CVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

CVector2::CVector2(const CVector2& v)
{
	this->x = v.x;
	this->y = v.y;
}

CVector2::CVector2(const CVector3& v)
{
	this->x = v.x;
	this->y = v.y;
}

CVector2::CVector2(const CVector4& v)
{
	this->x = v.x;
	this->y = v.y;
}

void CVector2::Normalize()
{
	float l = sqrtf(powf(x, 2) + powf(y, 2));
	x /= l;
	y /= l;
}

CVector2 CVector2::GetNormalized() const
{
	float l = sqrtf(powf(x, 2) + powf(y, 2));
	return CVector2(x / l, y / l);
}

void CVector2::Absolutize()
{
	x = fabsf(x);
	y = fabsf(y);
}

CVector2 CVector2::GetAbsolutized() const
{
	return CVector2(fabsf(x), fabsf(y));
}

void CVector2::ToArray(float* array) const
{
	array[0] = x;
	array[1] = y;
}

std::string CVector2::ToString() const
{
	return "Vector2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

float CVector2::GetMagnitude() const
{
	return sqrtf(x * x + y * y);
}

float CVector2::GetMagnitudeSquared() const
{
	return x * x + y * y;
}

void CVector2::Clear()
{
	x = y = 0.0f;
}

float& CVector2::operator[] (int i)
{
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		//Debug::LogError("Invalid index to Vector2 !");
		//assert(false);
		return x;
		break;
	}
}

const float& CVector2::operator[] (int i) const
{
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		//Debug::LogError("Invalid index to Vector2 !");
		//assert(false);
		return x;
		break;
	}
}

bool CVector2::operator== (const CVector2& v) const
{
	return Math::Approximately(x, v.x) && Math::Approximately(y, v.y);
}

bool CVector2::operator!= (const CVector2& v) const
{
	return !Math::Approximately(x, v.x) || !Math::Approximately(y, v.y);
}

CVector2& CVector2::operator= (const CVector2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

CVector2 CVector2::operator+ (const CVector2& v) const
{
	return CVector2(x + v.x, y + v.y);
}

CVector2 CVector2::operator- (const CVector2& v) const
{
	return CVector2(x - v.x, y - v.y);
}

CVector2 CVector2::operator* (const CVector2& v) const
{
	return CVector2(x * v.x, y * v.y);
}

CVector2 CVector2::operator/ (const CVector2& v) const
{
	return CVector2(x / v.x, y / v.y);
}

CVector2& CVector2::operator+= (const CVector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

CVector2& CVector2::operator-= (const CVector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

CVector2& CVector2::operator*= (const CVector2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

CVector2& CVector2::operator/= (const CVector2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

CVector2 CVector2::operator+ (float n) const
{
	return CVector2(x + n, y + n);
}

CVector2 CVector2::operator- (float n) const
{
	return CVector2(x - n, y - n);
}

CVector2 CVector2::operator* (float n) const
{
	return CVector2(x * n, y * n);
}

CVector2 CVector2::operator/ (float n) const
{
	return CVector2(x / n, y / n);
}

CVector2& CVector2::operator+= (float n)
{
	x += n;
	y += n;
	return *this;
}

CVector2& CVector2::operator-= (float n)
{
	x -= n;
	y -= n;
	return *this;
}

CVector2& CVector2::operator*= (float n)
{
	x *= n;
	y *= n;
	return *this;
}

CVector2& CVector2::operator/= (float n)
{
	x /= n;
	y /= n;
	return *this;
}

CVector2 operator+ (float n, const CVector2& v)
{
	return CVector2(n + v.x, n + v.y);
}

CVector2 operator- (float n, const CVector2& v)
{
	return CVector2(n - v.x, n - v.y);
}

CVector2 operator* (float n, const CVector2& v)
{
	return CVector2(n * v.x, n * v.y);
}

CVector2 operator/ (float n, const CVector2& v)
{
	return CVector2(n / v.x, n / v.y);
}

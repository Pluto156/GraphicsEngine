#pragma once
#include <string>
class CVector3;
class CVector4;
class CVector2
{
public:
	static const CVector2 Zero;
	static const CVector2 One;
	static const CVector2 Up;
	static const CVector2 Down;
	static const CVector2 Left;
	static const CVector2 Right;

public:
	union
	{
		struct { float v[2]; };
		struct { float x, y; };
		struct { float r, g; };
	};

	CVector2();
	CVector2(float n);
	CVector2(float x, float y);
	CVector2(const CVector2& v);
	CVector2(const CVector3& v);
	CVector2(const CVector4& v);

	void Normalize();
	CVector2 GetNormalized() const;

	void Absolutize();
	CVector2 GetAbsolutized() const;

	float GetMagnitude() const;
	float GetMagnitudeSquared() const;

	void ToArray(float* array) const;
	std::string ToString() const;
	void Clear();

	float& operator[] (int i);
	const float& operator[] (int i) const;
	bool operator== (const CVector2& v) const;
	bool operator!= (const CVector2& v) const;
	CVector2& operator= (const CVector2& v);
	CVector2 operator+ (const CVector2& v) const;
	CVector2 operator- (const CVector2& v) const;
	CVector2 operator* (const CVector2& v) const;
	CVector2 operator/ (const CVector2& v) const;
	CVector2& operator+= (const CVector2& v);
	CVector2& operator-= (const CVector2& v);
	CVector2& operator*= (const CVector2& v);
	CVector2& operator/= (const CVector2& v);
	CVector2 operator+ (float n) const;
	CVector2 operator- (float n) const;
	CVector2 operator* (float n) const;
	CVector2 operator/ (float n) const;
	CVector2& operator+= (float n);
	CVector2& operator-= (float n);
	CVector2& operator*= (float n);
	CVector2& operator/= (float n);
	friend CVector2 operator+ (float n, const CVector2& v);
	friend CVector2 operator- (float n, const CVector2& v);
	friend CVector2 operator* (float n, const CVector2& v);
	friend CVector2 operator/ (float n, const CVector2& v);
};


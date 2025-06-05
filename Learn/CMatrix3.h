#pragma once
class CVector3;
class CMatrix4;
class CMatrix3
{
	friend class Math;
	friend class CVector3;
	friend class CMatrix4;

public:
	static const CMatrix3 Zero;
	static const CMatrix3 Identity;

	static CMatrix3 CreateRotation(const CVector3& axis, float angle);

public:
	CMatrix3();
	CMatrix3(float n);
	CMatrix3(const CMatrix3& mat3);
	CMatrix3(const CMatrix4& mat4);
	CMatrix3(const CVector3& v1, const CVector3& v2, const CVector3& v3);
	CMatrix3(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22
	);
	// 斜对称矩阵构造函数
	CMatrix3(const CVector3& v);
	// 旋转矩阵构造函数(绕axis轴旋转angle度)
	CMatrix3(const CVector3& axis, float angle);

	float GetNorm() const;
	float GetDeterminant() const;
	float Get(uint32_t row, uint32_t col) const;
	CVector3 GetRow(uint32_t index) const;
	CVector3 GetColumn(uint32_t index) const;

	void Transpose();
	CMatrix3 GetTranspose();


	void ToRowMajorArray(float* array) const;
	void ToColumnMajorArray(float* array) const;

	std::string ToString() const;

	CMatrix3& operator= (const CMatrix3& mat);
	bool operator== (const CMatrix3& mat) const;
	bool operator!= (const CMatrix3& mat) const;
	CMatrix3 operator- () const;
	CMatrix3 operator* (float n) const;
	CMatrix3 operator+ (const CMatrix3& mat) const;
	CMatrix3 operator- (const CMatrix3& mat) const;
	CMatrix3 operator* (const CMatrix3& mat) const;
	CMatrix3& operator*= (float n);
	CMatrix3& operator+= (const CMatrix3& mat);
	CMatrix3& operator-= (const CMatrix3& mat);
	CMatrix3& operator*= (const CMatrix3& mat);
	CVector3 operator* (const CVector3& v) const;
	friend CMatrix3 operator* (float n, const CMatrix3& mat);

private:
	union
	{
		float m[3][3];
		struct
		{
			// 第一行
			float m00; float m01; float m02;
			// 第二行
			float m10; float m11; float m12;
			// 第三行
			float m20; float m21; float m22;
		};
	};
};


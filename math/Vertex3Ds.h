#pragma once

#include "Vertex2D.h"

#include "def.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Vertex3Ds
{
public:
	Vertex3Ds();
	Vertex3Ds(const float _x, const float _y, const float _z);

	void Set(const float a, const float b, const float c);
	void SetZero();

	Vertex3Ds operator+(const Vertex3Ds& v) const;
	Vertex3Ds operator-(const Vertex3Ds& v) const;
	Vertex3Ds operator-() const;
	Vertex3Ds& operator+=(const Vertex3Ds& v);
	Vertex3Ds& operator-=(const Vertex3Ds& v);
	Vertex3Ds operator*(const float s) const;

	friend Vertex3Ds operator*(const float s, const Vertex3Ds& v)
	{
		return Vertex3Ds(s * v.x, s * v.y, s * v.z);
	}

	Vertex3Ds operator/(const float s) const;
	Vertex3Ds& operator*=(const float s);
	Vertex3Ds& operator/=(const float s);

	void Normalize();
	void Normalize(const float scalar);
	void NormalizeSafe();
	float Dot(const Vertex3Ds& pv) const;
	float Dot(const Vertex3D_NoTex2& pv) const;
	float LengthSquared() const;
	float Length() const;
	bool IsZero() const;

	Vertex2D& xy();
	const Vertex2D& xy() const;

	float x;
	float y;
	float z;
};

void RotateAround(const Vertex3Ds& pvAxis, Vertex3D_NoTex2* const pvPoint, int count, float angle);
void RotateAround(const Vertex3Ds& pvAxis, Vertex3Ds* const pvPoint, int count, float angle);
Vertex3Ds RotateAround(const Vertex3Ds& pvAxis, const Vertex2D& pvPoint, float angle);

inline Vertex3Ds CrossProduct(const Vertex3Ds& pv1, const Vertex3Ds& pv2)
{
	return Vertex3Ds(pv1.y * pv2.z - pv1.z * pv2.y,
					 pv1.z * pv2.x - pv1.x * pv2.z,
					 pv1.x * pv2.y - pv1.y * pv2.x);
}

////////////////////////////////////////////////////////////////////////////////
//
// license:GPLv3+
// Ported at: VisualPinball.Engine/Math/Vertex3D.cs
//

inline Vertex3Ds GetRotatedAxis(const float angle, const Vertex3Ds& axis, const Vertex3Ds& temp)
{
	Vertex3Ds u = axis;
	u.Normalize();

	const float sinAngle = sinf((float)(M_PI / 180.0) * angle);
	const float cosAngle = cosf((float)(M_PI / 180.0) * angle);
	const float oneMinusCosAngle = 1.0f - cosAngle;

	Vertex3Ds rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;

	rotMatrixRow0.x = u.x * u.x + cosAngle * (1.f - u.x * u.x);
	rotMatrixRow0.y = u.x * u.y * oneMinusCosAngle - sinAngle * u.z;
	rotMatrixRow0.z = u.x * u.z * oneMinusCosAngle + sinAngle * u.y;

	rotMatrixRow1.x = u.x * u.y * oneMinusCosAngle + sinAngle * u.z;
	rotMatrixRow1.y = u.y * u.y + cosAngle * (1.f - u.y * u.y);
	rotMatrixRow1.z = u.y * u.z * oneMinusCosAngle - sinAngle * u.x;

	rotMatrixRow2.x = u.x * u.z * oneMinusCosAngle - sinAngle * u.y;
	rotMatrixRow2.y = u.y * u.z * oneMinusCosAngle + sinAngle * u.x;
	rotMatrixRow2.z = u.z * u.z + cosAngle * (1.f - u.z * u.z);

	return Vertex3Ds(temp.Dot(rotMatrixRow0), temp.Dot(rotMatrixRow1), temp.Dot(rotMatrixRow2));
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//

inline Vertex3Ds sphere_sample(const float u, const float v)
{
	const float phi = v * (float)(2.0 * M_PI);
	const float z = 1.0f - (u + u);
	const float r = sqrtf(1.0f - z * z);
	return Vertex3Ds(cosf(phi) * r, z, sinf(phi) * r);
}

inline Vertex3Ds hemisphere_sample(const float u, const float v)
{
	const float phi = v * (float)(2.0 * M_PI);
	const float cosTheta = 1.0f - u;
	const float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
	return Vertex3Ds(cosf(phi) * sinTheta, cosTheta, sinf(phi) * sinTheta);
}

inline Vertex3Ds cos_hemisphere_sample(const float u, const float v)
{
	const float phi = v * (float)(2.0 * M_PI);
	const float cosTheta = sqrtf(1.0f - u);
	const float sinTheta = sqrtf(u);
	return Vertex3Ds(cosf(phi) * sinTheta, cosTheta, sinf(phi) * sinTheta);
}

inline Vertex3Ds rotate_to_vector_upper(const Vertex3Ds& vec, const Vertex3Ds& normal)
{
	if (normal.y > -0.99999f)
	{
		const float h = 1.0f / (1.0f + normal.y);
		const float hz = h * normal.z;
		const float hzx = hz * normal.x;

		return Vertex3Ds(
			vec.x * (normal.y + hz * normal.z) + vec.y * normal.x - vec.z * hzx,
			vec.y * normal.y - vec.x * normal.x - vec.z * normal.z,
			vec.y * normal.z - vec.x * hzx + vec.z * (normal.y + h * normal.x * normal.x));
	}

	return -vec;
}

inline Vertex3Ds rotate_to_vector_full(const Vertex3Ds& vec, const Vertex3Ds& normal)
{
	if (fabsf(normal.y) <= 0.99999f)
	{
		const float xx = normal.x * normal.x;
		const float zz = normal.z * normal.z;
		const float h = (1.0f - normal.y) / (xx + zz);
		const float hzx = h * normal.z * normal.x;

		return Vertex3Ds(
			vec.x * (normal.y + h * zz) + vec.y * normal.x - vec.z * hzx,
			vec.y * normal.y - vec.x * normal.x - vec.z * normal.z,
			vec.y * normal.z - vec.x * hzx + vec.z * (normal.y + h * xx));
	}

	return (normal.y < 0.0f) ? -vec : vec;
}

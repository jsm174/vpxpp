#pragma once

#include <float.h>
#include <math.h>

class Vertex2D
{
public:
	Vertex2D();
	Vertex2D(const float _x, const float _y);

	void Set(const float a, const float b);
	void SetZero();

	Vertex2D operator+(const Vertex2D& v) const;
	Vertex2D operator-(const Vertex2D& v) const;
	Vertex2D operator-() const;
	Vertex2D& operator+=(const Vertex2D& v);
	Vertex2D& operator-=(const Vertex2D& v);
	Vertex2D operator*(const float s) const;

	friend Vertex2D operator*(const float s, const Vertex2D& v)
	{
		return Vertex2D(s * v.x, s * v.y);
	}

	Vertex2D operator/(const float s) const;
	Vertex2D& operator*=(const float s);
	Vertex2D& operator/=(const float s);

	float Dot(const Vertex2D& pv) const;
	float LengthSquared() const;
	float Length() const;
	void Normalize();
	void NormalizeSafe();
	bool IsZero() const;

	float x;
	float y;
};

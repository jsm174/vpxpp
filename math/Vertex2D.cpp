#include "Vertex2D.h"

Vertex2D::Vertex2D()
{
}

Vertex2D::Vertex2D(const float _x, const float _y) : x(_x), y(_y)
{
}

void Vertex2D::Set(const float a, const float b)
{
	x = a;
	y = b;
}

void Vertex2D::SetZero()
{
	Set(0.f, 0.f);
}

Vertex2D Vertex2D::operator+(const Vertex2D& v) const
{
	return Vertex2D(x + v.x, y + v.y);
}

Vertex2D Vertex2D::operator-(const Vertex2D& v) const
{
	return Vertex2D(x - v.x, y - v.y);
}

Vertex2D Vertex2D::operator-() const
{
	return Vertex2D(-x, -y);
}

Vertex2D& Vertex2D::operator+=(const Vertex2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vertex2D& Vertex2D::operator-=(const Vertex2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vertex2D Vertex2D::operator*(const float s) const
{
	return Vertex2D(s * x, s * y);
}

Vertex2D Vertex2D::operator/(const float s) const
{
	const float invs = 1.0f / s;
	return Vertex2D(x * invs, y * invs);
}

Vertex2D& Vertex2D::operator*=(const float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vertex2D& Vertex2D::operator/=(const float s)
{
	const float invs = 1.0f / s;
	x *= invs;
	y *= invs;
	return *this;
}

float Vertex2D::Dot(const Vertex2D& pv) const
{
	return x * pv.x + y * pv.y;
}

float Vertex2D::LengthSquared() const
{
	return x * x + y * y;
}

float Vertex2D::Length() const
{
	return sqrtf(x * x + y * y);
}

void Vertex2D::Normalize()
{
	const float oneoverlength = 1.0f / Length();
	x *= oneoverlength;
	y *= oneoverlength;
}

void Vertex2D::NormalizeSafe()
{
	if (!IsZero())
		Normalize();
}

bool Vertex2D::IsZero() const
{
	return fabsf(x) <= FLT_MIN && fabsf(y) <= FLT_MIN;
}

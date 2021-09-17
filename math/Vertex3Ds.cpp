#include "Vertex3Ds.h"
#include "Matrix3.h"

Vertex3Ds::Vertex3Ds()
{
}

Vertex3Ds::Vertex3Ds(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)
{
}

void Vertex3Ds::Set(const float a, const float b, const float c)
{
	x = a;
	y = b;
	z = c;
}

void Vertex3Ds::SetZero()
{
	Set(0.f, 0.f, 0.f);
}

Vertex3Ds Vertex3Ds::operator+(const Vertex3Ds& v) const
{
	return Vertex3Ds(x + v.x, y + v.y, z + v.z);
}

Vertex3Ds Vertex3Ds::operator-(const Vertex3Ds& v) const
{
	return Vertex3Ds(x - v.x, y - v.y, z - v.z);
}

Vertex3Ds Vertex3Ds::operator-() const
{
	return Vertex3Ds(-x, -y, -z);
}

Vertex3Ds& Vertex3Ds::operator+=(const Vertex3Ds& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vertex3Ds& Vertex3Ds::operator-=(const Vertex3Ds& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vertex3Ds Vertex3Ds::operator*(const float s) const
{
	return Vertex3Ds(s * x, s * y, s * z);
}

Vertex3Ds Vertex3Ds::operator/(const float s) const
{
	const float invs = 1.0f / s;
	return Vertex3Ds(x * invs, y * invs, z * invs);
}

Vertex3Ds& Vertex3Ds::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
Vertex3Ds& Vertex3Ds::operator/=(const float s)
{
	const float invs = 1.0f / s;
	x *= invs;
	y *= invs;
	z *= invs;
	return *this;
}

void Vertex3Ds::Normalize()
{
	const float oneoverlength = 1.0f / sqrtf(x * x + y * y + z * z);
	x *= oneoverlength;
	y *= oneoverlength;
	z *= oneoverlength;
}
void Vertex3Ds::Normalize(const float scalar)
{
	const float oneoverlength = scalar / sqrtf(x * x + y * y + z * z);
	x *= oneoverlength;
	y *= oneoverlength;
	z *= oneoverlength;
}

void Vertex3Ds::NormalizeSafe()
{
	if (!IsZero())
		Normalize();
}

float Vertex3Ds::Dot(const Vertex3Ds& pv) const
{
	return x * pv.x + y * pv.y + z * pv.z;
}

float Vertex3Ds::Dot(const Vertex3D_NoTex2& pv) const
{
	return x * pv.x + y * pv.y + z * pv.z;
}

float Vertex3Ds::LengthSquared() const
{
	return x * x + y * y + z * z;
}

float Vertex3Ds::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

bool Vertex3Ds::IsZero() const
{
	return fabsf(x) <= FLT_MIN && fabsf(y) <= FLT_MIN && fabsf(z) <= FLT_MIN;
}

Vertex2D& Vertex3Ds::xy()
{
	return *(reinterpret_cast<Vertex2D*>(&x));
}

const Vertex2D& Vertex3Ds::xy() const
{
	return *(reinterpret_cast<const Vertex2D*>(&x));
}

void RotateAround(const Vertex3Ds& pvAxis, Vertex3D_NoTex2* const pvPoint, const int count, const float angle)
{
	Matrix3 mat;
	mat.RotationAroundAxis(pvAxis, angle);

	for (int i = 0; i < count; ++i)
	{
		const float result[3] = {
			mat.m_d[0][0] * pvPoint[i].x + mat.m_d[0][1] * pvPoint[i].y + mat.m_d[0][2] * pvPoint[i].z,
			mat.m_d[1][0] * pvPoint[i].x + mat.m_d[1][1] * pvPoint[i].y + mat.m_d[1][2] * pvPoint[i].z,
			mat.m_d[2][0] * pvPoint[i].x + mat.m_d[2][1] * pvPoint[i].y + mat.m_d[2][2] * pvPoint[i].z};

		pvPoint[i].x = result[0];
		pvPoint[i].y = result[1];
		pvPoint[i].z = result[2];

		const float resultn[3] = {
			mat.m_d[0][0] * pvPoint[i].nx + mat.m_d[0][1] * pvPoint[i].ny + mat.m_d[0][2] * pvPoint[i].nz,
			mat.m_d[1][0] * pvPoint[i].nx + mat.m_d[1][1] * pvPoint[i].ny + mat.m_d[1][2] * pvPoint[i].nz,
			mat.m_d[2][0] * pvPoint[i].nx + mat.m_d[2][1] * pvPoint[i].ny + mat.m_d[2][2] * pvPoint[i].nz};

		pvPoint[i].nx = resultn[0];
		pvPoint[i].ny = resultn[1];
		pvPoint[i].nz = resultn[2];
	}
}

void RotateAround(const Vertex3Ds& pvAxis, Vertex3Ds* const pvPoint, const int count, const float angle)
{
	Matrix3 mat;
	mat.RotationAroundAxis(pvAxis, angle);

	for (int i = 0; i < count; ++i)
	{
		const float result[3] = {
			mat.m_d[0][0] * pvPoint[i].x + mat.m_d[0][1] * pvPoint[i].y + mat.m_d[0][2] * pvPoint[i].z,
			mat.m_d[1][0] * pvPoint[i].x + mat.m_d[1][1] * pvPoint[i].y + mat.m_d[1][2] * pvPoint[i].z,
			mat.m_d[2][0] * pvPoint[i].x + mat.m_d[2][1] * pvPoint[i].y + mat.m_d[2][2] * pvPoint[i].z};

		pvPoint[i].x = result[0];
		pvPoint[i].y = result[1];
		pvPoint[i].z = result[2];
	}
}

Vertex3Ds RotateAround(const Vertex3Ds& pvAxis, const Vertex2D& pvPoint, const float angle)
{
	const float rsin = sinf(angle);
	const float rcos = cosf(angle);

	float matrix[3][2];
	matrix[0][0] = pvAxis.x * pvAxis.x + rcos * (1.0f - pvAxis.x * pvAxis.x);
	matrix[1][0] = pvAxis.x * pvAxis.y * (1.0f - rcos) - pvAxis.z * rsin;
	matrix[2][0] = pvAxis.z * pvAxis.x * (1.0f - rcos) + pvAxis.y * rsin;

	matrix[0][1] = pvAxis.x * pvAxis.y * (1.0f - rcos) + pvAxis.z * rsin;
	matrix[1][1] = pvAxis.y * pvAxis.y + rcos * (1.0f - pvAxis.y * pvAxis.y);
	matrix[2][1] = pvAxis.y * pvAxis.z * (1.0f - rcos) - pvAxis.x * rsin;

	return Vertex3Ds(matrix[0][0] * pvPoint.x + matrix[0][1] * pvPoint.y,
					 matrix[1][0] * pvPoint.x + matrix[1][1] * pvPoint.y,
					 matrix[2][0] * pvPoint.x + matrix[2][1] * pvPoint.y);
}
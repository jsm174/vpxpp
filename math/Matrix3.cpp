#include "Matrix3.h"

Matrix3::Matrix3()
{
}

void Matrix3::scaleX(const float factor)
{
	m_d[0][0] *= factor;
}

void Matrix3::scaleY(const float factor)
{
	m_d[1][1] *= factor;
}

void Matrix3::scaleZ(const float factor)
{
	m_d[2][2] *= factor;
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/Math/Matrix2D.cs
//

void Matrix3::SkewSymmetric(const Vertex3Ds& pv3D)
{
	m_d[0][0] = 0;
	m_d[0][1] = -pv3D.z;
	m_d[0][2] = pv3D.y;
	m_d[1][0] = pv3D.z;
	m_d[1][1] = 0;
	m_d[1][2] = -pv3D.x;
	m_d[2][0] = -pv3D.y;
	m_d[2][1] = pv3D.x;
	m_d[2][2] = 0;
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//

void Matrix3::MulScalar(const float scalar)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int l = 0; l < 3; ++l)
		{
			m_d[i][l] *= scalar;
		}
	}
}

template <class VecType>
Vertex3Ds Matrix3::operator*(const VecType& v) const
{
	return Vertex3Ds(
		m_d[0][0] * v.x + m_d[0][1] * v.y + m_d[0][2] * v.z,
		m_d[1][0] * v.x + m_d[1][1] * v.y + m_d[1][2] * v.z,
		m_d[2][0] * v.x + m_d[2][1] * v.y + m_d[2][2] * v.z);
}

template <class VecType>
Vertex3Ds Matrix3::MulVector(const VecType& v) const
{
	return (*this) * v;
}

template <class VecType>
Vertex3Ds Matrix3::MulVectorT(const VecType& v) const
{
	return Vertex3Ds(
		m_d[0][0] * v.x + m_d[1][0] * v.y + m_d[2][0] * v.z,
		m_d[0][1] * v.x + m_d[1][1] * v.y + m_d[2][1] * v.z,
		m_d[0][2] * v.x + m_d[1][2] * v.y + m_d[2][2] * v.z);
}

void Matrix3::MulMatrices(const Matrix3& pmat1, const Matrix3& pmat2)
{
	Matrix3 matans;
	for (int i = 0; i < 3; ++i)
	{
		for (int l = 0; l < 3; ++l)
		{
			matans.m_d[i][l] = pmat1.m_d[i][0] * pmat2.m_d[0][l] +
							   pmat1.m_d[i][1] * pmat2.m_d[1][l] +
							   pmat1.m_d[i][2] * pmat2.m_d[2][l];
		}
	}
	*this = matans;
}

void Matrix3::MulMatricesAndMulScalar(const Matrix3& pmat1, const Matrix3& pmat2, const float scalar)
{
	Matrix3 matans;
	for (int i = 0; i < 3; ++i)
	{
		for (int l = 0; l < 3; ++l)
		{
			matans.m_d[i][l] = (pmat1.m_d[i][0] * pmat2.m_d[0][l] +
								pmat1.m_d[i][1] * pmat2.m_d[1][l] +
								pmat1.m_d[i][2] * pmat2.m_d[2][l]) *
							   scalar;
		}
	}
	*this = matans;
}

void Matrix3::AddMatrix(const Matrix3& pmat)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int l = 0; l < 3; ++l)
		{
			m_d[i][l] += pmat.m_d[i][l];
		}
	}
}

void Matrix3::OrthoNormalize()
{
	Vertex3Ds vX(m_d[0][0], m_d[1][0], m_d[2][0]);
	Vertex3Ds vY(m_d[0][1], m_d[1][1], m_d[2][1]);
	Vertex3Ds vZ = CrossProduct(vX, vY);
	vX.Normalize();
	vZ.Normalize();
	vY = CrossProduct(vZ, vX);

	m_d[0][0] = vX.x;
	m_d[0][1] = vY.x;
	m_d[0][2] = vZ.x;
	m_d[1][0] = vX.y;
	m_d[1][1] = vY.y;
	m_d[1][2] = vZ.y;
	m_d[2][0] = vX.z;
	m_d[2][1] = vY.z;
	m_d[2][2] = vZ.z;
}

void Matrix3::Identity(const float value)
{
	m_d[0][0] = m_d[1][1] = m_d[2][2] = value;
	m_d[0][1] = m_d[0][2] =
		m_d[1][0] = m_d[1][2] =
			m_d[2][0] = m_d[2][1] = 0.0f;
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/Math/Matrix2D.cs
//

void Matrix3::RotationAroundAxis(const Vertex3Ds& axis, const float angle)
{
	const float rsin = sinf(angle);
	const float rcos = cosf(angle);

	m_d[0][0] = axis.x * axis.x + rcos * (1.0f - axis.x * axis.x);
	m_d[1][0] = axis.x * axis.y * (1.0f - rcos) - axis.z * rsin;
	m_d[2][0] = axis.z * axis.x * (1.0f - rcos) + axis.y * rsin;

	m_d[0][1] = axis.x * axis.y * (1.0f - rcos) + axis.z * rsin;
	m_d[1][1] = axis.y * axis.y + rcos * (1.0f - axis.y * axis.y);
	m_d[2][1] = axis.y * axis.z * (1.0f - rcos) - axis.x * rsin;

	m_d[0][2] = axis.z * axis.x * (1.0f - rcos) - axis.y * rsin;
	m_d[1][2] = axis.y * axis.z * (1.0f - rcos) + axis.x * rsin;
	m_d[2][2] = axis.z * axis.z + rcos * (1.0f - axis.z * axis.z);
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//

void Matrix3::RotationAroundAxis(const Vertex3Ds& axis, const float rsin, const float rcos)
{
	m_d[0][0] = axis.x * axis.x + rcos * (1.0f - axis.x * axis.x);
	m_d[1][0] = axis.x * axis.y * (1.0f - rcos) - axis.z * rsin;
	m_d[2][0] = axis.z * axis.x * (1.0f - rcos) + axis.y * rsin;

	m_d[0][1] = axis.x * axis.y * (1.0f - rcos) + axis.z * rsin;
	m_d[1][1] = axis.y * axis.y + rcos * (1.0f - axis.y * axis.y);
	m_d[2][1] = axis.y * axis.z * (1.0f - rcos) - axis.x * rsin;

	m_d[0][2] = axis.z * axis.x * (1.0f - rcos) - axis.y * rsin;
	m_d[1][2] = axis.y * axis.z * (1.0f - rcos) + axis.x * rsin;
	m_d[2][2] = axis.z * axis.z + rcos * (1.0f - axis.z * axis.z);
}

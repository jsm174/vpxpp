#include "Matrix3D.h"

Matrix3D Matrix3D::ComputeLaybackTransform(const float layback)
{
	Matrix3D matTrans;
	matTrans.SetIdentity();
	matTrans._32 = -tanf(0.5f * ANGTORAD(layback));
	return matTrans;
}

Matrix3D::Matrix3D()
{
}

Matrix3D::Matrix3D(const float Scale)
{
	SetScaling(Scale, Scale, Scale);
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/Math/Matrix3D.cs
//

void Matrix3D::Multiply(const Matrix3D& mult, Matrix3D& result) const
{
	Matrix3D matrixT;
	for (int i = 0; i < 4; ++i)
	{
		for (int l = 0; l < 4; ++l)
		{
			matrixT.m[i][l] = (m[0][l] * mult.m[i][0]) + (m[1][l] * mult.m[i][1]) +
							  (m[2][l] * mult.m[i][2]) + (m[3][l] * mult.m[i][3]);
		}
	}
	result = matrixT;
}

void Matrix3D::SetTranslation(const float tx, const float ty, const float tz)
{
	SetIdentity();
	_41 = tx;
	_42 = ty;
	_43 = tz;
}

void Matrix3D::SetTranslation(const Vertex3Ds& t)
{
	SetTranslation(t.x, t.y, t.z);
}

void Matrix3D::SetScaling(const float sx, const float sy, const float sz)
{
	SetIdentity();
	_11 = sx;
	_22 = sy;
	_33 = sz;
}

void Matrix3D::RotateXMatrix(const float x)
{
	SetIdentity();
	_22 = _33 = cosf(x);
	_23 = sinf(x);
	_32 = -_23;
}

void Matrix3D::RotateYMatrix(const float y)
{
	SetIdentity();
	_11 = _33 = cosf(y);
	_31 = sinf(y);
	_13 = -_31;
}

void Matrix3D::RotateZMatrix(const float z)
{
	SetIdentity();
	_11 = _22 = cosf(z);
	_12 = sinf(z);
	_21 = -_12;
}

void Matrix3D::SetIdentity()
{
	_11 = _22 = _33 = _44 = 1.0f;
	_12 = _13 = _14 = _41 =
		_21 = _23 = _24 = _42 =
			_31 = _32 = _34 = _43 = 0.0f;
}

void Matrix3D::Scale(const float x, const float y, const float z)
{
	_11 *= x;
	_12 *= x;
	_13 *= x;
	_21 *= y;
	_22 *= y;
	_23 *= y;
	_31 *= z;
	_32 *= z;
	_33 *= z;
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//

void Matrix3D::GetRotationPart(Matrix3D& rot)
{
	rot._11 = _11;
	rot._12 = _12;
	rot._13 = _13;
	rot._14 = 0.0f;
	rot._21 = _21;
	rot._22 = _22;
	rot._23 = _23;
	rot._24 = 0.0f;
	rot._31 = _31;
	rot._32 = _32;
	rot._33 = _33;
	rot._34 = 0.0f;
	rot._41 = rot._42 = rot._43 = 0.0f;
	rot._44 = 1.0f;
}

//
// license:GPLv3+
// Ported at: VisualPinball.Engine/Math/Matrix3D.cs
//

Vertex3Ds Matrix3D::MultiplyVector(const Vertex3Ds& v) const
{
	const float xp = _11 * v.x + _21 * v.y + _31 * v.z + _41;
	const float yp = _12 * v.x + _22 * v.y + _32 * v.z + _42;
	const float zp = _13 * v.x + _23 * v.y + _33 * v.z + _43;
	const float wp = _14 * v.x + _24 * v.y + _34 * v.z + _44;

	const float inv_wp = 1.0f / wp;
	return Vertex3Ds(xp * inv_wp, yp * inv_wp, zp * inv_wp);
}

Vertex3Ds Matrix3D::MultiplyVectorNoTranslate(const Vertex3Ds& v) const
{
	const float xp = _11 * v.x + _21 * v.y + _31 * v.z;
	const float yp = _12 * v.x + _22 * v.y + _32 * v.z;
	const float zp = _13 * v.x + _23 * v.y + _33 * v.z;

	return Vertex3Ds(xp, yp, zp);
}

//
// end of license:GPLv3+, back to 'old MAME'-like
//

void Matrix3D::Invert()
{
	int ipvt[4] = {0, 1, 2, 3};

	for (int k = 0; k < 4; ++k)
	{
		float temp = 0.f;
		int l = k;
		for (int i = k; i < 4; ++i)
		{
			const float d = fabsf(m[k][i]);
			if (d > temp)
			{
				temp = d;
				l = i;
			}
		}
		if (l != k)
		{
			const int tmp = ipvt[k];
			ipvt[k] = ipvt[l];
			ipvt[l] = tmp;
			for (int j = 0; j < 4; ++j)
			{
				temp = m[j][k];
				m[j][k] = m[j][l];
				m[j][l] = temp;
			}
		}
		const float d = 1.0f / m[k][k];
		for (int j = 0; j < k; ++j)
		{
			const float c = m[j][k] * d;
			for (int i = 0; i < 4; ++i)
			{
				m[j][i] -= m[k][i] * c;
			}
			m[j][k] = c;
		}
		for (int j = k + 1; j < 4; ++j)
		{
			const float c = m[j][k] * d;
			for (int i = 0; i < 4; ++i)
			{
				m[j][i] -= m[k][i] * c;
			}
			m[j][k] = c;
		}
		for (int i = 0; i < 4; ++i)
		{
			m[k][i] = -m[k][i] * d;
		}
		m[k][k] = d;
	}

	Matrix3D mat3D;
	mat3D.m[ipvt[0]][0] = m[0][0];
	mat3D.m[ipvt[0]][1] = m[0][1];
	mat3D.m[ipvt[0]][2] = m[0][2];
	mat3D.m[ipvt[0]][3] = m[0][3];
	mat3D.m[ipvt[1]][0] = m[1][0];
	mat3D.m[ipvt[1]][1] = m[1][1];
	mat3D.m[ipvt[1]][2] = m[1][2];
	mat3D.m[ipvt[1]][3] = m[1][3];
	mat3D.m[ipvt[2]][0] = m[2][0];
	mat3D.m[ipvt[2]][1] = m[2][1];
	mat3D.m[ipvt[2]][2] = m[2][2];
	mat3D.m[ipvt[2]][3] = m[2][3];
	mat3D.m[ipvt[3]][0] = m[3][0];
	mat3D.m[ipvt[3]][1] = m[3][1];
	mat3D.m[ipvt[3]][2] = m[3][2];
	mat3D.m[ipvt[3]][3] = m[3][3];

	m[0][0] = mat3D.m[0][0];
	m[0][1] = mat3D.m[0][1];
	m[0][2] = mat3D.m[0][2];
	m[0][3] = mat3D.m[0][3];
	m[1][0] = mat3D.m[1][0];
	m[1][1] = mat3D.m[1][1];
	m[1][2] = mat3D.m[1][2];
	m[1][3] = mat3D.m[1][3];
	m[2][0] = mat3D.m[2][0];
	m[2][1] = mat3D.m[2][1];
	m[2][2] = mat3D.m[2][2];
	m[2][3] = mat3D.m[2][3];
	m[3][0] = mat3D.m[3][0];
	m[3][1] = mat3D.m[3][1];
	m[3][2] = mat3D.m[3][2];
	m[3][3] = mat3D.m[3][3];
}

void Matrix3D::Transpose()
{
	Matrix3D tmp;
	for (int i = 0; i < 4; ++i)
	{
		tmp.m[0][i] = m[i][0];
		tmp.m[1][i] = m[i][1];
		tmp.m[2][i] = m[i][2];
		tmp.m[3][i] = m[i][3];
	}
	*this = tmp;
}

template <class VecIn, class VecOut>
void Matrix3D::MultiplyVectorNoTranslate(const VecIn& vIn, VecOut& vOut) const
{
	const float xp = _11 * vIn.x + _21 * vIn.y + _31 * vIn.z;
	const float yp = _12 * vIn.x + _22 * vIn.y + _32 * vIn.z;
	const float zp = _13 * vIn.x + _23 * vIn.y + _33 * vIn.z;

	vOut.x = xp;
	vOut.y = yp;
	vOut.z = zp;
}

template <class VecIn, class VecOut>
void Matrix3D::MultiplyVectorNoTranslateNormal(const VecIn& vIn, VecOut& vOut) const
{
	const float xp = _11 * vIn.nx + _21 * vIn.ny + _31 * vIn.nz;
	const float yp = _12 * vIn.nx + _22 * vIn.ny + _32 * vIn.nz;
	const float zp = _13 * vIn.nx + _23 * vIn.ny + _33 * vIn.nz;

	vOut.x = xp;
	vOut.y = yp;
	vOut.z = zp;
}

template <class VecIn, class VecOut>
void Matrix3D::MultiplyVector(const VecIn& vIn, VecOut& vOut) const
{
	const float xp = _11 * vIn.x + _21 * vIn.y + _31 * vIn.z + _41;
	const float yp = _12 * vIn.x + _22 * vIn.y + _32 * vIn.z + _42;
	const float zp = _13 * vIn.x + _23 * vIn.y + _33 * vIn.z + _43;
	const float wp = _14 * vIn.x + _24 * vIn.y + _34 * vIn.z + _44;

	const float inv_wp = 1.0f / wp;
	vOut.x = xp * inv_wp;
	vOut.y = yp * inv_wp;
	vOut.z = zp * inv_wp;
}

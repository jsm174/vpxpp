#pragma once

#include "Vertex3Ds.h"

class Matrix3
{
public:
	Matrix3();

	void scaleX(const float factor);
	void scaleY(const float factor);
	void scaleZ(const float factor);

	void SkewSymmetric(const Vertex3Ds& pv3D);
	void MulScalar(const float scalar);

	template <class VecType>
	Vertex3Ds operator*(const VecType& v) const;

	template <class VecType>
	Vertex3Ds MulVector(const VecType& v) const;

	template <class VecType>
	Vertex3Ds MulVectorT(const VecType& v) const;

	void MulMatrices(const Matrix3& pmat1, const Matrix3& pmat2);
	void MulMatricesAndMulScalar(const Matrix3& pmat1, const Matrix3& pmat2, const float scalar);
	void AddMatrix(const Matrix3& pmat);
	void OrthoNormalize();
	void Identity(const float value = 1.0f);
	void RotationAroundAxis(const Vertex3Ds& axis, const float angle);
	void RotationAroundAxis(const Vertex3Ds& axis, const float rsin, const float rcos);

	float m_d[3][3];
};
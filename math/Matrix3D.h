#pragma once

#include "D3DX9.h"
#include "Matrix3.h"

class Matrix3D : public D3DMATRIX
{
public:
	Matrix3D();
	Matrix3D(const float Scale);

	void Multiply(const Matrix3D& mult, Matrix3D& result) const;
	void SetTranslation(const float tx, const float ty, const float tz);
	void SetTranslation(const Vertex3Ds& t);
	void SetScaling(const float sx, const float sy, const float sz);
	void RotateXMatrix(const float x);
	void RotateYMatrix(const float y);
	void RotateZMatrix(const float z);
	void SetIdentity();
	void Scale(const float x, const float y, const float z);

	void GetRotationPart(Matrix3D& rot);

	template <class VecIn, class VecOut>
	void MultiplyVector(const VecIn& vIn, VecOut& vOut) const;

	Vertex3Ds MultiplyVector(const Vertex3Ds& v) const;
	Vertex3Ds MultiplyVectorNoTranslate(const Vertex3Ds& v) const;

	template <class VecIn, class VecOut>
	void MultiplyVectorNoTranslate(const VecIn& vIn, VecOut& vOut) const;

	template <class VecIn, class VecOut>
	void MultiplyVectorNoTranslateNormal(const VecIn& vIn, VecOut& vOut) const;

	void Invert();
	void Transpose();
};

Matrix3D ComputeLaybackTransform(float layback);

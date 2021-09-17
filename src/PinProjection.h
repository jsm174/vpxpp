#pragma once

#include "misc.h"
#include "Matrix3D.h"

#include <vector>

class PinProjection
{
public:
	void ScaleView(const float x, const float y, const float z);
	void MultiplyView(const Matrix3D& mat);
	void RotateView(float x, float y, float z);
	void TranslateView(const float x, const float y, const float z);

	void FitCameraToVerticesFS(std::vector<Vertex3Ds>& pvvertex3D, float aspect, float rotation, float inclination, float FOV, float xlatez, float layback);
	void FitCameraToVertices(std::vector<Vertex3Ds>& pvvertex3D, float aspect, float rotation, float inclination, float FOV, float xlatez, float layback);
	void CacheTransform();
	void TransformVertices(const Vertex3Ds* const rgv, const WORD* const rgi, const int count, Vertex2D* const rgvout) const;

	void ComputeNearFarPlane(std::vector<Vertex3Ds>& verts);

	Matrix3D m_matWorld;
	Matrix3D m_matView;
	Matrix3D m_matProj;
	Matrix3D m_matrixTotal;

	RECT m_rcviewport;

	float m_rznear, m_rzfar;
	Vertex3Ds m_vertexcamera;
};

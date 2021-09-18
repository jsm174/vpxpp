#include "PinProjection.h"

void PinProjection::RotateView(float x, float y, float z)
{
	Matrix3D matRotateX, matRotateY, matRotateZ;

	matRotateX.RotateXMatrix(x);
	m_matView.Multiply(matRotateX, m_matView);
	matRotateY.RotateYMatrix(y);
	m_matView.Multiply(matRotateY, m_matView);
	matRotateZ.RotateZMatrix(z);
	m_matView.Multiply(matRotateZ, m_matView);
}

void PinProjection::TranslateView(const float x, const float y, const float z)
{
	Matrix3D matTrans;
	matTrans.SetTranslation(x, y, z);
	m_matView.Multiply(matTrans, m_matView);
}

void PinProjection::ScaleView(const float x, const float y, const float z)
{
	m_matView.Scale(x, y, z);
}

void PinProjection::MultiplyView(const Matrix3D& mat)
{
	m_matView.Multiply(mat, m_matView);
}

void PinProjection::FitCameraToVerticesFS(std::vector<Vertex3Ds>& pvvertex3D, float aspect, float rotation, float inclination, float FOV, float xlatez, float layback)
{
	const float rrotsin = sinf(rotation);
	const float rrotcos = cosf(rotation);
	const float rincsin = sinf(inclination);
	const float rinccos = cosf(inclination);

	const float slopey = tanf(0.5f * ANGTORAD(FOV));

	const float slopex = slopey * aspect;

	float maxyintercept = -FLT_MAX;
	float minyintercept = FLT_MAX;
	float maxxintercept = -FLT_MAX;
	float minxintercept = FLT_MAX;

	for (size_t i = 0; i < pvvertex3D.size(); ++i)
	{
		Vertex3Ds v = pvvertex3D[i];
		float temp;

		temp = v.y;
		v.y = rinccos * temp - rincsin * v.z;
		v.z = rincsin * temp + rinccos * v.z;

		temp = v.x;
		v.x = rrotcos * temp - rrotsin * v.y;
		v.y = rrotsin * temp + rrotcos * v.y;

		maxyintercept = max(maxyintercept, v.y + slopey * v.z);
		minyintercept = min(minyintercept, v.y - slopey * v.z);
		maxxintercept = max(maxxintercept, v.x + slopex * v.z);
		minxintercept = min(minxintercept, v.x - slopex * v.z);
	}

	// TODO: slintf("maxy: %f\n", maxyintercept);
	// TODO: slintf("miny: %f\n", minyintercept);
	// TODO: slintf("maxx: %f\n", maxxintercept);
	// TODO: slintf("minx: %f\n", minxintercept);

	const float ydist = (maxyintercept - minyintercept) / (slopey * 2.0f);
	const float xdist = (maxxintercept - minxintercept) / (slopex * 2.0f);
	m_vertexcamera.z = (float)(max(ydist, xdist)) + xlatez;
	m_vertexcamera.y = (float)((maxyintercept + minyintercept) * 0.5f);
	m_vertexcamera.x = (float)((maxxintercept + minxintercept) * 0.5f);
}

void PinProjection::FitCameraToVertices(std::vector<Vertex3Ds>& pvvertex3D, float aspect, float rotation, float inclination, float FOV, float xlatez, float layback)
{
	const float rrotsin = sinf(rotation);
	const float rrotcos = cosf(rotation);
	const float rincsin = sinf(inclination);
	const float rinccos = cosf(inclination);

	const float slopey = tanf(0.5f * ANGTORAD(FOV));

	const float slopex = slopey * aspect;

	float maxyintercept = -FLT_MAX;
	float minyintercept = FLT_MAX;
	float maxxintercept = -FLT_MAX;
	float minxintercept = FLT_MAX;

	Matrix3D laybackTrans = ComputeLaybackTransform(layback);

	for (size_t i = 0; i < pvvertex3D.size(); ++i)
	{
		Vertex3Ds v = laybackTrans.MultiplyVector(pvvertex3D[i]);

		float temp = v.y;
		v.y = rinccos * temp - rincsin * v.z;
		v.z = rincsin * temp + rinccos * v.z;

		temp = v.x;
		v.x = rrotcos * temp - rrotsin * v.y;
		v.y = rrotsin * temp + rrotcos * v.y;

		maxyintercept = max(maxyintercept, v.y + slopey * v.z);
		minyintercept = min(minyintercept, v.y - slopey * v.z);
		maxxintercept = max(maxxintercept, v.x + slopex * v.z);
		minxintercept = min(minxintercept, v.x - slopex * v.z);
	}

	// TODO: slintf("maxy: %f\n", maxyintercept);
	// TODO: slintf("miny: %f\n", minyintercept);
	// TODO: slintf("maxx: %f\n", maxxintercept);
	// TODO: slintf("minx: %f\n", minxintercept);

	const float ydist = (maxyintercept - minyintercept) / (slopey * 2.0f);
	const float xdist = (maxxintercept - minxintercept) / (slopex * 2.0f);
	m_vertexcamera.z = max(ydist, xdist) + xlatez;
	m_vertexcamera.y = (maxyintercept + minyintercept) * 0.5f;
	m_vertexcamera.x = (maxxintercept + minxintercept) * 0.5f;
}

void PinProjection::ComputeNearFarPlane(std::vector<Vertex3Ds>& verts)
{
	m_rznear = FLT_MAX;
	m_rzfar = -FLT_MAX;

	Matrix3D matWorldView;
	m_matView.Multiply(m_matWorld, matWorldView);

	for (size_t i = 0; i < verts.size(); ++i)
	{
		const float tempz = matWorldView.MultiplyVector(verts[i]).z;

		m_rznear = min(m_rznear, tempz);
		m_rzfar = max(m_rzfar, tempz);
	}

	// TODO: slintf("m_rznear: %f\n", m_rznear);
	// TODO: slintf("m_rzfar : %f\n", m_rzfar);

	if (m_rznear < 0.001f)
	{
		m_rznear = 0.001f;
	}

	m_rzfar *= 1.01f;
}

void PinProjection::CacheTransform()
{
	Matrix3D matT;
	m_matProj.Multiply(m_matView, matT);
	matT.Multiply(m_matWorld, m_matrixTotal);
}

void PinProjection::TransformVertices(const Vertex3Ds* const rgv, const WORD* const rgi, const int count, Vertex2D* const rgvout) const
{
	const float rClipWidth = (m_rcviewport.right - m_rcviewport.left) * 0.5f;
	const float rClipHeight = (m_rcviewport.bottom - m_rcviewport.top) * 0.5f;
	const int xoffset = m_rcviewport.left;
	const int yoffset = m_rcviewport.top;

	for (int i = 0; i < count; ++i)
	{
		const int l = rgi ? rgi[i] : i;

		const float x = rgv[l].x;
		const float y = rgv[l].y;
		const float z = rgv[l].z;

		const float xp = m_matrixTotal._11 * x + m_matrixTotal._21 * y + m_matrixTotal._31 * z + m_matrixTotal._41;
		const float yp = m_matrixTotal._12 * x + m_matrixTotal._22 * y + m_matrixTotal._32 * z + m_matrixTotal._42;
		const float wp = m_matrixTotal._14 * x + m_matrixTotal._24 * y + m_matrixTotal._34 * z + m_matrixTotal._44;

		const float inv_wp = 1.0f / wp;
		const float vTx = (1.0f + xp * inv_wp) * rClipWidth + xoffset;
		const float vTy = (1.0f - yp * inv_wp) * rClipHeight + yoffset;

		rgvout[l].x = vTx;
		rgvout[l].y = vTy;
	}
}
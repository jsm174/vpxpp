#pragma once

#include "PinProjection.h"
#include "RenderDevice.h"
#include "Vertex3Ds.h"
#include "misc.h"

class Pin3D
{
public:
	Pin3D();
	virtual ~Pin3D();

	RenderDevice* m_pd3dPrimaryDevice;

	void RenderPlayfieldGraphics(const bool depth_only);

	HRESULT InitPin3D(const bool fullScreen, const int width, const int height, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl);
	void InitLayout(const bool FSS_mode, const float xpixoff = 0.f, const float ypixoff = 0.f);

	void InitRenderState(RenderDevice * const pd3dDevice);
	void InitPrimaryRenderState();
	void DrawBackground();

	PinProjection m_proj;

	Vertex3Ds m_cam;
	float m_inc;

	ViewPort m_viewPort;

private:
	HRESULT InitPrimary(const bool fullScreen, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl);
};

D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX* pout, float fovy, float aspect, float zn, float zf);
D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* pout);

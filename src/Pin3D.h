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

	void InitRenderState(RenderDevice* const pd3dDevice);
	void InitPrimaryRenderState();
	void DrawBackground();

	void EnableAlphaBlend(const bool additiveBlending, const bool set_dest_blend = true, const bool set_blend_op = true) const;
	void DisableAlphaBlend() const;

	void Flip(const bool vsync);

	void InitPlayfieldGraphics();

	PinProjection m_proj;

	Vertex3Ds m_cam;
	float m_inc;

	ViewPort m_viewPort;

	Texture m_pinballEnvTexture;
	Texture m_builtinEnvTexture;
	Texture m_aoDitherTexture;

	Texture* m_envTexture;
	BaseTexture* m_envRadianceTexture;

private:
	void EnvmapPrecalc(const void* __restrict envmap, const DWORD env_xres, const DWORD env_yres, void* const __restrict rad_envmap, const DWORD rad_env_xres, const DWORD rad_env_yres, const bool isHDR);
	HRESULT InitPrimary(const bool fullScreen, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl);
};

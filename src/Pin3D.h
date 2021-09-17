#pragma once

#include "PinProjection.h"
#include "misc.h"

class Pin3D
{
public:
	Pin3D();
	virtual ~Pin3D();

	void RenderPlayfieldGraphics(const bool depth_only);

	HRESULT InitPin3D(const bool fullScreen, const int width, const int height, const int colordepth, int& refreshrate, const int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool useAO, const bool ss_refl);
  	void InitLayout(const bool FSS_mode, const float xpixoff = 0.f, const float ypixoff = 0.f);

	PinProjection m_proj;

private:
};
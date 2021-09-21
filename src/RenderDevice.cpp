#include "RenderDevice.h"

RenderDevice::RenderDevice(/* TODO: const HWND hwnd,*/ const int width, const int height, const bool fullscreen, const int colordepth, int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool ss_refl, /* TODO: const bool useNvidiaApi, */ const bool disable_dwm, const int BWrendering)
	: /* TODO: m_windowHwnd(hwnd),*/
	  m_width(width),
	  m_height(height),
	  m_fullscreen(fullscreen),
	  m_colorDepth(colordepth),
	  m_vsync(VSync),
	  m_useAA(useAA),
	  m_stereo3D(stereo3D),
	  m_ssRefl(ss_refl),
	  m_disableDwm(disable_dwm),
	  m_sharpen(sharpen),
	  m_FXAA(FXAA),
	  m_BWrendering(BWrendering),
	  /* TODO: m_useNvidiaApi(useNvidiaApi),*/
	  m_texMan(*this)
{
}

void RenderDevice::SetTransform(const TransformStateType p1, const D3DMATRIX* p2)
{
	// TODO: CHECKD3D(m_pD3DDevice->SetTransform((D3DTRANSFORMSTATETYPE)p1, p2));
}

void RenderDevice::GetTransform(const TransformStateType p1, D3DMATRIX* p2)
{
	// TODO: CHECKD3D(m_pD3DDevice->GetTransform((D3DTRANSFORMSTATETYPE)p1, p2));
}

void RenderDevice::BeginScene()
{
	// TODO: CHECKD3D(m_pD3DDevice->BeginScene());
}

void RenderDevice::EndScene()
{
	// TODO: CHECKD3D(m_pD3DDevice->EndScene());
}

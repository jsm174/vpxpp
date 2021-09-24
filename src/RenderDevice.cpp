#include "RenderDevice.h"

unsigned RenderDevice::m_curLockCalls = 0;
unsigned RenderDevice::m_frameLockCalls = 0;

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

void RenderDevice::CreateDevice(int& refreshrate, UINT adapterIndex)
{
}

bool RenderDevice::LoadShaders()
{
	return true;
}

void RenderDevice::BeginScene()
{
	// TODO: CHECKD3D(m_pD3DDevice->BeginScene());
}

void RenderDevice::EndScene()
{
	// TODO: CHECKD3D(m_pD3DDevice->EndScene());
}

void RenderDevice::CreateVertexBuffer(const unsigned int vertexCount, const DWORD usage, const DWORD fvf, VertexBuffer** vBuffer)
{
}

void RenderDevice::CreateIndexBuffer(const unsigned int numIndices, const DWORD usage, const IndexBuffer::Format format, IndexBuffer** idxBuffer)
{
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const unsigned int numIndices, const WORD* indices)
{
	IndexBuffer* ib;
	CreateIndexBuffer(numIndices, 0, IndexBuffer::FMT_INDEX16, &ib);

	/* TODO: void* buf;
   ib->lock(0, 0, &buf, 0);
   memcpy(buf, indices, numIndices * sizeof(indices[0]));
   ib->unlock();*/

	return ib;
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const unsigned int numIndices, const unsigned int* indices)
{
	IndexBuffer* ib;
	CreateIndexBuffer(numIndices, 0, IndexBuffer::FMT_INDEX32, &ib);

	/* TODO: void* buf;
   ib->lock(0, 0, &buf, 0);
   memcpy(buf, indices, numIndices * sizeof(indices[0]));
   ib->unlock();*/

	return ib;
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const std::vector<WORD>& indices)
{
	return CreateAndFillIndexBuffer((unsigned int)indices.size(), indices.data());
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const std::vector<unsigned int>& indices)
{
	return CreateAndFillIndexBuffer((unsigned int)indices.size(), indices.data());
}

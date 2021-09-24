#pragma once

#include "IndexBuffer.h"
#include "TextureManager.h"
#include "VertexBuffer.h"

class RenderDevice
{
public:
	static unsigned m_curLockCalls;
	static unsigned m_frameLockCalls;

	enum RenderStates
	{
	};

	RenderDevice(/*const HWND hwnd,*/ const int width, const int height, const bool fullscreen, const int colordepth, int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool ss_refl, /*const bool useNvidiaApi,*/ const bool disable_dwm, const int BWrendering);
	~RenderDevice();

	void SetTransform(const TransformStateType, const D3DMATRIX*);
	void GetTransform(const TransformStateType, D3DMATRIX*);

	void CreateDevice(int& refreshrate, UINT adapterIndex);
	bool LoadShaders();

	void BeginScene();
	void EndScene();

	void CreateVertexBuffer(const unsigned int numVerts, const DWORD usage, const DWORD fvf, VertexBuffer** vBuffer);
	void CreateIndexBuffer(const unsigned int numIndices, const DWORD usage, const IndexBuffer::Format format, IndexBuffer** idxBuffer);

	IndexBuffer* CreateAndFillIndexBuffer(const unsigned int numIndices, const unsigned int* indices);
	IndexBuffer* CreateAndFillIndexBuffer(const unsigned int numIndices, const WORD* indices);
	IndexBuffer* CreateAndFillIndexBuffer(const std::vector<unsigned int>& indices);
	IndexBuffer* CreateAndFillIndexBuffer(const std::vector<WORD>& indices);

	int m_width;
	int m_height;
	bool m_fullscreen;
	int m_colorDepth;
	int m_vsync;
	bool m_useAA;
	bool m_stereo3D;
	bool m_ssRefl;
	bool m_disableDwm;
	bool m_sharpen;
	unsigned int m_FXAA;
	int m_BWrendering;

	TextureManager m_texMan;

private:
	// TODO: IDirect3DDevice9* m_pD3DDevice;
};
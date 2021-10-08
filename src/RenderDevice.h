#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "TextureManager.h"
#include "VertexBuffer.h"

#include "D3DX9.h"

#include <bgfx/bgfx.h>

class RenderDevice
{
public:
	enum TransformStateType
	{
		TRANSFORMSTATE_WORLD = D3DTS_WORLD,
		TRANSFORMSTATE_VIEW = D3DTS_VIEW,
		TRANSFORMSTATE_PROJECTION = D3DTS_PROJECTION
	};

	enum PrimitiveTypes
	{
		TRIANGLEFAN,   // TODO: = D3DPT_TRIANGLEFAN,
		TRIANGLESTRIP, // TODO: = D3DPT_TRIANGLESTRIP,
		TRIANGLELIST,  // TODO: = D3DPT_TRIANGLELIST,
		POINTLIST,	   // TODO: = D3DPT_POINTLIST,
		LINELIST,	   // TODO: = D3DPT_LINELIST,
		LINESTRIP,	   // TODO: = D3DPT_LINESTRIP
	};

	enum RenderStates
	{
		ALPHABLENDENABLE, // TODO: = D3DRS_ALPHABLENDENABLE,
		ALPHATESTENABLE,  // TODO: = D3DRS_ALPHATESTENABLE,
		ALPHAREF,		  // TODO: = D3DRS_ALPHAREF,
		ALPHAFUNC,		  // TODO: = D3DRS_ALPHAFUNC,
		BLENDOP,		  // TODO: = D3DRS_BLENDOP,
		CLIPPING,		  // TODO: = D3DRS_CLIPPING,
		CLIPPLANEENABLE,  // TODO: = D3DRS_CLIPPLANEENABLE,
		CULLMODE,		  // TODO: = D3DRS_CULLMODE,
		DESTBLEND,		  // TODO: = D3DRS_DESTBLEND,
		LIGHTING,		  // TODO: = D3DRS_LIGHTING,
		SRCBLEND,		  // TODO: = D3DRS_SRCBLEND,
		SRGBWRITEENABLE,  // TODO: = D3DRS_SRGBWRITEENABLE,
		ZENABLE,		  // TODO: = D3DRS_ZENABLE,
		ZFUNC,			  // TODO: = D3DRS_ZFUNC,
		ZWRITEENABLE,	  // TODO: = D3DRS_ZWRITEENABLE,
		TEXTUREFACTOR,	  // TODO: = D3DRS_TEXTUREFACTOR,
		DEPTHBIAS,		  // TODO: = D3DRS_DEPTHBIAS,
		COLORWRITEENABLE, // TODO: = D3DRS_COLORWRITEENABLE
	};

	enum RenderStateValue
	{
		RS_FALSE,			 // TODO: = FALSE,
		RS_TRUE,			 // TODO:  = TRUE,
		CULL_NONE,			 // TODO:  = D3DCULL_NONE,
		CULL_CW,			 // TODO:  = D3DCULL_CW,
		CULL_CCW,			 // TODO:  = D3DCULL_CCW,
		Z_ALWAYS,			 // TODO:  = D3DCMP_ALWAYS,
		Z_LESS,				 // TODO:  = D3DCMP_LESS,
		Z_LESSEQUAL,		 // TODO:  = D3DCMP_LESSEQUAL,
		Z_GREATER,			 // TODO:  = D3DCMP_GREATER,
		Z_GREATEREQUAL,		 // TODO:  = D3DCMP_GREATEREQUAL,
		BLENDOP_MAX,		 // TODO:  = D3DBLENDOP_MAX,
		BLENDOP_ADD,		 // TODO:  = D3DBLENDOP_ADD,
		BLENDOP_REVSUBTRACT, // TODO:  = D3DBLENDOP_REVSUBTRACT,
		ZERO,				 // TODO:  = D3DBLEND_ZERO,
		ONE,				 // TODO:  = D3DBLEND_ONE,
		SRC_ALPHA,			 // TODO:  = D3DBLEND_SRCALPHA,
		DST_ALPHA,			 // TODO:  = D3DBLEND_DESTALPHA,
		INVSRC_ALPHA,		 // TODO:  = D3DBLEND_INVSRCALPHA,
		INVSRC_COLOR,		 // TODO:  = D3DBLEND_INVSRCCOLOR,
		PLANE0,				 // TODO:  = D3DCLIPPLANE0,
		UNDEFINED
	};

	static unsigned m_curLockCalls;
	static unsigned m_frameLockCalls;

	RenderDevice(/*const HWND hwnd,*/ const int width, const int height, const bool fullscreen, const int colordepth, int VSync, const bool useAA, const bool stereo3D, const unsigned int FXAA, const bool sharpen, const bool ss_refl, /*const bool useNvidiaApi,*/ const bool disable_dwm, const int BWrendering);
	~RenderDevice();

	void SetViewport(const ViewPort*);
	void GetViewport(ViewPort*);

	void SetTransform(const TransformStateType, const D3DMATRIX*);
	void GetTransform(const TransformStateType, D3DMATRIX*);

	void Clear(const DWORD numRects, const D3DRECT* rects, const DWORD flags, const D3DCOLOR color, const D3DVALUE z, const DWORD stencil);

	void ForceAnisotropicFiltering(const bool enable);
	void CompressTextures(const bool enable);

	void CreateDevice(int& refreshrate, UINT adapterIndex);
	bool LoadShaders();

	void BeginScene();
	void EndScene();

	void Flip(const bool vsync);

	void CreateVertexBuffer(const unsigned int numVerts, const DWORD usage, const DWORD fvf, VertexBuffer** vBuffer);
	void CreateIndexBuffer(const unsigned int numIndices, const DWORD usage, const IndexBuffer::Format format, IndexBuffer** idxBuffer);

	IndexBuffer* CreateAndFillIndexBuffer(const unsigned int numIndices, const unsigned int* indices);
	IndexBuffer* CreateAndFillIndexBuffer(const unsigned int numIndices, const WORD* indices);
	IndexBuffer* CreateAndFillIndexBuffer(const std::vector<unsigned int>& indices);
	IndexBuffer* CreateAndFillIndexBuffer(const std::vector<WORD>& indices);

	bgfx::TextureHandle CreateSystemTexture(BaseTexture* const surf, const bool linearRGB);
	void UpdateTexture(bgfx::TextureHandle, BaseTexture* const surf, const bool linearRGB);
	bgfx::TextureHandle UploadTexture(BaseTexture* const surf, int* const pTexWidth, int* const pTexHeight, const bool linearRGB);

	bool SetRenderStateCache(const RenderStates p1, DWORD p2);
	void SetRenderState(const RenderStates p1, DWORD p2);

	void DrawIndexedPrimitiveVB(const PrimitiveTypes type, const DWORD fvf, VertexBuffer* vb, const DWORD startVertex, const DWORD vertexCount, IndexBuffer* ib, const DWORD startIndex, const DWORD indexCount);

	unsigned int Perf_GetNumDrawCalls() const; // DELETEME: Line 300
	unsigned int Perf_GetNumStateChanges() const;
	unsigned int Perf_GetNumTextureChanges() const;
	unsigned int Perf_GetNumParameterChanges() const;
	unsigned int Perf_GetNumTechniqueChanges() const;
	unsigned int Perf_GetNumTextureUploads() const;
	unsigned int Perf_GetNumLockCalls() const;

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

	bool m_force_aniso;
	bool m_compress_textures;

	TextureManager m_texMan;

	unsigned int m_stats_drawn_triangles;

	unsigned int m_curDrawCalls;
	unsigned int m_frameDrawCalls;
	unsigned int m_curStateChanges;
	unsigned int m_frameStateChanges;
	unsigned int m_curTextureChanges;
	unsigned int m_frameTextureChanges;
	unsigned int m_curParameterChanges;
	unsigned int m_frameParameterChanges;
	unsigned int m_curTechniqueChanges;
	unsigned int m_frameTechniqueChanges;
	unsigned int m_curTextureUpdates;
	unsigned int m_frameTextureUpdates;

	Shader* basicShader;

private:
	VertexBuffer* m_curVertexBuffer;
	IndexBuffer* m_curIndexBuffer;

	D3DMATRIX m_matProj;
	D3DMATRIX m_matView;
	D3DMATRIX m_matWorld;

	std::map<RenderStates, DWORD> renderStateCache;

	Material currentMaterial;
	// TODO: IDirect3DDevice9* m_pD3DDevice;
};
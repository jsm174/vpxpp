#include "RenderDevice.h"

#include "Player.h"
#include "Vertex3D_TexelOnly.h"
#include "extern.h"

#include <assert.h>
#include <iostream>

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
	m_stats_drawn_triangles = 0;

	m_curIndexBuffer = NULL;
	m_curVertexBuffer = NULL;
	// TODO: currentDeclaration = NULL;

	// TODO: memset(textureStateCache, 0xCC, sizeof(DWORD) * TEXTURE_SAMPLERS * TEXTURE_STATE_CACHE_SIZE);
	// TODO: memset(textureSamplerCache, 0xCC, sizeof(DWORD) * TEXTURE_SAMPLERS * TEXTURE_SAMPLER_CACHE_SIZE);

	m_curDrawCalls = 0;
	m_frameDrawCalls = 0;
	m_curStateChanges = 0;
	m_frameStateChanges = 0;
	m_curTextureChanges = 0;
	m_frameTextureChanges = 0;
	m_curParameterChanges = 0;
	m_frameParameterChanges = 0;
	m_curTechniqueChanges = 0;
	m_frameTechniqueChanges = 0;
	m_curTextureUpdates = 0;
	m_frameTextureUpdates = 0;

	m_curLockCalls = 0;
	m_frameLockCalls = 0;
}

void RenderDevice::SetTransform(const TransformStateType p1, const D3DMATRIX* p2)
{
	// TODO: CHECKD3D(m_pD3DDevice->SetTransform((D3DTRANSFORMSTATETYPE)p1, p2));
}

void RenderDevice::GetTransform(const TransformStateType p1, D3DMATRIX* p2)
{
	// TODO: CHECKD3D(m_pD3DDevice->GetTransform((D3DTRANSFORMSTATETYPE)p1, p2));
}

void RenderDevice::Clear(const DWORD numRects, const D3DRECT* rects, const DWORD flags, const D3DCOLOR color, const D3DVALUE z, const DWORD stencil)
{
	// TODO: CHECKD3D(m_pD3DDevice->Clear(numRects, rects, flags, color, z, stencil));
}

void RenderDevice::SetViewport(const ViewPort* p1)
{
	bgfx::setViewRect(0, p1->X, p1->Y, p1->Width, p1->Height);

	// TODO: CHECKD3D(m_pD3DDevice->SetViewport((D3DVIEWPORT9*)p1));
}

void RenderDevice::GetViewport(ViewPort* p1)
{
	// TODO: CHECKD3D(m_pD3DDevice->GetViewport((D3DVIEWPORT9*)p1));
}

void RenderDevice::ForceAnisotropicFiltering(const bool enable)
{
	m_force_aniso = enable;
}

void RenderDevice::CompressTextures(const bool enable)
{
	m_compress_textures = enable;
}

void RenderDevice::CreateDevice(int& refreshrate, UINT adapterIndex)
{
}

bool RenderDevice::LoadShaders()
{
	static const std::vector<D3DXHANDLE> basicShaders = {
		"basic_without_texture_isMetal",
		"basic_without_texture_isNotMetal",
		"basic_with_texture_isMetal",
		"basic_with_texture_isNotMetal"};

	basicShader = new Shader(this);
	basicShader->Load(basicShaders);

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

void RenderDevice::Flip(const bool vsync)
{
	m_frameDrawCalls = m_curDrawCalls;
	m_frameStateChanges = m_curStateChanges;
	m_frameTextureChanges = m_curTextureChanges;
	m_frameParameterChanges = m_curParameterChanges;
	m_frameTechniqueChanges = m_curTechniqueChanges;

	m_curDrawCalls = 0;
	m_curStateChanges = 0;
	m_curTextureChanges = 0;
	m_curParameterChanges = 0;
	m_curTechniqueChanges = 0;
	m_frameTextureUpdates = 0;
	m_curTextureUpdates = 0;
	m_curTextureUpdates = 0;

	m_frameLockCalls = m_curLockCalls;
	m_curLockCalls = 0;
}

static unsigned int fvfToSize(const DWORD fvf)
{
	switch (fvf)
	{
	case MY_D3DFVF_NOTEX2_VERTEX:
	case MY_D3DTRANSFORMED_NOTEX2_VERTEX:
		return sizeof(Vertex3D_NoTex2);
	case MY_D3DFVF_TEX:
		return sizeof(Vertex3D_TexelOnly);
	default:
		assert(0 && "Unknown FVF type in fvfToSize");
		return 0;
	}
}

void RenderDevice::CreateVertexBuffer(const unsigned int vertexCount, const DWORD usage, const DWORD fvf, VertexBuffer** vBuffer)
{
	HRESULT hr;
	hr = VertexBuffer::Create(vertexCount, fvf, vBuffer);

	// TODO: hr = m_pD3DDevice->CreateVertexBuffer(vertexCount * fvfToSize(fvf), USAGE_STATIC | usage, 0,
	// TODO:									  (D3DPOOL)memoryPool::DEFAULT, (IDirect3DVertexBuffer9**)vBuffer, NULL);
	// TODO: if (FAILED(hr))
	// TODO:	ReportError("Fatal Error: unable to create vertex buffer!", hr, __FILE__, __LINE__);*/
}

void RenderDevice::CreateIndexBuffer(const unsigned int numIndices, const DWORD usage, const IndexBuffer::Format format, IndexBuffer** idxBuffer)
{
	HRESULT hr;
	const unsigned idxSize = (format == IndexBuffer::FMT_INDEX16) ? 2 : 4;
	hr = IndexBuffer::Create(idxSize * numIndices, format, idxBuffer);

	// TODO: hr = m_pD3DDevice->CreateIndexBuffer(idxSize * numIndices, usage | USAGE_STATIC, (D3DFORMAT)format,
	// TODO:									 (D3DPOOL)memoryPool::DEFAULT, (IDirect3DIndexBuffer9**)idxBuffer, NULL);
	// TODO: if (FAILED(hr))
	// TODO:	ReportError("Fatal Error: unable to create index buffer!", hr, __FILE__, __LINE__);
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const unsigned int numIndices, const WORD* indices)
{
	IndexBuffer* idxBuffer;
	CreateIndexBuffer(numIndices, 0, IndexBuffer::FMT_INDEX16, &idxBuffer);

	// void* buf;
	// idxBuffer->lock(0, 0, &buf, 0);
	// memcpy(buf, indices, numIndices * sizeof(indices[0]));

	idxBuffer->CopyMemory(indices, 0, numIndices * sizeof(indices[0]));

	// idxBuffer->unlock();

	return idxBuffer;
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const unsigned int numIndices, const unsigned int* indices)
{
	IndexBuffer* idxBuffer;
	CreateIndexBuffer(numIndices, 0, IndexBuffer::FMT_INDEX32, &idxBuffer);

	// void* buf;
	// idxBuffer->lock(0, 0, &buf, 0);
	// memcpy(buf, indices, numIndices * sizeof(indices[0]));

	idxBuffer->CopyMemory(indices, 0, numIndices * sizeof(indices[0]));

	// idxBuffer->unlock();

	return idxBuffer;
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const std::vector<WORD>& indices)
{
	return CreateAndFillIndexBuffer((unsigned int)indices.size(), indices.data());
}

IndexBuffer* RenderDevice::CreateAndFillIndexBuffer(const std::vector<unsigned int>& indices)
{
	return CreateAndFillIndexBuffer((unsigned int)indices.size(), indices.data());
}

bgfx::TextureHandle RenderDevice::CreateSystemTexture(BaseTexture* const surf, const bool linearRGB)
{
	return BGFX_INVALID_HANDLE;
}

void RenderDevice::UpdateTexture(bgfx::TextureHandle, BaseTexture* const surf, const bool linearRGB)
{
	bgfx::TextureHandle sysTex = CreateSystemTexture(surf, linearRGB);
	m_curTextureUpdates++;

	// TODO: IDirect3DTexture9* sysTex = CreateSystemTexture(surf, linearRGB);
	// TODO: m_curTextureUpdates++;
	// TODO: CHECKD3D(m_pD3DDevice->UpdateTexture(sysTex, tex));
	// TODO: SAFE_RELEASE(sysTex);
}

bgfx::TextureHandle RenderDevice::UploadTexture(BaseTexture* const surf, int* const pTexWidth, int* const pTexHeight, const bool linearRGB)
{
	const int texwidth = surf->width();
	const int texheight = surf->height();

	if (pTexWidth)
	{
		*pTexWidth = texwidth;
	}
	if (pTexHeight)
	{
		*pTexHeight = texheight;
	}

	const BaseTexture::Format basetexformat = surf->m_format;

	bgfx::TextureHandle sysTex = CreateSystemTexture(surf, linearRGB);

	const ColorFormat texformat = (m_compress_textures && ((texwidth & 3) == 0) && ((texheight & 3) == 0) && (texwidth > 256) && (texheight > 256) && (basetexformat != BaseTexture::RGB_FP)) ? ColorFormat::DXT5 : ((basetexformat == BaseTexture::RGB_FP) ? ColorFormat::RGBA32F : ColorFormat::RGBA8);

	bgfx::TextureHandle tex;

	/* 	TODO: HRESULT hr = m_pD3DDevice->CreateTexture(texwidth, texheight, (texformat != ColorFormat::DXT5 && m_autogen_mipmap) ? 0 : sysTex->GetLevelCount(), (texformat != ColorFormat::DXT5 && m_autogen_mipmap) ? textureUsage::AUTOMIPMAP : 0, (D3DFORMAT)texformat, (D3DPOOL)memoryPool::DEFAULT, &tex, NULL);
	if (FAILED(hr))
	{
		ReportError("Fatal Error: out of VRAM!", hr, __FILE__, __LINE__);
	}

	m_curTextureUpdates++;
	hr = m_pD3DDevice->UpdateTexture(sysTex, tex);
	if (FAILED(hr))
	{
		ReportError("Fatal Error: uploading texture failed!", hr, __FILE__, __LINE__);
	}

	SAFE_RELEASE(sysTex);

	if (texformat != ColorFormat::DXT5 && m_autogen_mipmap)
	{
		tex->GenerateMipSubLevels();
	} */

	return tex;
}

bool RenderDevice::SetRenderStateCache(const RenderStates p1, DWORD p2)
{
	if (renderStateCache.find(p1) == renderStateCache.end())
	{
		renderStateCache.emplace(std::pair<RenderStates, DWORD>(p1, p2));
		return false;
	}
	else if (renderStateCache[p1] != p2)
	{
		renderStateCache[p1] = p2;
		return false;
	}
	return true;
}

void RenderDevice::SetRenderState(const RenderStates p1, DWORD p2)
{
	if (SetRenderStateCache(p1, p2))
	{
		return;
	}

	if (p1 == CULLMODE && (g_pplayer && (g_pplayer->m_ptable->m_tblMirrorEnabled ^ g_pplayer->m_ptable->m_reflectionEnabled)))
	{
		if (p2 == RenderDevice::CULL_CCW)
		{
			p2 = RenderDevice::CULL_CW;
		}
		else if (p2 == RenderDevice::CULL_CW)
		{
			p2 = RenderDevice::CULL_CCW;
		}
	}

	uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;

	for (auto const& [key, val] : renderStateCache)
	{
		switch (key)
		{
		case RenderDevice::CULLMODE:
			if (val == RenderDevice::CULL_CW)
			{
				state |= BGFX_STATE_CULL_CW;
			}
			else if (val == RenderDevice::CULL_CCW)
			{
				state |= BGFX_STATE_CULL_CCW;
			}
			break;
		case RenderDevice::ALPHABLENDENABLE:
			if (val == RenderDevice::RS_TRUE)
			{
			}
			break;
		case RenderDevice::SRCBLEND:
			if (val == RenderDevice::SRC_ALPHA)
			{
			}
			else if (val == RenderDevice::ZERO)
			{
			}
			else if (val == RenderDevice::ONE)
			{
			}
			break;
		case RenderDevice::DESTBLEND:
			if (val == RenderDevice::INVSRC_ALPHA)
			{
			}
			else if (val == RenderDevice::INVSRC_COLOR)
			{
			}
			else if (val == RenderDevice::ONE)
			{
			}
			break;
		case RenderDevice::BLENDOP:
			if (val == RenderDevice::BLENDOP_ADD)
			{
			}
			else if (val == RenderDevice::BLENDOP_REVSUBTRACT)
			{
			}
			else
			{
				std::cout << "blend value not implemented: " << val << std::endl;
			}
			break;
		case RenderDevice::DEPTHBIAS:
			std::cout << "depth bias implemented: " << std::endl;
			break;
		case RenderDevice::ZWRITEENABLE:
			if (val == RenderDevice::RS_TRUE)
			{
				state |= BGFX_STATE_WRITE_Z;
			}
			break;
		case RenderDevice::CLIPPLANEENABLE:
			break;
		default:
			std::cout << "key not implemented: " << key << std::endl;
			break;
		}
	}

	// TODO: CHECKD3D(m_pD3DDevice->SetRenderState((D3DRENDERSTATETYPE)p1, p2));
	bgfx::setState(state);

	m_curStateChanges++;
}

static UINT ComputePrimitiveCount(const RenderDevice::PrimitiveTypes type, const int vertexCount)
{
	switch (type)
	{
	case RenderDevice::POINTLIST:
		return vertexCount;
	case RenderDevice::LINELIST:
		return vertexCount / 2;
	case RenderDevice::LINESTRIP:
		return std::max(0, vertexCount - 1);
	case RenderDevice::TRIANGLELIST:
		return vertexCount / 3;
	case RenderDevice::TRIANGLESTRIP:
	case RenderDevice::TRIANGLEFAN:
		return std::max(0, vertexCount - 2);
	default:
		return 0;
	}
}

void RenderDevice::DrawIndexedPrimitiveVB(const RenderDevice::PrimitiveTypes type, const DWORD fvf, VertexBuffer* vb, const DWORD startVertex, const DWORD vertexCount, IndexBuffer* ib, const DWORD startIndex, const DWORD indexCount)
{
	// TODO: VertexDeclaration* declaration = fvfToDecl(fvf);
	// TODO: SetVertexDeclaration(declaration);

	if (m_curVertexBuffer != vb)
	{
		// TODO: const unsigned int vsize = fvfToSize(fvf);
		// TODO: CHECKD3D(m_pD3DDevice->SetStreamSource(0, vb, 0, vsize));
		// TODO: m_curVertexBuffer = vb;
	}

	if (m_curIndexBuffer != ib)
	{
		// TODO: CHECKD3D(m_pD3DDevice->SetIndices(ib));
		m_curIndexBuffer = ib;
	}

	const unsigned int np = ComputePrimitiveCount(type, indexCount);
	m_stats_drawn_triangles += np;

	// TODO: CHECKD3D(m_pD3DDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type, startVertex, 0, vertexCount, startIndex, np));

	vb->Set(startVertex, vertexCount);
	ib->Set(startIndex, np);

	m_curDrawCalls++;
}

unsigned int RenderDevice::Perf_GetNumDrawCalls() const
{
	return m_frameDrawCalls;
}

unsigned int RenderDevice::Perf_GetNumStateChanges() const
{
	return m_frameStateChanges;
}

unsigned int RenderDevice::Perf_GetNumTextureChanges() const
{
	return m_frameTextureChanges;
}

unsigned int RenderDevice::Perf_GetNumParameterChanges() const
{
	return m_frameParameterChanges;
}

unsigned int RenderDevice::Perf_GetNumTechniqueChanges() const
{
	return m_frameTechniqueChanges;
}

unsigned int RenderDevice::Perf_GetNumTextureUploads() const
{
	return m_frameTextureUpdates;
}

unsigned int RenderDevice::Perf_GetNumLockCalls() const
{
	return m_frameLockCalls;
}

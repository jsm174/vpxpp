#include "VertexBuffer.h"
#include "RenderDevice.h"

#include <bx/bx.h>

HRESULT VertexBuffer::Create(const unsigned int vertexCount, const DWORD fvf, VertexBuffer** vBuffer)
{
	*vBuffer = new VertexBuffer();

	if (fvf == MY_D3DFVF_NOTEX2_VERTEX)
	{
		(*vBuffer)->m_layout.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float).add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float).end();
	}

	(*vBuffer)->m_handle = bgfx::createDynamicVertexBuffer(vertexCount, (*vBuffer)->m_layout);

	return S_OK;
}

VertexBuffer::VertexBuffer()
{
}

void VertexBuffer::CopyMemory(const void* pData, const unsigned int start, const unsigned long size)
{
	//const bgfx::Memory* mem = bgfx::makeRef(pData, size);

	const bgfx::Memory* mem = bgfx::alloc(size);
	bx::memCopy(mem->data, pData, size);

	bgfx::update(m_handle, start, mem);
}

void VertexBuffer::Set(const DWORD startVertex, const DWORD vertexCount)
{
	bgfx::setVertexBuffer(0, m_handle, startVertex, vertexCount);
}

void VertexBuffer::unlock()
{
	// TODO: CHECKD3D(this->Unlock());
}

void VertexBuffer::release()
{
	// TODO: SAFE_RELEASE_NO_CHECK_NO_SET(this);
}

void VertexBuffer::lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void** dataBuffer, const DWORD flags)
{
	RenderDevice::m_curLockCalls++;
	// TODO: CHECKD3D(this->Lock(offsetToLock, sizeToLock, dataBuffer, flags));
}

/*

HRESULT CreateVertexBuffer(
  UINT                   Length,
  DWORD                  Usage,
  DWORD                  FVF,
  D3DPOOL                Pool,
  IDirect3DVertexBuffer9 **ppVertexBuffer,
  HANDLE                 *pSharedHandle
);

*/
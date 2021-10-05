#include "IndexBuffer.h"
#include "RenderDevice.h"

#include <bx/bx.h>

HRESULT IndexBuffer::Create(const unsigned int indexCount, Format format, IndexBuffer** idxBuffer)
{
	*idxBuffer = new IndexBuffer();

	(*idxBuffer)->m_format = format;
	(*idxBuffer)->m_handle = bgfx::createDynamicIndexBuffer(indexCount);

	return S_OK;
}

void IndexBuffer::CopyMemory(const void* pData, const unsigned int start, const unsigned long size)
{
	//const bgfx::Memory* mem = bgfx::makeRef(pData, size);

	const bgfx::Memory* mem = bgfx::alloc(size);
	bx::memCopy(mem->data, pData, size);

	bgfx::update(m_handle, start, mem);
}

void IndexBuffer::Set(const DWORD startIndex, const DWORD indexCount)
{
	bgfx::setIndexBuffer(m_handle, startIndex, indexCount);
}

IndexBuffer::IndexBuffer()
{
}

void IndexBuffer::unlock()
{
	// TODO: CHECKD3D(this->Unlock());
}

void IndexBuffer::release()
{
	// TODO: SAFE_RELEASE_NO_CHECK_NO_SET(this);
}

void IndexBuffer::lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void** dataBuffer, const DWORD flags)
{
	RenderDevice::m_curLockCalls++;
	// TODO: CHECKD3D(this->Lock(offsetToLock, sizeToLock, dataBuffer, flags));
}
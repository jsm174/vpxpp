#include "VertexBuffer.h"
#include "RenderDevice.h"

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
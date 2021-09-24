#include "IndexBuffer.h"
#include "RenderDevice.h"

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
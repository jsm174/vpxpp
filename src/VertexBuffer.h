#pragma once

#include <bgfx/bgfx.h>
#include "misc.h"

class VertexBuffer // TODO: public IDirect3DVertexBuffer9
{
public:
	static HRESULT Create(const unsigned int vertexCount, const DWORD fvf, VertexBuffer** vBuffer);

	void CopyMemory(const void* pData, const unsigned int start, const unsigned long size);
	void Set(const DWORD startVertex, const DWORD vertexCount);

	enum LockFlags
	{
		WRITEONLY = 0, // in DX9, this is specified during VB creation
					   // TODO: NOOVERWRITE = D3DLOCK_NOOVERWRITE,    // meaning: no recently drawn vertices are overwritten. only works with dynamic VBs.
					   // it's only needed for VBs which are locked several times per frame
					   // TODO: DISCARDCONTENTS = D3DLOCK_DISCARD     // discard previous contents; only works with dynamic VBs
	};

	void lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void** dataBuffer, const DWORD flags);

	void unlock();
	void release();

private:
	VertexBuffer();

	bgfx::DynamicVertexBufferHandle m_handle;
	bgfx::VertexLayout m_layout;
};
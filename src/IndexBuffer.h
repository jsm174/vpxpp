#pragma once

#include "misc.h"
#include <bgfx/bgfx.h>

class IndexBuffer // TODO: public public IDirect3DIndexBuffer9
{
public:
	enum Format
	{
		FMT_INDEX16 = 0, // TODO: D3DFMT_INDEX16,
		FMT_INDEX32 = 1	 // TODO: D3DFMT_INDEX32
	};

	static HRESULT Create(const unsigned int indexCount, Format format, IndexBuffer** idxBuffer);

	void CopyMemory(const void* pData, const unsigned int start, const unsigned long size);
	void Set(const DWORD startIndex, const DWORD indexCount);

	enum LockFlags
	{
		WRITEONLY = 0, // in DX9, this is specified during VB creation
					   // TODO: NOOVERWRITE = //D3DLOCK_NOOVERWRITE,  // meaning: no recently drawn vertices are overwritten. only works with dynamic VBs.
					   // it's only needed for VBs which are locked several times per frame
					   // TODO: DISCARD = D3DLOCK_DISCARD           // discard previous contents; only works with dynamic VBs
	};

	void lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void** dataBuffer, const DWORD flags);

	void unlock();
	void release();

private:
	IndexBuffer();

	bgfx::DynamicIndexBufferHandle m_handle;
	Format m_format;
};
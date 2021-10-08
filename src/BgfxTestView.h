#pragma once

#include <bgfx/bgfx.h>

class BgfxTestView
{
public:
	BgfxTestView();
	void Update(int width, int height);

private:
	int64_t m_timeOffset;

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	bgfx::ProgramHandle m_ph;
};
